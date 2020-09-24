//------------------------------------------------------------------------------
///@file lib_ieee_ts754_work_floattest.h                                        
//------------------------------------------------------------------------------
#ifndef LIB_IEEE_TS754_WORK_FLOATTEST_H_FILEGUARD
#define LIB_IEEE_TS754_WORK_FLOATTEST_H_FILEGUARD

#include "dev_test_work_test.h"

namespace lib {
namespace ieee {
namespace ts754 {
namespace work {
namespace floattest {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Test                                                             
///                                                                             
///@par Purpose:                                                                
///         Test the bit routines.                                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
//------------------------------------------------------------------------------
class Test : public dev::test::work::Test {
    public:
        Test();
        Test(const Test& that);
        virtual ~Test();
        Test& operator=(const Test& that);

    protected:
        void runTest() override;


    private:


}; //  class Test : public dev::Test //

} // namespace test 
} // namespace floattest 
} // namespace work
} // namespace ts754 
} // namespace ieee
} // namespace lib



#endif // #ifndef LIB_IEEE_TS754_WORK_FLOATTEST_H_FILEGUARD

