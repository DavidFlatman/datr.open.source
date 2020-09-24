//------------------------------------------------------------------------------
///@file lib_eu_work_valuetest.h                                                
//------------------------------------------------------------------------------
#ifndef LIB_EU_WORK_VALUETEST_H
#define LIB_EU_WORK_VALUETEST_H

#include "dev_test_work_test.h"

namespace lib {
namespace eu {
namespace work {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: ValueTest                                                        
///                                                                             
///@par Purpose:                                                                
///         The ValueTest class provides the regression test for the            
///         lib::eu::work::Value class.                                         
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-09-01  DHF         File creation via Make Test Utility         
///                                                                             
//------------------------------------------------------------------------------
class ValueTest : public dev::test::work::Test {
    public:
        ValueTest();
        ValueTest(const ValueTest& that);
        virtual ~ValueTest();
        ValueTest& operator=(const ValueTest& that);

    protected:
        void runTest() override;

    private:


}; //  class ValueTest : public dev::Test //

} // namespace test 
} // namespace work
} // namespace eu
} // namespace lib



#endif // #ifndef LIB_EU_WORK_VALUETEST_H //

