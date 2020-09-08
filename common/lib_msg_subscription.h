//------------------------------------------------------------------------------
///@file lib_msg_subscription.h                                                 
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MSG_SUBSCRIPTION_H_FILE_GUARD
#define LIB_MSG_SUBSCRIPTION_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"
#include "lib_msg_subscriptionbase.h"

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Subscription                                                     
///                                                                             
///@brief Template to act as a base to Subscriber; represents one subscription  
///       type.                                                                 
///                                                                             
///@tparam TYPE     The base type being subscribed to.  The actual object       
///                 passed will be boost::shared_ptr<const TYPE>.               
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                                                                             
///@version 2016-11-14  DHF     Removed knowledge of lib::mp::Queue.            
///                             Added addToParentQueue as virtual.              
///                                                                             
///@version 2015-08-30  DHF     Moved to its own file.                          
///                                                                             
///@version 2013-08-28  DHF     Modified it so that only boost::shared_ptr<const
///                             objects can be used.                            
///                                                                             
///@version 2009-09-10  DHF     Added pure virutal incrementSubscriptiontCount  
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
class Subscription : public SubscriptionBase
{
    public:
        Subscription()
        {
        }

        virtual ~Subscription() 
        {
        }

        //----------------------------------------------------------------------
        ///                                                                     
        ///@brief Pure virtual process method; where the real work of processing
        ///       a published item is performed.                                
        ///                                                                     
        //----------------------------------------------------------------------
        virtual void process(lib::ds::shared_ptr<const TYPE>&  item) = 0;

        //----------------------------------------------------------------------
        ///                                                                     
        ///@brief Grab a published item off the queue and process it.           
        ///                                                                     
        ///@note  Implements the SubscriptionBase::process pure virutal method. 
        ///                                                                     
        //----------------------------------------------------------------------
        void processQueueItem() {
            lib::ds::shared_ptr<const TYPE> item;

            if (m_Queue.pop(item)) {
                process(item);
            }

        }

        //----------------------------------------------------------------------
        ///                                                                     
        ///@brief Receive another item from the Publisher to put onto our queue 
        ///       to eventually process.                                        
        ///                                                                     
        ///@note  This method is generally executed in the Publisher's thread   
        ///       context, not the Subscriber's.                                
        ///                                                                     
        //----------------------------------------------------------------------
        virtual void inbox(lib::ds::shared_ptr<const TYPE>  item) {
            m_Queue.push(item);
            addToParentQueue(this);
        }

    protected:
        //----------------------------------------------------------------------
        ///@brief   Added item to the master queue for needing to be  executed  
        ///         next.                                                       
        //----------------------------------------------------------------------
        virtual void addToParentQueue(SubscriptionBase* item) 
        {
        }

    private:
        lib::mp::work::Queue<lib::ds::shared_ptr<const TYPE> > m_Queue;

}; // class Subscription //


} // namespace msg
} // namespace lib


#endif // #ifndef LIB_MSG_SUBSCRIPTION_H_FILE_GUARD

