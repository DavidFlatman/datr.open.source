//------------------------------------------------------------------------------
///@file lib_mp_work_threadablecollection.cpp                                   
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Allow control (join, start) of multiple threads at one time.        
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Changed (*this)[j]->thread() == NULL to         
///                             (*this)[j]->thread().get() == nullptr           
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                                                                             
///@version 2011-03-31  DHF         File creation                               
///                                                                             
//------------------------------------------------------------------------------

#include "lib_mp_work_threadablecollection.h"
#include "lib_log_work_messagefactory.h"
#include "lib_mp_work_thread.h"

#include <boost/scoped_array.hpp>

namespace lib {
namespace mp {
namespace work {

const lib::log::ds::messageid_t  ThreadableCollection::MSG_THREAD_COMPLETED (1);
const lib::log::ds::messageid_t  ThreadableCollection::MSG_THREADS_STARTED  (2);
const lib::log::ds::messageid_t  ThreadableCollection::MSG_THREADS_COMPLETED(3);

static const lib::log::work::MessageFactory s_Message(
    lib::log::ds::LIB_MP_THREADABLE_COLLECTION
);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ThreadableCollection::ThreadableCollection()
    : m_IsStarted(false)
{
} // ThreadableCollection::ThreadableCollection() //

//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
ThreadableCollection::~ThreadableCollection()
{

} // ThreadableCollection::~ThreadableCollection

//------------------------------------------------------------------------------
///@brief                                                                       
///@param   checkAllIsWell   This boolean determines whether the method         
///                         automatically checks that all is well with the      
///                         Threadable objects.  If isAllIsWell check is        
///                         performed before calling this routine, pass "false" 
///                         for this value.                                     
//------------------------------------------------------------------------------
void ThreadableCollection::startAll(bool checkAllIsWell)
{
    int s = int(size());

    for (int i = 0; i < s; ++i) {
        if (checkAllIsWell) (*this)[i]->checkIfAllIsWell();
    }

    //--------------------------------------------------------------------------
    //  Start the threadables in reverse order (odds are the publishers are     
    //  put in first so we want the dependents started and ready to catch the   
    //  data from the others).                                                  
    //--------------------------------------------------------------------------
    for (int i = 0; i < s; ++i) {
        int j = s - i - 1;

        //----------------------------------------------------------------------
        //  Don't try to restart something that already has a thread.           
        //----------------------------------------------------------------------
        if ((*this)[j]->thread().get() == nullptr) {

            if ((*this)[j]->beforeThreadStarts()) {
                (*this)[j]->setThread(
                    ThreadPtr(
                        new Thread((*this)[j]->name(), (*this)[j].get())
                    )
                );

                publish(
                    s_Message.debug(
                        MSG_THREADS_STARTED
                      , "started   thread:  " + (*this)[j]->name() 
                    )
                );
            }
        }
    }

    publish(s_Message.debug(MSG_THREADS_STARTED, "threads started"));

    m_IsStarted = true;

} // void ThreadableCollection::startAll() //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void ThreadableCollection::joinAll()
{
    for (iterator t = begin(); t != end(); ++t) {
        if ((*t)->thread()->joinable()) {
            (*t)->thread()->join();
            publish(
                s_Message.debug(
                    MSG_THREAD_COMPLETED
                  , "thread " + (*t)->name() + " stopped"
                )
            );
            (*t)->afterJoin();
        }
    }
    publish(s_Message.debug(MSG_THREADS_COMPLETED, "threads completed"));
    endPublication();

} // void ThreadableCollection::joinAll() //


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void ThreadableCollection::joinAllDebug()
{                                                                               
    boost::scoped_array<bool> done(new bool[size()]);                           
                                                                                
    for (size_t d = 0; d < size(); ++d) {                                       
        done[d] = false;                                                        
    }                                                                           
                                                                                
    size_t completed = 0;                                                       
                                                                                
    while (completed < size()) {                                                
        for (size_t t = 0; t < size(); ++t) {                                   
            if (!done[t]) {                                                     
                lib::ds::shared_ptr<Thread> thread((*this)[t]->thread());
                                                                                
                boost::posix_time::time_duration timeout(                       
                    boost::posix_time::milliseconds(1000)                       
                );                                                              
                                                                                
                if (thread->timed_join(timeout)) {                              
                    (*this)[t]->afterJoin();                                    
                                                                                
                    ++completed;                                                
                    done[t] = true;                                             
                    publish(                                                    
                        s_Message.debug(                                        
                            MSG_THREAD_COMPLETED                                
                          , "completed thread: " + thread->name()               
                        )                                                       
                    );                                                          
                                                                                
                } // if (thread->timed_join(timeout)) //                        
                                                                                
            } // if (!done[t]) //                                               
                                                                                
        } // for (size_t t = 0; t < size(); ++t) //                             
                                                                                
    } // while (completed < size()) //                                          
    publish(s_Message.debug(MSG_THREADS_COMPLETED, "threads completed"));       
    endPublication();                                                           
                                                                                
} // void ThreadableCollection::joinAllDebug() //


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool ThreadableCollection::isStarted() const
{
    return m_IsStarted;
}
    
} // namespace work //
} // namespace mp //
} // namespace lib //
