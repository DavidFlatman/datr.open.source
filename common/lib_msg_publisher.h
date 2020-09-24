//------------------------------------------------------------------------------
///@file lib_msg_publisher.h                                                    
///                                                                             
///@brief Hold the definition of the lib::msg::Publisher template.              
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@author  David H. Flatman    DHF         davidflatman@email.com              
///                                                                             
//------------------------------------------------------------------------------
#ifndef LIB_MSG_PUBLISHER_H
#define LIB_MSG_PUBLISHER_H

#include "lib_cast.h"
#include "lib_ds_null.h"
#include "lib_ds_shared_ptr.h"
#include "lib_msg_subscriber.h"
#include "lib_msg_publication.h"
#include "lib_msg_publisherhelper.h"
#include "lib_msg_publisherconnectionhelper.h"
#include "lib_string.h"                             // lib::format          

#include <stdint.h>

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///                                                                             
///@brief   Allow a derived class to publish multiple publications (type).      
///                                                                             
///@par Design Ramblings:                                                       
///         The original approach was to just leave things at the Publication   
///         level.  If a class wanted to publish mutiple types, then it would   
///         multiply inherit from Publication (this approach still works).      
///\n\n                                                                         
///         The drawback to this approach is that when a connection is made     
///         the type needs to be specified.  Assume that a class publishes      
///         a DateTimePtr and a PacketPtr.  The connection code would then      
///         look like:                                                          
///         @code                                                               
///             myPub.lib::msg::Publication<DateTimePtr>::connect(mySub);       
///         @endcode                                                            
///\n\n                                                                         
///         With this approach, the code can look like:                         
///         @code                                                               
///             myPub.connect(mySub).                                           
///         @endcode                                                            
///                                                                             
///@note    The @e actual types that are published are shared pointers of the   
///         type boost::shared_ptr<const TYPE> versus just TYPE.  See           
///         lib::msg::Publication for rationale.                                
///                                                                             
///@tparam  TYPE0   [required] A type that the object is going to publish.      
///@tparam  TYPE1   [optional] A secondary type that the object is may          
///                 publish.  (e.g., the primary type may be a CH10 packet and  
///                 the secondary type status information).                     
///@tparam  TYPE2   [optional] Analogous to TYPE1.                              
///                                                                             
///@par Design Trade Off:                                                       
///         The drawback (and this can be considered a big one) is that we lose 
///         compile time type checking on @e connect and replace it with        
///         runtime type checking (yeah, I don't like it either).               
///\n\n                                                                         
///         If you don't want to sacrifice compile-time type checking then      
///         just inherit multiple times from Publication versus using Publisher.
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                                                                             
///@version 2019-02-20  DHF     -   Added m_EndPublicationCalled to prevent     
///                                 multiple endPublication calls.              
///                             -   Minor code cleanup.                         
///                             -   Made the template parameters accessible to  
///                                 other code via the using statements.        
///                                                                             
///@version 2018-07-19  DHF     Added operator>> for connecting publishers to   
///                             subscribers.                                    
///                                                                             
///@version 2016-12-14  DHF     Made the throw message more descriptive if there
///                             are no common types between the publisher and   
///                             the subscriber.                                 
///                                                                             
///@version 2016-11-01  DHF     Moved lib::msg::NULL* to lib::ds::NULL*         
///                                                                             
///@version 2014-07-15  DHF     Added subscriptionCount()                       
///                                                                             
///@version 2013-12-11  DHF     Removed connect (std::vector<SubscriberBase>&)  
///                             which was not being used (and had too much      
///                             potential for abuse).                           
///                                                                             
///@version 2011-03-14  DHF     Added connect for containers.                   
///                                                                             
//------------------------------------------------------------------------------
template <
    typename TYPE0
  , typename TYPE1 = ds::NULL_DEFAULT_TYPE_1
  , typename TYPE2 = ds::NULL_DEFAULT_TYPE_2
  , typename TYPE3 = ds::NULL_DEFAULT_TYPE_3
  , typename TYPE4 = ds::NULL_DEFAULT_TYPE_4
  , typename TYPE5 = ds::NULL_DEFAULT_TYPE_5
  , typename TYPE6 = ds::NULL_DEFAULT_TYPE_6
  , typename TYPE7 = ds::NULL_DEFAULT_TYPE_7
  , typename TYPE8 = ds::NULL_DEFAULT_TYPE_8
