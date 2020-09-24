//------------------------------------------------------------------------------
///@file lib_msg_publisherconnectionhelper.h                                    
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG  Replaced #include <boost/shared_ptr.hpp>           
//------------------------------------------------------------------------------

#ifndef LIB_MSG_PUBLISHERCONNECTIONHELPER_H_FILE_GUARD
#define LIB_MSG_PUBLISHERCONNECTIONHELPER_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"
#include "lib_msg_actualconnection.h"
#include "lib_msg_publication.h"
#include "lib_msg_subscription.h"


namespace lib {
namespace msg {


//------------------------------------------------------------------------------
///@brief   Aide in connecting publications to subscriptions.                   
///                                                                             
///         At compile time, we need to determine which of the Publisher's types
///         are compatible with which of the Subscriber's types.  We want each  
///         of the Publisher's types to go to at most one of the Subscriber's   
///         types.  We prefer same-type to same-type and then derived-type to   
///         base-type.  This class allows us to determine the same/base-derived 
///         at compilie time so that we don't have code (never executed) that   
///         tries to call methods that don't exist.                             
///                                                                             
///         This code can be greatly simplified once specialized methods are    
///         allowed (as opposed to specialized classes).                        
///                                                                             
///@note Convertible                                                            
///         It's not so much that PUB_TYPE is convertible to SUB_TYPE but       
///         rather that boost::shared_ptr<const PUB_TYPE> is convertible to     
///         boost::shared_ptr<const SUB_TYPE>.                                  
///                                                                             
///@par Alternative (non-working) Example:                                      
///         Assume we had the following code:                                   
///         @code                                                               
///             template<typename SUB_TYPE, typename PUB_TYPE>                  
///             int connect(Subscription<SUB_TYPE>& s, Publication<PUB_TYPE>& p)
///             {                                                               
///                 if ((std::is_same<SUB_TYPE, PUB_TYPE>::value)) {          
///                     p.connect(s);                                           
///                 }                                                           
///             }                                                               
///         @endcode                                                            
///\n\n                                                                         
///         If the PUB_TYPE was completely independent of the SUB_TYPE then the 
///         above code would result in a compilation error even though the if   
///         statement would block it from ever being executed.                  
///\n\n                                                                         
///         To avoid this, we use partially instantiated templates to prevent   
///         code that will never be executed from generating compilation errors.
///                                                                             
///@warning There is very little reason to use this class-template directly.    
///         It is used by the Publisher::connect method.                        
///                                                                             
///@note Parameter Ordering                                                     
///         The SUB_TYPE appears before PUB_TYPE to match the boost::           
///         is_base_and_derived ordering.  The SUB_TYPE has to be a base of     
///         PUB_TYPE for things to work.                                        
///                                                                             
///@tparam  SAME        True if the SUB_TYPE and PUB_TYPE are the same type.    
///@tparam  DERIVED     True if the SUB_TYPE is a base class of PUB_TYPE.       
///@tparam  SUB_TYPE    The type the Subscription subscribes to.                
///@tparam  PUB_TYPE    The type the Publication publishes to.                  
///                                                                             
//------------------------------------------------------------------------------
template<
    bool SAME
  , bool DERIVED
  , bool CONVERTIBLE
  , typename SUB_TYPE
  , typename PUB_TYPE
>
class PublisherConnectionHelper
{
    public:
        //----------------------------------------------------------------------
        ///@brief   Make the connection.                                        
        ///                                                                     
        ///         The default, not the same and not base-derived relationship 
        ///         implies no connection can be made.                          
        //----------------------------------------------------------------------
        static int connect(
            Subscription<SUB_TYPE>* s
          , Publication<PUB_TYPE>* p
          , bool sameOnly
          , bool derivedOnly
        )
        {
            return 0;
        }
};


//------------------------------------------------------------------------------
///@brief   Connect the publication and subscription if the published type and  
///         subscribed-to type are the same.                                    
///                                                                             
///         We don't care if it's derived or convertible.  They're the same.    
///                                                                             
///@tparam  DERIVED     Probably false, but we don't care what it is.           
///@tparam  SUB_TYPE    The type the Subscription subscribes to.                
///@tparam  PUB_TYPE    The type the Publication publishes.                     
//------------------------------------------------------------------------------
template<bool DERIVED, bool CONVERTIBLE, typename SUB_TYPE, typename PUB_TYPE>
class PublisherConnectionHelper<true, DERIVED, CONVERTIBLE, SUB_TYPE, PUB_TYPE>
{
    public:
        static int connect(
            Subscription<SUB_TYPE>* s
          , Publication<PUB_TYPE>* p
          , bool sameOnly
          , bool derivedOnly
        )
        {
            return actualConnection(s, p);
        }
};


//------------------------------------------------------------------------------
///@brief   Connect the publisher and subscriber if the publication type is     
///         derived from the subscription type and derived is true.             
///                                                                             
///         They are not the same, but PUB_TYPE is derived from SUB_TYPE.       
///                                                                             
///@tparam  SUB_TYPE    The type the Subscription subscribes to.                
///@tparam  PUB_TYPE    The type the Publication publishes.                     
//------------------------------------------------------------------------------
template<bool CONVERTIBLE, typename SUB_TYPE, typename PUB_TYPE>
class PublisherConnectionHelper<false, true, CONVERTIBLE, SUB_TYPE, PUB_TYPE>
{
    public:
        static int connect(
            Subscription<SUB_TYPE>* s
          , Publication<PUB_TYPE>* p
          , bool sameOnly
          , bool derivedOnly
        )
        {
            int result (0);
            if (!sameOnly) {
                result = actualConnection(s, p);
            }
            return result;
        }
};


//------------------------------------------------------------------------------
///@brief   Connect the publisher and subscriber if the publication type is     
///         derived from the subscription type and derived is true.             
///                                                                             
///         They aren't the same and PUB_TYPE is not derived from SUB_TYPE, but 
///         PUB_TYPE is conertbile to SUB_TYPE.                                 
///                                                                             
///@tparam  SUB_TYPE    The type the Subscription subscribes to.                
///@tparam  PUB_TYPE    The type the Publication publishes.                     
//------------------------------------------------------------------------------
template<typename SUB_TYPE, typename PUB_TYPE>
class PublisherConnectionHelper<false, false, true, SUB_TYPE, PUB_TYPE>
{
    public:
        static int connect(
            Subscription<SUB_TYPE>* s
          , Publication<PUB_TYPE>* p
          , bool sameOnly
          , bool derivedOnly
        )
        {
            int result (0);
            if (!sameOnly && !derivedOnly) {
                result = actualConnection(s, p);
            }
            return result;
        }
};



} // namespace msg
} // namespace lib


#endif // #ifndef LIB_MSG_PUBLISHERCONNECTIONHELPER_H_FILE_GUARD

