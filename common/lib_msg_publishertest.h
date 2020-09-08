//------------------------------------------------------------------------------
///@file lib_msg_publishertest.h                                                
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MSG_PUBLISHERTEST_H
#define LIB_MSG_PUBLISHERTEST_H

#include "dev_test_work_test.h"

namespace lib {
namespace msg {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: PublisherTest                                                    
///                                                                             
///@par Purpose:                                                                
///         The PublisherTest class provides the regression test for the        
///         lib::msg::Publisher class.                                          
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-08-30  MTU         File creation                               
///                                                                             
//------------------------------------------------------------------------------
class PublisherTest : public dev::test::work::Test {
    public:
        PublisherTest();
        PublisherTest(const PublisherTest& that);
        virtual ~PublisherTest();
        PublisherTest& operator=(const PublisherTest& that);

    protected:
        void runTest();

        void Publisher();
        void publish();
        void connect();
        void endPublication();
        void subscriptionCount();

    private:

}; //  class PublisherTest : public dev::Test //

} // namespace test 
} // namespace msg
} // namespace lib



#endif // #ifndef LIB_MSG_PUBLISHERTEST_H //

