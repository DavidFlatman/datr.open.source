//------------------------------------------------------------------------------
///@file lib_eu_work_valuetest.cpp                                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-01  flatman     File creation via Make Test Utility         
//------------------------------------------------------------------------------
#include "lib_eu_work_valuetest.h"
#include "dev_test_work_test.h"
#include "dev_test_work.h"
#include "lib_config_work_filepaths.h"
#include "lib_eu_work_value.h"

#include "debug.h"

namespace lib {
namespace eu {
namespace work {
namespace test {

//------------------------------------------------------------------------------
//  Register the test class for the dev_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::eu::work::test::ValueTest);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
ValueTest::ValueTest() : dev::test::work::Test("lib::eu::work::Value")
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    ValueTest object to copy.                                   
//------------------------------------------------------------------------------
ValueTest::ValueTest(const ValueTest& that) : Test(that)
{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
ValueTest::~ValueTest()

{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
ValueTest& ValueTest::operator=(const ValueTest& that)
{
    Test::operator=(that);
    return *this;
} // ValueTest::operator=(const ValueTest& that) //


//------------------------------------------------------------------------------
/// @brief Execute the various test for Value.                                  
//------------------------------------------------------------------------------
void ValueTest::runTest()
{
    std::vector<MetaData> md {
        { "fixed-01-16-00",  "in",   1,  16, 0,   0,  0,  0,  0,  true, work::DataType::Fixed }
      , { "fixed-02-32-00",  "ft",   2,  32, 0,   0,  0,  0,  0,  true, work::DataType::Fixed }
      , { "int-03-16-00",    "yd",   3,  16, 0,   0,  0,  0,  0,  true, work::DataType::Integer }
      , { "int-04-32-00",    "mi",   4,  32, 0,   0,  0,  0,  0,  true, work::DataType::Integer }
      , { "uns-05-16-00",    "yd",   5,  16, 0,   0,  0,  0,  0,  false, work::DataType::Integer }
      , { "uns-06-32-00",    "mi",   6,  32, 0,   0,  0,  0,  0,  false, work::DataType::Integer }
    };

    std::vector<uint16_t> data
    {
        
    };

    //--------------------------------------------------------------------------
    //  Test the test                                                           
    //--------------------------------------------------------------------------
    TEST(md[0].name() == "fixed-01-16-00");
    TEST(md[1].units() == "ft");
    TEST(md[2].dataType() == work::DataType::Integer);
    TEST(md[3].bitCount() == 32);
    TEST(md[4].isSigned() == false);

    MetaDataPtr fixed_01_16_00(new MetaData(md[0]));
    MetaDataPtr fixed_02_32_00(new MetaData(md[1]));
    MetaDataPtr integ_03_16_00(new MetaData(md[2]));
    MetaDataPtr integ_04_32_00(new MetaData(md[3]));
    MetaDataPtr unsig_05_16_00(new MetaData(md[4]));
    MetaDataPtr unsig_06_32_00(new MetaData(md[5]));

    {
        Value v(fixed_01_16_00);
        TEST(v.metaData().get() == fixed_01_16_00.get());
        TEST(v.toDouble() == 0.0);
        TEST(v.toInteger() == 0.0);
        TEST(v.toUnsigned() == 0.0);
        TEST(v.toString(3,2) == "0.0 in");
        TEST(v.toString(Value::COMMA, 3, 2) == "0.0,in");
    }
} 



} // namespace test 
} // namespace work
} // namespace eu
} // namespace lib


