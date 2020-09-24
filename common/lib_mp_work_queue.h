//------------------------------------------------------------------------------
///@file lib_mp_work_queue.h                                                    
///@brief Holds the definition of the thread safe lib::mp::Queue template.      
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MP_QUEUE_H
#define LIB_MP_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <deque>

namespace lib {
namespace mp {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@brief   A thread safe queue.                                                
///                                                                             
///@par Template: queue                                                         
///                                                                             
///@par Purpose:                                                                
///         This is a thread safe implementation of a queue.  Not all the parts 
///         of std::queue are reimplemented here ... only the parts that I need.
///         Furthermore, the call signature of the pop method is changed to     
///         account for multithreading (specifically, pop and front are merged).
///                                                                             
///@par Design Note:  One Reader Thread and One Writer Thread                   
///         The initial design of this class is for one thread to be writing    
///         to the Queue and one other thread reading from the Queue.  If more  
///         threads are supposed to be reading from the Queue, then the         
///         interrupt should probably be changed from notify_one to notify_all. 
///                                                                             
///@par Thread Safety:  object                                                  
///                                                                             
///@version 2020-09-23  JRS     replace boost references with std references.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2019-10-02  DHF     Made maximumSize() constant.                    
///                                                                             
///@version 2015-11-23  DHF     Add abort mechanism.                           
///                             Changed underlying structure to a std::dequeue  
///                             in order to have the clear() method.            
///                                                                             
///@version 2014-10-28  DHF     Added maximum size.                             
///                                                                             
///@version 2013-10-30  DHF     The empty() symantics were confused with clear. 
///                                                                             
///@version 2013-04-08  DHF     Changed maximumSize and setMaximumSize to       
///                             governor and setGovernor.                       
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-08-10  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
class Queue 
{
    public:
        //----------------------------------------------------------------------
        ///@param max_size  Control the maximum size of the queue.  If the queue
        ///                 is at the maximum size and an attempt to add to it  
        ///                 occurs, the adding thread will hang until something 
        ///                 is removed from the queue.                          
        //----------------------------------------------------------------------
        Queue(size_t max_size = 0)
            : m_Governor(max_size)
            , m_MaximumSize(0)
            , m_Interrupt(false)
            , m_Aborted(false)
        {
        }

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        virtual ~Queue() {}

        //----------------------------------------------------------------------
        ///@brief   Add an item to the queue.                                   
        ///@param   item    The item to add to the back of the queue.           
        ///@param   use_governor True = if the queue would exceed max size, wait
        ///                     False = stuff the data in the queue regardless  
        //----------------------------------------------------------------------
        void push(TYPE item)
        {
            if (m_Aborted) return;

            //------------------------------------------------------------------
            /// @todo size check?                                               
            //------------------------------------------------------------------
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            if (m_Governor > 0 && m_Governor <= m_Queue.size()) {
                m_QueueReady.wait(lock);
                if (m_Aborted) return;
            }

            //std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_Queue.push_back(item);
            if (m_Queue.size() > m_MaximumSize) m_MaximumSize = m_Queue.size();
            lock.unlock();

            m_DataReady.notify_one();
        }

        //----------------------------------------------------------------------
        ///@brief       Remove and return the item from the front of the queue. 
        ///@warning     If the queue is empty, rather than causing an error,    
        ///             pop will hang the calling thread until there is         
        ///             something to read or another thread calls setInterrupt  
        ///             to wake up the queue.                                   
        ///@param       item The item popped off the stack^H^H^H^H^H queue.     
        ///@return      true = an item was returned from the queue;             
        ///             false = an item was not returned from the queue, but    
        ///                     an interrupt was received with an empty queue.  
        //----------------------------------------------------------------------
        bool pop(TYPE& item)
        {
            if (m_Aborted) return false;

            std::unique_lock<std::mutex> lock(m_QueueMutex);
            while (m_Queue.empty() && !m_Interrupt && !m_Aborted) {
                m_DataReady.wait(lock);
            }

            if (m_Aborted) return false;

            bool result = !m_Queue.empty();
            if (result) {
                item = m_Queue.front();
                m_Queue.pop_front();

                m_QueueReady.notify_one();
            }

            return result;
        }

        //----------------------------------------------------------------------
        ///@brief   Return the number of items currently in the queue (subject  
        ///         to change without notice).                                  
        //----------------------------------------------------------------------
        size_t size() const {
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            return m_Queue.size(); 
        }

        size_t governor() const {
            return m_Governor;
        }

        void setGovernor(size_t ms) {
            m_Governor = ms;
        }

        //----------------------------------------------------------------------
        ///@brief   Return true if the queue is empty.                          
        //----------------------------------------------------------------------
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            return m_Queue.empty();
        }

        //----------------------------------------------------------------------
        ///@brief   Change the interrupt value and wake up someone waiting for  
        ///         the queue lock.                                             
        ///@param value The value to set the interrupt to (almost always true). 
        //----------------------------------------------------------------------
        void setInterrupt(bool value = true)
        {
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_Interrupt = value;
            m_DataReady.notify_one();
        }

        //----------------------------------------------------------------------
        ///@brief   Return the maximum size that the queue ever grew to.        
        //----------------------------------------------------------------------
        uint64_t maximumSize() 
        {
            return m_MaximumSize;
        }

        //----------------------------------------------------------------------
        ///@brief   Used when the queue will no longer be used.                 
        //----------------------------------------------------------------------
        void abort()
        {
            m_Aborted = true;
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_Queue.clear();
            m_QueueReady.notify_all();
            m_DataReady.notify_all();
        }

    protected:

    private:
        //----------------------------------------------------------------------
        //  Try to live without these ... copying the underlying queue may make 
        //  sense, but copying the locks doesn't.                               
        //----------------------------------------------------------------------
        Queue& operator=(const Queue& that);
        Queue(const Queue& that);

        //----------------------------------------------------------------------
        ///@brief The most number of items that the queue should hold.          
        //----------------------------------------------------------------------
        size_t m_Governor;

        //----------------------------------------------------------------------
        ///@brief   The largest size the queue ever grew to.                    
        //----------------------------------------------------------------------
        uint64_t m_MaximumSize;

        //----------------------------------------------------------------------
        ///@brief The actual holder of the data.                               
        //----------------------------------------------------------------------
        std::deque<TYPE> m_Queue;

        //----------------------------------------------------------------------
        ///@brief The lock to use before accessing m_Queue; doesn't really      
        ///       change the state, therefore mutable.                          
        //----------------------------------------------------------------------
        mutable std::mutex m_QueueMutex;

        //----------------------------------------------------------------------
        ///@brief Used to wake up a thread waiting to get data out of the queue.
        //----------------------------------------------------------------------
        std::condition_variable m_DataReady;

        std::condition_variable m_QueueReady;

        //----------------------------------------------------------------------
        ///@brief Simplistically, indicates if setInterrupt was issued; used to 
        ///       break out of the "pop" even if there's no data.               
        //----------------------------------------------------------------------
        bool m_Interrupt;

        //----------------------------------------------------------------------
        ///@brief   Someone signaled that the queue is to terminate/stop/abort. 
        //----------------------------------------------------------------------
        bool m_Aborted;

}; // class Queue //

}  // namespace work //
}  // namespace mp //
}  // namespace lib //

#endif // #ifndef LIB_DATA_CONSUMER_H //
