//------------------------------------------------------------------------------
///@file lib_msg_conversionlabtest.h                                            
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MSG_CONVERSIONLABTEST_H
#define LIB_MSG_CONVERSIONLABTEST_H

#include "dev_test_work_test.h"

namespace lib {
namespace msg {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: ConversionLabTest                                                
///                                                                             
///@par Purpose:                                                                
///         The ConversionLabTest class provides the regression test for the    
///         lib::msg::ConversionLab class.                                      
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-08-07  MTU     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class ConversionLabTest : public dev::test::work::Test {
    public:
        ConversionLabTest();
        ConversionLabTest(const ConversionLabTest& that);
        virtual ~ConversionLabTest();
        ConversionLabTest& operator=(const ConversionLabTest& that);

    protected:
        void runTest();

    private:

}; //  class ConversionLabTest : public dev::Test //

} // namespace test 
} // namespace msg
} // namespace lib

#endif // #ifndef LIB_MSG_CONVERSIONLABTEST_H //

