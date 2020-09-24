//------------------------------------------------------------------------------
///@file lib_msg_publisherproxy.h                                               
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MSG_PUBLISHERPROXY_H_FILE_GUARD
#define LIB_MSG_PUBLISHERPROXY_H_FILE_GUARD

#include "lib_ds_null.h"
#include "lib_ds_shared_ptr.h"
#include "lib_mp_work_threadablecollection.h"

namespace lib {
namespace msg {

class PublisherProxyBase : public lib::mp::work::ThreadableCollection
{
};

//------------------------------------------------------------------------------
///                                                                             
///@par Class: PublisherProxy                                                   
///                                                                             
///@brief   Base class for pseudo-publisher objects that are proxies for other  
///         objects.                                                            
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///         See lib::irig106::ch8::clr::msg::FilePublisher for an example of    
///         how to use this class.                                              
///                                                                             
///@warning Each template type specified must have an override of the           
///         corresponding handleConnection method.  See below.                  
///                                                                             
///@par Quote:                                                                  
///         A MORE POSITIVE ATTITUDE WOULD ASSIST AT THIS POINT, THANK YOU      
///         SO VERY MUCH.                                                       
///                           -- Death, <i>Hogfather</i> by Terry Pratchett     
///                                                                             
///@version 2020-09-23  JRS     replace boost references with std references.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                             Changed typedef to using                        
///                                                                             
///@version 2017-07-13  DHF     File creation                                   
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
class PublisherProxy
    : public PublisherProxyBase
    , public lib::mp::work::Threadable
{
    public:
        PublisherProxy() = default;
        PublisherProxy(const PublisherProxy& p) = default;
        virtual ~PublisherProxy() = default;

        PublisherProxy& operator=(const PublisherProxy& p) = default;

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
            #define is_same_not_null(SUB_TYPE, PUB_TYPE)                    \
                (                                                           \
                    std::is_same<SUB_TYPE, PUB_TYPE>::value &&            \
                   !std::is_base_of<lib::ds::NULL_BASE, PUB_TYPE>::value  \
                )
            #define is_published_type(SUB_TYPE)                             \
                (                                                           \
                    is_same_not_null(SUB_TYPE, TYPE0)                       \
                 || is_same_not_null(SUB_TYPE, TYPE1)                       \
                 || is_same_not_null(SUB_TYPE, TYPE2)                       \
                 || is_same_not_null(SUB_TYPE, TYPE3)                       \
                 || is_same_not_null(SUB_TYPE, TYPE4)                       \
                 || is_same_not_null(SUB_TYPE, TYPE5)                       \
                 || is_same_not_null(SUB_TYPE, TYPE6)                       \
                 || is_same_not_null(SUB_TYPE, TYPE7)                       \
                 || is_same_not_null(SUB_TYPE, TYPE8)                       \
                )

            #define internal_connect(SUB_TYPE)                              \
                {                                                           \
                    lib::msg::Subscription<SUB_TYPE>* pSub;                 \
                    lib::cast_dynamic(pSub, &s);                            \
                    result += Internal<is_published_type(SUB_TYPE), SUB_TYPE>::connect(this, pSub); \
                }
            int result(0);

            internal_connect(S0);
            internal_connect(S1);
            internal_connect(S2);
            internal_connect(S3);
            internal_connect(S4);
            internal_connect(S5);
            internal_connect(S6);
            internal_connect(S7);
            internal_connect(S8);

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
        ///@brief   Allow the calling routine to call with a smart pointer      
        ///         versus dereferencing it.                                    
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


        void operator()() { }       // needed on because of lib::mp::work::Threadable

        //----------------------------------------------------------------------
        ///@warning Each template-type that you specify for your PublisherProxy 
        ///         mush have a corresponding handleConnection override.        
        //----------------------------------------------------------------------
        virtual int handleConnection(Subscription<TYPE0>* sub) = 0;
        virtual int handleConnection(Subscription<TYPE1>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE2>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE3>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE4>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE5>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE6>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE7>* sub) { return 0; } 
        virtual int handleConnection(Subscription<TYPE8>* sub) { return 0; }

    protected:
        using THIS_CLASS = PublisherProxy<
            TYPE0, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8
        >;

        template <bool, typename T> 
        struct Internal {
            static int connect(THIS_CLASS* t, lib::msg::Subscription<T>* sub) 
            { 
                return 0; 
            }
        };


        template<typename T>
        struct Internal<true, T>
        {
            static int connect(THIS_CLASS* t, lib::msg::Subscription<T>* sub)
            {
                return t->handleConnection(sub);
            }
        };


        template <typename SUB_TYPE, typename PUBLISHER>
        int makeConnection(
            lib::msg::Subscription<SUB_TYPE>* sub
          , PUBLISHER* pub
        )
        {
            return actualConnection(
                sub
              , dynamic_cast<lib::msg::Publication<SUB_TYPE>*>(pub)
            );
        }

        template <typename TYPE>
        void include(TYPE t)
        {
            if (t.get())
            {
                push_back(t);
            }
        }
        


    private:


}; // class PublisherProxy //


} // namespace msg
} // namespace lib


#endif // #ifndef LIB_MSG_PUBLISHERPROXY_H_FILE_GUARD

