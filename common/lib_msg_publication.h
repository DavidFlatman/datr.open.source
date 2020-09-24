//------------------------------------------------------------------------------
///@file lib_msg_publication.h                                                  
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MSG_PUBLICATION_H_FILE_GUARD
#define LIB_MSG_PUBLICATION_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"
#include "lib_msg_publisherbase.h"
#include "lib_msg_subscription.h"
#include <boost/signals2.hpp>
#include <type_traits>
#include <stdint.h>

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///                                                                             
///@brief Base class to allow easy implementation of publisher idiom.           
///                                                                             
///@par Template: Publication                                                   
///                                                                             
///@par Purpose:                                                                
///         The msg::Publisher template is designed to create a consistent      
///         interface for msg::Subscriber's to interface with.  It is           
///         envisioned that a derived class (think file reader) will "emit" the 
///         publish signal when there is data for others to consume.            
///\n\n                                                                         
///         Additionally, when the publisher has no more information to publish 
///         (think end of file), the publisher must "emit" the endPublication   
///         signal to allow subscribers to know that the publisher is shutting  
///         down.                                                               
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@tparam  TYPE    The base type of the objects being dealt with (why worded   
///                 so poorly?).  Because we don't know how many objects are    
///                 subscribing to a publication, the actual object published   
///                 should be a boost::shared_ptr<const TYPE>.  Why?            
///                     -#  boost::shared_ptr   Since we don't know how many    
///                             subscribers there are, and we don't want to     
///                             track the life-cycle of the object, we'll let   
///                             the object track its own life-cycle.            
///                     -#  const TYPE  If @e const isn't specified then we     
///                             can have non-deterministic results if one       
///                             subscriber starts changing the value of the     
///                             objects we're publishing.  Other subscribers    
///                             may get the original version or may get the     
///                             modified version.  Let's save ourselves that    
///                             headache and only allow @e const objects.       
///                                                                             
///@version 2020-09-23  JRS     replace boost references with std references.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                                                                             
///@version 2019-02-20  DHF     Made PublicationSubscriberBase public to allow  
///                             access to outside code.                         
///                                                                             
///@version 2015-09-14  DHF     Changed the type returned by connect to uint32_t
///                                                                             
///@version 2015-08-30  DHF     Moved to its own file.                          
///                                                                             
///@version 2015-04-14  DHF     Removed the heart/heart-attack functionality.   
///                                                                             
///@version 2014-07-15  DHF     Added subscriptionCount()                       
///                                                                             
///@version 2013-10-29  DHF     Added lib::mp::Heart as base.                   
///                             Added beat() in publish.                        
///                                                                             
///@version 2013-08-28  DHF     Force all object published to be of type        
///                             boost::shared_ptr<const ...>.                   
///                                                                             
///@version 2009-08-11  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
class Publication 
    : private PublisherBase
    , virtual public PublisherSubscriberBase
{
    public:
        Publication() { setPublisher(); }
        virtual ~Publication() {
            //------------------------------------------------------------------
            //  Note:   You cannot call m_PublicationEndingSignal here.  The    
            //          connected objects may have already disappeared          
            //          (shouldn't boost handle that?!?).  Furthermore,         
            //          the actual work may have stopped long before the object 
            //          is destroyed.                                           
            //------------------------------------------------------------------
        }

        //----------------------------------------------------------------------
        ///@brief Publish the item passed.                                      
        ///@param item  The data to be published.                               
        ///@note    The main advantage of this over having the boost signal     
        ///         called directly is to provide type safety when a publisher  
        ///         can publish multiple data types.\n\n                        
        //----------------------------------------------------------------------
        void publish(lib::ds::shared_ptr<const TYPE> item) {
            m_PublishSignal(item);
        }

        //----------------------------------------------------------------------
        ///@brief Connect the given subscription to this publication.           
        //----------------------------------------------------------------------
        template <typename SUB_TYPE>
        uint32_t connect(Subscription<SUB_TYPE>& sub)
        {
            //------------------------------------------------------------------
            //  Note:   We only use same and derived once.  However, the "value"
            //          isn't really a bool but a trinary operator...           
            //------------------------------------------------------------------
            bool same = std::is_same<SUB_TYPE, TYPE>::value;
            bool derived = 
                std::is_base_of<SUB_TYPE, TYPE>::value
                && std::is_same<SUB_TYPE, TYPE>::value;
            uint32_t result(0);
            if (same || derived) {
                boost::signals2::connection connection1(
                    m_PublishSignal.connect(
                        std::bind(
                            &Subscription<SUB_TYPE>::inbox
                          , &sub
                          , std::placeholders::_1
                        )
                    )
                );

                boost::signals2::connection connection2;
                PublisherBase::connectPublicationEnding(
                    connection2
                  , sub
                  , m_PublicationEndingSignal
                );

                incrementPublicationCount(sub, connection1, connection2);
                result = 1;
            }
            return result;
        }

        //----------------------------------------------------------------------
        ///@brief   Specialized version of above template for when the          
        ///         Subscription type is the same as the Publication type.      
        ///                                                                     
        ///         This is needed to prevent confusion when someone does:      
        ///             myPublication<TYPE>::connect(mySubscriber);             
        ///         Without it, the compiler gets confused as to which SUB_TYPE 
        ///         to pass to the connection.                                  
        //----------------------------------------------------------------------
        uint32_t connect(Subscription<TYPE>& sub)
        {
            boost::signals2::connection connection1(
                m_PublishSignal.connect(
                    std::bind(
                        &Subscription<TYPE>::inbox
                      , &sub
                      , std::placeholders::_1
                    )
                )
            );

            boost::signals2::connection connection2;
            PublisherBase::connectPublicationEnding(
                connection2
              , sub
              , m_PublicationEndingSignal
            );

            incrementPublicationCount(sub, connection1, connection2);

            return 1;
        } 
        //----------------------------------------------------------------------
        ///@brief   Inform subscribers that we are done publishing; call after  
        ///         last item is published.                                     
        ///                                                                     
        ///@par Purpose:                                                        
        ///         The subscriber thread will hang waiting for another item    
        ///         from the publisher.  The endPublication wakes up the        
        ///         subscriber's wait state allowing it to determine if there   
        ///         are any other publisher it should wait for -- if not, the   
        ///         subscriber's thread terminates.                             
        //----------------------------------------------------------------------
        virtual void endPublication() {
            m_PublicationEndingSignal();
        }

        //----------------------------------------------------------------------
        ///@brief   Return the number of subscribers in for this publication.   
        //----------------------------------------------------------------------
        size_t subscriptionCount() const { return m_PublishSignal.num_slots(); }

    protected:

    private:
        //----------------------------------------------------------------------
        //  Let's see how long we can get by without implementing these.        
        //----------------------------------------------------------------------
        Publication(const Publication& that);
        Publication& operator=(const Publication& that);

        //----------------------------------------------------------------------
        ///@brief Signal to "emit" when there is data to be published.          
        //----------------------------------------------------------------------
        boost::signals2::signal<
            void (lib::ds::shared_ptr<const TYPE> )
        > m_PublishSignal;

        //----------------------------------------------------------------------
        ///@brief Signal to let our subscribers know that we're shutting down.  
        //----------------------------------------------------------------------
        boost::signals2::signal<void ()> m_PublicationEndingSignal;
};

} // namespace msg
} // namespace lib


#endif // #ifndef LIB_MSG_PUBLICATION_H_FILE_GUARD

