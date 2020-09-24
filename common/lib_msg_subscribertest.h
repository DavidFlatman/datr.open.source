//------------------------------------------------------------------------------
///@file lib_msg_subscribertest.h                                               
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MSG_SUBSCRIBERTEST_H
#define LIB_MSG_SUBSCRIBERTEST_H

#include "dev_test_work_test.h"

namespace lib {
namespace msg {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: SubscriberTest                                                   
///                                                                             
///@par Purpose:                                                                
///         The SubscriberTest class provides the regression test for the       
///         lib::msg::Subscriber class.                                         
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-09-09  MTU         File creation                               
//------------------------------------------------------------------------------
class SubscriberTest : public dev::test::work::Test {
    public:
        //----------------------------------------------------------------------
        ///@par  Method:  SubscriberTest                                        
        ///@par  Purpose:                                                       
        ///          The SubscriberTest constructs the test object.             
        //----------------------------------------------------------------------
        SubscriberTest();
        SubscriberTest(const SubscriberTest& that);
        virtual ~SubscriberTest();
        SubscriberTest& operator=(const SubscriberTest& that);

    protected:
        //----------------------------------------------------------------------
        ///@par  Method:  runTest                                               
        ///@par  Purpose:                                                       
        ///          The runTest method executes the actual test.               
        //----------------------------------------------------------------------
        void runTest() override;

    private:

};  // class SubscriberTest //

} // namespace test //
} // namespace msg //
} // namespace lib //

#endif // #ifndef LIB_MSG_SUBSCRIBERTEST_H //
