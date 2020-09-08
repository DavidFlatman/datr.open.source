//------------------------------------------------------------------------------
///@file lib_msg_subscriptionbase.h                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MSG_SUBSCRIPTIONBASE_H_FILE_GUARD
#define LIB_MSG_SUBSCRIPTIONBASE_H_FILE_GUARD

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

namespace lib {
namespace msg {

class PublisherBase;

//------------------------------------------------------------------------------
///@class SubscriptionBase                                                      
///                                                                             
///@brief The base of the Subscription class, used to get a non-template based  
///       pointer to the object and then back to the templated version.         
///                                                                             
///@par Purpose:                                                                
///         This class is solely in support of the Subscriber class.  The       
///         Subscriber template needs a single queue effectively holding all    
///         published items (in order to process them in the order received).   
///\n\n                                                                         
///         The purpose of this class is to provide a consistent type for all   
///         the subscribed to items by stripping the template parameter from    
///         subscription, but having a pure virtual process method that         
///         allows us to get back to the templated Subscription version.        
///\n\n                                                                         
///         In this way we can have a std::queue<SubscriptionBase*> and still   
///         get to the derived object's process method.                         
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-08-30  DHF     Moved to its own file.                          
///                                                                             
///@version 2009-09-09  DHF     Class creation.                                 
///                                                                             
//------------------------------------------------------------------------------
class SubscriptionBase 
{
    public:
        virtual ~SubscriptionBase() 
        {
        }
        virtual void processQueueItem() = 0;

        virtual int publicationCount() const = 0;

    protected:

    private:
        //----------------------------------------------------------------------
        ///@brief  Allows Subscriber to track the number of subscriptions the   
        ///        object is listening for.                                     
        ///                                                                     
        ///@warning This method is automatically called by the connect method of
        ///         lib::msg::Publisher.  There should be no reason to call     
        ///         this method directly.                                       
        ///                                                                     
        ///@par Design Ramblings                                                
        ///         This is the first time that I've ever considered making a   
        ///         method private and then declaring the one and only class    
        ///         that should call it @e friend.                              
        ///\n\n                                                                 
        ///         If this method is called in any other context, it will      
        ///         mess up the Subscriber.                                     
        ///\n\n                                                                 
        ///         As any of my coworkers can tell you, I hate friends.  But   
        ///         in this first case it may actually be appropriate.  (Let the
        ///         debate begin).                                              
        ///                                                                     
        //----------------------------------------------------------------------
        virtual void incrementPublicationCount(
            boost::signals2::connection& connection1
          , boost::signals2::connection& connection2
        ) = 0;
        virtual void publicationEnding() = 0;

        //----------------------------------------------------------------------
        ///@todo    I hate friends (I know, I've said this before, repeatedly). 
        ///         The friend declaration can be eliminated by having a        
        ///         common base class betwen PublisherBase and SubscriptionBase.
        //----------------------------------------------------------------------
        friend class lib::msg::PublisherBase;

};

} // namespace msg
} // namespace lib

#endif // #ifndef LIB_MSG_SUBSCRIPTIONBASE_H_FILE_GUARD

