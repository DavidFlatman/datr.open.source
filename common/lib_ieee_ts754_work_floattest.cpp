//------------------------------------------------------------------------------
///@file lib_ieee_work_floattest.cpp                                            
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-01  DHF         File creation via Make Test Utility         
//------------------------------------------------------------------------------
#include "lib_ieee_ts754_work_floattest.h"
#include "dev_test_work_test.h"
#include "dev_test_work.h"
#include "lib_config_work_filepaths.h"
#include "lib_ieee_ts754_work_float.h"


namespace lib {
namespace ieee {
namespace ts754 {
namespace work {
namespace floattest {
namespace test {


//------------------------------------------------------------------------------
//  Register the test class for the dev_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::ieee::ts754::work::floattest::test::Test);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
Test::Test() : dev::test::work::Test("lib::ieee::ts754::work::float")
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    Test object to copy.                                        
//------------------------------------------------------------------------------
Test::Test(const Test& that) : dev::test::work::Test(that)
{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
Test::~Test()
{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
Test& Test::operator=(const Test& that)
{
    dev::test::work::Test::operator=(that);
    return *this;
} // Test::operator=(const Test& that) //                                       


//------------------------------------------------------------------------------
/// @brief Execute the various test for IEEE floats.                            
//------------------------------------------------------------------------------
void Test::runTest()
{
    //--------------------------------------------------------------------------
    //  Nice byte boundaries                                                    
    //--------------------------------------------------------------------------
    std::vector<uint8_t> data
    {
        //------------------------------------------------------------------
        // 3.0 + 1.0 / 4.0 + 1.0 / 16.0                                     
        //------------------------------------------------------------------
        0x40
      , 0x54
      , 0x00
      , 0x00

        //------------------------------------------------------------------
        //  Euler's constant                                                
        //------------------------------------------------------------------
      , 0x3F 
      , 0xE2
      , 0x78
      , 0x8C
      , 0xFC
      , 0x7D
      , 0x3E
      , 0x9A                                                                    

      //--------------------------------------------------------------------
      //    2.0625                                                          
      //--------------------------------------------------------------------
      , 0x40
      , 0x20

    };

    double d;

    TEST(lib::ieee::ts754::work::getFloat32(d, data, 0));
    TEST(d == 3.3125);

    TEST(lib::ieee::ts754::work::getFloat64(d, data, 32));
    TEST(d == 0.577215665);

    TEST(lib::ieee::ts754::work::getFloat16(d, data, 96));
    TEST(d == 2.0625);


    //--------------------------------------------------------------------------
    //  Everything shifted by 4-bits                                            
    //--------------------------------------------------------------------------
    std::vector<uint8_t> data2
    {
        //------------------------------------------------------------------
        // 3.0 + 1.0 / 4.0 + 1.0 / 16.0                                     
        //------------------------------------------------------------------
            0xF4
          , 0x05
          , 0x40
          , 0x00
          , 0x03
          , 0xFE
          , 0x27
          , 0x88
          , 0xCF
          , 0xC7
          , 0xD3
          , 0xE9
          , 0xA4
          , 0x02
          , 0x0F
    };

    TEST(lib::ieee::ts754::work::getFloat32(d, data2,  4));
    TEST(d == 3.3125);

    TEST(lib::ieee::ts754::work::getFloat64(d, data2,  36));
    TEST(d == 0.577215665);

    TEST(lib::ieee::ts754::work::getFloat16(d, data2, 100));
    TEST(d == 2.0625);

} 


} // namespace test                                                             
} // namespace floattest                                                        
} // namespace work                                                             
} // namespace ts754                                                            
} // namespace ieee                                                             
} // namespace lib                                                              


