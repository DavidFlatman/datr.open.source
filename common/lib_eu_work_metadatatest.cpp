//------------------------------------------------------------------------------
///@file lib_eu_work_metadatatest.cpp                                           
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-08-27  DHF     File creation.                                  
//------------------------------------------------------------------------------
#include "lib_eu_work_metadatatest.h"
#include "dev_test_work.h"
#include "dev_test_work_test.h"
#include "lib_config_work_filepaths.h"
#include "lib_eu_work_metadata.h"

namespace lib {
namespace eu {
namespace work {
namespace test {

//------------------------------------------------------------------------------
//  Register the test class for the dev_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::eu::work::test::MetaDataTest);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
MetaDataTest::MetaDataTest() : Test("lib::eu::work::MetaData")
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    MetaDataTest object to copy.                                
//------------------------------------------------------------------------------
MetaDataTest::MetaDataTest(const MetaDataTest& that) : Test(that)
{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
MetaDataTest::~MetaDataTest()

{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
MetaDataTest& MetaDataTest::operator=(const MetaDataTest& that)
{
    Test::operator=(that);
    return *this;
} // MetaDataTest::operator=(const MetaDataTest& that) //


//------------------------------------------------------------------------------
/// @brief Execute the various test for MetaData.                               
//------------------------------------------------------------------------------
void MetaDataTest::runTest()
{
    std::vector<MetaData>    values
    {
        {"parameter-1", "meters",  2, 16, 0, 3.14, 1.5, 5.6, 7.8, true, work::DataType::Fixed}
      , {"parameter-2", "feet",    3, 17, 1, 4.56, 2.6, 6.7, 8.9, false, work::DataType::Integer}
    };

    TEST(values[0].wordSequence() == 2);
    TEST(values[0].bitCount() == 16);
    TEST(values[0].lsb() == 0);
    TEST(values[0].scale() ==  3.14);
    TEST(values[0].msbValue() ==  1.5);
    TEST(values[0].units() == "meters");
    TEST(values[0].rangeLow() == 5.6);
    TEST(values[0].rangeHigh() == 7.8);
    TEST(values[0].name() == "parameter-1");
    TEST(values[0].isSigned() == true);
    TEST(values[0].dataType() == DataType::Fixed);


    TEST(values[1].wordSequence() == 3);
    TEST(values[1].bitCount() == 17);
    TEST(values[1].lsb() == 1);
    TEST(values[1].scale() ==  4.56);
    TEST(values[1].msbValue() ==  2.6);
    TEST(values[1].units() == "feet");
    TEST(values[1].rangeLow() == 6.7);
    TEST(values[1].rangeHigh() == 8.9);
    TEST(values[1].name() == "parameter-2");
    TEST(values[1].isSigned() == false);
    TEST(values[1].dataType() == DataType::Integer);


} // void MetaDataTest::runTest() //


} // namespace test 
} // namespace work
} // namespace eu
} // namespace lib


