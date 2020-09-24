//------------------------------------------------------------------------------
///@file lib_msg_publisherbase.h                                                
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MSG_PUBLISHERBASE_H_FILE_GUARD
#define LIB_MSG_PUBLISHERBASE_H_FILE_GUARD

#include <boost/signals2.hpp>
#include "lib_msg_subscriptionbase.h"

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///                                                                             
///@brief Base class to the Publisher class.                                    
///                                                                             
///@par Purpose:                                                                
///         As any of my coworkers can tell you, I hate friends.  The purpose   
///         of this class is to allow the template Publisher the ability to     
///         increment the subscription count on a Subscriber.                   
///\n\n                                                                         
///         There is no reason for anyone else to increment a subscription      
///         count; doing so will mess up the Subscriber.  To protect the        
///         template derived Subscriber, the increment count is declared        
///         private.  We would declare the template Publisher a friend of       
///         the given Subscriber, but C++ won't allow a templated class to be   
///         a friend.                                                           
///\n\n                                                                         
///         So, we have a non-templated PublisherBase class that is declared a  
///         friend of the SubscriptionBase class (neither one templated) so     
///         that we can declare the Publisher a friend of the Subscriber        
///         (indirectly).                                                       
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-08-30  DHF     Broke out into file of it's own.                
///                                                                             
///@version 2014-07-15  DHF     Added the connection objects for subscriber     
///                             to disconnect from if permature termination.    
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-09-10  DHF     Class creation (do I have to admit to writing   
///                             this?)                                          
//------------------------------------------------------------------------------
class PublisherBase
{
    public:
        inline virtual ~PublisherBase() 
        = default;

    protected:
        inline PublisherBase() = default;

        //----------------------------------------------------------------------
        ///@brief Increment the subscription count of the given SubscriptionBase
        //----------------------------------------------------------------------
        inline void incrementPublicationCount(
            SubscriptionBase& sub
          , boost::signals2::connection& connection1
          , boost::signals2::connection& connection2
        ) {
            sub.incrementPublicationCount(connection1, connection2);
        }

        //----------------------------------------------------------------------
        ///@brief Connect the PublicationEnding signal to the SubscriptionBase  
        ///       publicationEnding.                                            
        //----------------------------------------------------------------------
        inline void connectPublicationEnding(
            boost::signals2::connection& connection
          , SubscriptionBase& sub
          , boost::signals2::signal<void ()>& signal
        ) {
            connection = signal.connect(
                std::bind(&SubscriptionBase::publicationEnding, &sub)
            );
        }

}; // class PublisherBase

} // namespace msg
} // namespace lib


#endif // #ifndef LIB_MSG_PUBLISHERBASE_H_FILE_GUARD

