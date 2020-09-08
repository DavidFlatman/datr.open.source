//------------------------------------------------------------------------------
///@file lib_msg_publishersubscriberbase.h                                      
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MSG_PUBLISHERSUBSCRIBERBASE_H_FILE_GUARD
#define LIB_MSG_PUBLISHERSUBSCRIBERBASE_H_FILE_GUARD

#include <boost/scoped_ptr.hpp>

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: PublisherSubscriberBase                                          
///                                                                             
///@brief   The ultimate base of all Publisher and Subscriber objects.  Probably
///         of no value outside of those templates.                             
///                                                                             
///@note    To be of any value, this class needs to be inherited with the       
///         virtual keyword.  See lib::msg::Publisher and lib::msg::Subscriber. 
///         The ultimate use is for classes that drive from both Subscriber and 
///         Publisher; allow the Subscriber operator()() to call endPublication 
///         (something Publisher objects need to do).                           
///\n\n                                                                         
///         This method is implemented by Publisher.                            
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-04-14  DHF     Made isPublisher and isSubscriber virtual.      
///                                                                             
///@version 2011-04-22  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class PublisherSubscriberBase
{
    public:
        PublisherSubscriberBase() : m_Publisher(false), m_Subscriber(false) { }
        virtual void endPublication() {}

        virtual bool isPublisher() const { return m_Publisher; }
        virtual bool isSubscriber() const { return m_Subscriber; }

        virtual size_t subscriptionCount() const { return 0; }

    protected:
        void setPublisher()  { m_Publisher  = true; }
        void setSubscriber() { m_Subscriber = true; }

    private:
        bool m_Publisher;
        bool m_Subscriber;

}; // class PublisherSubscriberBase //

} // namespace msg
} // namespace lib

#endif // #ifndef LIB_MSG_PUBLISHERSUBSCRIBERBASE_H_FILE_GUARD