>
class Publisher
    : public Publication<TYPE0>
    , public Publication<TYPE1>
    , public Publication<TYPE2>
    , public Publication<TYPE3>
    , public Publication<TYPE4>
    , public Publication<TYPE5>
    , public Publication<TYPE6>
    , public Publication<TYPE7>
    , public Publication<TYPE8>
{
    public:
        //----------------------------------------------------------------------
        //  Give non-templated code access to the template parameters.          
        //----------------------------------------------------------------------
        using Type0 = TYPE0;
        using Type1 = TYPE1;
        using Type2 = TYPE2;
        using Type3 = TYPE3;
        using Type4 = TYPE4;
        using Type5 = TYPE5;
        using Type6 = TYPE6;
        using Type7 = TYPE7;
        using Type8 = TYPE8;

        //----------------------------------------------------------------------
        //  If a publisher and subscriber have no common types, that is a logic 
        //  error.                                                              
        //----------------------------------------------------------------------
        using ConnectionError = std::logic_error;

        Publisher() : m_EndPublicationCalled(false) {  }
        virtual ~Publisher() = default;

        template <typename TYPE>
        void publish(lib::ds::shared_ptr<const TYPE>  item) {
            this->Publication<TYPE>::publish(item);
        }

        template <typename TYPE>
        void publish(lib::ds::shared_ptr<TYPE> item) {
            lib::ds::shared_ptr<const TYPE> temp(item);
            this->Publication<TYPE>::publish(temp);
        }

        //----------------------------------------------------------------------
        ///@brief   Connect the given subscriber to as many items we publish.   
        ///                                                                     
        ///         The trick to this routine is to find at least one           
        ///         Subscription in the Subscriber that will take each of the   
        ///         Publications in the Publisher.  We prefer the same type     
        ///         (first), and will fall back on the first base-derived pair  
        ///         we can find.  But we don't want the same Publication going  
        ///         to more than one of the Subscriptions in the Subscriber.    
        ///                                                                     
        ///         Additionally, any type (Publication or Subscriber) that     
        ///         inherits from lib::msg::NULL_BASE should be ignored.        
        ///                                                                     
        ///@note    If you do not want a subscriber to subscribe to all possible
        ///         items that the publisher publishes then use the Publication 
        ///         connect method directly.                                    
        ///                                                                     
        ///@throw   ConnectionError     Thrown if there are no common types     
        ///                             between the publisher and subscriber.   
        ///                             This indicates a coding (versus runtime)
        ///                             error.                                  
        //----------------------------------------------------------------------
        template<
            typename S0
          , typename S1
          , typename S2
          , typename S3
          , typename S4
          , typename S5
          , typename S6
          , typename S7
          , typename S8
        >
        uint32_t connect(
            lib::msg::Subscriber<S0, S1, S2, S3, S4, S5, S6, S7, S8>& s
          , bool allowConnectDerived = false
          , bool allowConnectConvertible = false
        )
        {

            uint32_t result(0);
            uint32_t pubUsed[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

            PUB_HELPER_ALL_PUB(PUB_HELPER_CONNECT_THEM, true, true);

            if (allowConnectDerived) {
                PUB_HELPER_ALL_PUB(PUB_HELPER_CONNECT_THEM, false, true);
            }

            if (allowConnectConvertible) {
                PUB_HELPER_ALL_PUB(PUB_HELPER_CONNECT_THEM, false, false);
            }

            if (result == 0) {
                std::string publisher("publisher");
                lib::work::NamedObject* name;
                lib::cast_dynamic(name, this);;
                if (name) publisher = name->name();
                throw ConnectionError(
                    lib::format(
                        "%s->connect(%s) error -- no common type"
                      , publisher.c_str()
                      , s.name().c_str()
                    )
                );
            }
            return result;
        }

        //----------------------------------------------------------------------
        ///@brief   Allow the user to call with a pointer instead of object.    
        //----------------------------------------------------------------------
        template<
            typename S0
          , typename S1
          , typename S2
          , typename S3
          , typename S4
          , typename S5
          , typename S6
          , typename S7
          , typename S8
        >
        uint32_t connect(
            lib::msg::Subscriber<S0, S1, S2, S3, S4, S5, S6, S7, S8>* s
          , bool allowConnectDerived = false
          , bool allowConnectConvertible = false
        )
        {
            return connect(*s, allowConnectDerived, allowConnectConvertible);
        }

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        template <typename SUB_TYPE>
        uint32_t connect(
            lib::ds::shared_ptr<SUB_TYPE>& sub
          , bool allowConnectDerived = false
          , bool allowConnectConvertible = false
        )
        {
            return connect(*sub, allowConnectDerived, allowConnectConvertible);
        }

        //----------------------------------------------------------------------
        ///@brief   Called when the publisher has completed its task(s).        
        ///         Informs subscribers that this Publisher is done.            
        //----------------------------------------------------------------------
        virtual void endPublication()
        {
            if (!m_EndPublicationCalled)
            {
                m_EndPublicationCalled = true;
                Publication<TYPE0>::endPublication();
                Publication<TYPE1>::endPublication();
                Publication<TYPE2>::endPublication();
                Publication<TYPE3>::endPublication();
                Publication<TYPE4>::endPublication();
                Publication<TYPE5>::endPublication();
                Publication<TYPE6>::endPublication();
                Publication<TYPE7>::endPublication();
                Publication<TYPE8>::endPublication();
            }
        }

        //----------------------------------------------------------------------
        ///@brief  Return the total number of subscriptions for this Publisher. 
        ///                                                                     
        ///         It is possible that a single subscriber may subscribe to    
        ///         more than one publication type.  This returns the number    
        ///         of subscriptions listening, not the number of subscribers.  
        ///         In either event, the value is to help determine if object   
        ///         is listening.                                               
        //----------------------------------------------------------------------
        size_t subscriptionCount() const
        {
            return Publication<TYPE0>::subscriptionCount()
                 + Publication<TYPE1>::subscriptionCount()
                 + Publication<TYPE2>::subscriptionCount()
                 + Publication<TYPE3>::subscriptionCount()
                 + Publication<TYPE4>::subscriptionCount()
                 + Publication<TYPE5>::subscriptionCount()
                 + Publication<TYPE6>::subscriptionCount()
                 + Publication<TYPE7>::subscriptionCount()
                 + Publication<TYPE8>::subscriptionCount()
            ;
            
        }

    protected:

    private:
        Publisher(const Publisher& that);
        Publisher& operator=(const Publisher& that);
        bool m_EndPublicationCalled;

};


//------------------------------------------------------------------------------
///@brief   Create a way to chain connect publishers and subscribers.           
///                                                                             
///@par Expected Usage                                                          
///     @code                                                                   
///         MyPublisherPtr  pub   (new MyPublisher);                            
///         MyPubSubPtr     pubsub(new MyPubSub);                               
///         MySubscriberPtr sub   (new MySubscriber);                           
///                                                                             
///                                                                             
///         lib::mp::work::ThreadableCollection threads;                        
///                                                                             
///         threads.push_back(pub);                                             
///         threads.push_back(pubsub);                                          
///         threads.push_back(sub);                                             
///                                                                             
///         //------------------------------------------------------------------
///         //  The following line illustrates operator>>.                      
///         //------------------------------------------------------------------
///         pub >> pubsub >> sub;                                               
///                                                                             
///         std::cout << "starting" << std::endl;                               
///                                                                             
///         threads.startAll();                                                 
///         threads.joinAll();                                                  
///                                                                             
///         std::cout << "ending" << std::endl;                                 
///     @endcode                                                                
///                                                                             
///@warning The operator>> only calls the default version of connect.           
///         If any of the other parameters need to be passed in then            
///         connect should be called directly.                                  
///                                                                             
///@tparam  PUBLISHER   A lib::msg::Publisher type.                             
///                                                                             
///@tparam  SUBSCRIBER  A lib::msg::Subscriber type with a type common to the   
///                     PUBLISHER.  If the SUBSCRIBER is also a Publisher then  
///                     the calls can be dasiy chained with p >> sp >> s;       
///                                                                             
///@param   p           A boost::shared_ptr to the Publisher to connect to.     
///                                                                             
///@param   s           A boost::shared_ptr to the Subscriber to connect.       
///                                                                             
///@return              s.  The reason for returning the subscriber parameter   
///                     is to allow the daisy chain effect if the subscriber    
///                     is also a publisher.                                    
//------------------------------------------------------------------------------
template <typename PUBLISHER, typename SUBSCRIBER>
lib::ds::shared_ptr<SUBSCRIBER> operator>> (
    lib::ds::shared_ptr<PUBLISHER> p
  , lib::ds::shared_ptr<SUBSCRIBER> s
)
{
    p->connect(s);
    return s;
}

}  // namespace msg //
}  // namespace lib //

#endif // #ifndef LIB_MSG_PUBLISHER_H //

