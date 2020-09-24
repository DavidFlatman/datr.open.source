//------------------------------------------------------------------------------
///@file lib_bits_work_test.h                                                   
//------------------------------------------------------------------------------
#ifndef LIB_BITS_WORK_TEST_H_FILE_GUARD
#define LIB_BITS_WORK_TEST_H_FILE_GUARD

#include "dev_test_work_test.h"

namespace lib {
namespace eu {
namespace work {
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

        void isBitSet();
        void getUnsigned();
        void getSigned();


    private:


}; //  class Test : public dev::Test //

} // namespace test 
} // namespace work
} // namespace eu
} // namespace lib



#endif // #ifndef LIB_BITS_WORK_TEST_H_FILE_GUARD

