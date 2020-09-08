//------------------------------------------------------------------------------
///@file lib_eu_work_test.cpp                                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-01  flatman     File creation via Make Test Utility         
//------------------------------------------------------------------------------
#include "lib_bits_work_test.h"
#include "dev_test_work_test.h"
#include "dev_test_work.h"
#include "lib_config_work_filepaths.h"
#include "lib_bits_work.h"

namespace lib {
namespace eu {
namespace work {
namespace test {

//------------------------------------------------------------------------------
//  Register the test class for the dev_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::eu::work::test::Test);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
Test::Test() : dev::test::work::Test("lib::bits_work::")
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
    Test::operator=(that);
    return *this;
} // Test::operator=(const Test& that) //


//------------------------------------------------------------------------------
/// @brief Execute the various test for bits.                                  
//------------------------------------------------------------------------------
void Test::runTest()
{
    isBitSet();
    getUnsigned();
    getSigned();
} 

//------------------------------------------------------------------------------
///@brief   Test the isBitSet functions.                                        
//------------------------------------------------------------------------------
void Test::isBitSet()
{
    //--------------------------------------------------------------------------
    //  Note:   Specifying the data this way and we avoid worrying about if the 
    //          machine running this is big endian or little endian.            
    //--------------------------------------------------------------------------
    std::vector<uint8_t> data
    {
        0x12
      , 0x34
      , 0x56
      , 0x78
      , 0x9A
      , 0xBC
      , 0xDE
      , 0xF0
    };

    TEST((lib::bits::work::isBitSet(data,  0) == false));     // 0x12
    TEST((lib::bits::work::isBitSet(data,  1) == false));
    TEST((lib::bits::work::isBitSet(data,  2) == false));
    TEST((lib::bits::work::isBitSet(data,  3) == true));

    TEST((lib::bits::work::isBitSet(data,  4) == false));    
    TEST((lib::bits::work::isBitSet(data,  5) == false));
    TEST((lib::bits::work::isBitSet(data,  6) == true));
    TEST((lib::bits::work::isBitSet(data,  7) == false));

    TEST((lib::bits::work::isBitSet(data,  8) == false));     // 0x34
    TEST((lib::bits::work::isBitSet(data,  9) == false));
    TEST((lib::bits::work::isBitSet(data, 10) == true));
    TEST((lib::bits::work::isBitSet(data, 11) == true));

    TEST((lib::bits::work::isBitSet(data, 12) == false));    
    TEST((lib::bits::work::isBitSet(data, 13) == true));
    TEST((lib::bits::work::isBitSet(data, 14) == false));
    TEST((lib::bits::work::isBitSet(data, 15) == false));

    TEST((lib::bits::work::isBitSet(data, 16) == false));     // 0x56
    TEST((lib::bits::work::isBitSet(data, 17) == true));
    TEST((lib::bits::work::isBitSet(data, 18) == false));
    TEST((lib::bits::work::isBitSet(data, 19) == true));

    TEST((lib::bits::work::isBitSet(data, 20) == false));    
    TEST((lib::bits::work::isBitSet(data, 21) == true));
    TEST((lib::bits::work::isBitSet(data, 22) == true));
    TEST((lib::bits::work::isBitSet(data, 23) == false));

    TEST((lib::bits::work::isBitSet(data, 24) == false));     // 0x78
    TEST((lib::bits::work::isBitSet(data, 25) == true));
    TEST((lib::bits::work::isBitSet(data, 26) == true));
    TEST((lib::bits::work::isBitSet(data, 27) == true));

    TEST((lib::bits::work::isBitSet(data, 28) == true));    
    TEST((lib::bits::work::isBitSet(data, 29) == false));
    TEST((lib::bits::work::isBitSet(data, 30) == false));
    TEST((lib::bits::work::isBitSet(data, 31) == false));

    TEST((lib::bits::work::isBitSet(data, 32) == true));     // 0x9A
    TEST((lib::bits::work::isBitSet(data, 33) == false));
    TEST((lib::bits::work::isBitSet(data, 34) == false));
    TEST((lib::bits::work::isBitSet(data, 35) == true));

    TEST((lib::bits::work::isBitSet(data, 36) == true));    
    TEST((lib::bits::work::isBitSet(data, 37) == false));
    TEST((lib::bits::work::isBitSet(data, 38) == true));
    TEST((lib::bits::work::isBitSet(data, 39) == false));

    TEST((lib::bits::work::isBitSet(data, 40) == true));     // 0xBC
    TEST((lib::bits::work::isBitSet(data, 41) == false));
    TEST((lib::bits::work::isBitSet(data, 42) == true));
    TEST((lib::bits::work::isBitSet(data, 43) == true));

    TEST((lib::bits::work::isBitSet(data, 44) == true));    
    TEST((lib::bits::work::isBitSet(data, 45) == true));
    TEST((lib::bits::work::isBitSet(data, 46) == false));
    TEST((lib::bits::work::isBitSet(data, 47) == false));

    TEST((lib::bits::work::isBitSet(data, 48) == true));     // 0xDE
    TEST((lib::bits::work::isBitSet(data, 49) == true));
    TEST((lib::bits::work::isBitSet(data, 50) == false));
    TEST((lib::bits::work::isBitSet(data, 51) == true));

    TEST((lib::bits::work::isBitSet(data, 52) == true));    
    TEST((lib::bits::work::isBitSet(data, 53) == true));
    TEST((lib::bits::work::isBitSet(data, 54) == true));
    TEST((lib::bits::work::isBitSet(data, 55) == false));

    TEST((lib::bits::work::isBitSet(data, 56) == true));     // 0xF0
    TEST((lib::bits::work::isBitSet(data, 57) == true));
    TEST((lib::bits::work::isBitSet(data, 58) == true));
    TEST((lib::bits::work::isBitSet(data, 59) == true));

    TEST((lib::bits::work::isBitSet(data, 60) == false));    
    TEST((lib::bits::work::isBitSet(data, 61) == false));
    TEST((lib::bits::work::isBitSet(data, 62) == false));
    TEST((lib::bits::work::isBitSet(data, 63) == false));


}

//------------------------------------------------------------------------------
///@brief   Test the get functions.                                             
//------------------------------------------------------------------------------
void Test::getUnsigned()
{                                                                               
    //--------------------------------------------------------------------------
    //  Note:   Specifying the data this way and we avoid worrying about if the 
    //          machine running this is big endian or little endian.            
    //--------------------------------------------------------------------------
    std::vector<uint8_t> data
    {
        0x12
      , 0x34
      , 0x56
      , 0x78
      , 0x9A
      , 0xBC
      , 0xDE
      , 0xF5
    };

    uint64_t u;

    //--------------------------------------------------------------------------
    //  Testing on byte boundary                                                
    //--------------------------------------------------------------------------
    TEST((lib::bits::work::getUnsigned(u, data,  0, 8), u == 0x12));
    TEST((lib::bits::work::getUnsigned(u, data,  8, 8), u == 0x34));
    TEST((lib::bits::work::getUnsigned(u, data, 16, 8), u == 0x56));
    TEST((lib::bits::work::getUnsigned(u, data, 24, 8), u == 0x78));
    TEST((lib::bits::work::getUnsigned(u, data, 32, 8), u == 0x9A));
    TEST((lib::bits::work::getUnsigned(u, data, 40, 8), u == 0xBC));
    TEST((lib::bits::work::getUnsigned(u, data, 48, 8), u == 0xDE));
    TEST((lib::bits::work::getUnsigned(u, data, 56, 8), u == 0xF5));

    TEST((lib::bits::work::getUnsigned(u, data,  0, 16), u == 0x1234));
    TEST((lib::bits::work::getUnsigned(u, data, 16, 16), u == 0x5678));
    TEST((lib::bits::work::getUnsigned(u, data, 32, 16), u == 0x9ABC));
    TEST((lib::bits::work::getUnsigned(u, data, 48, 16), u == 0xDEF5));

    TEST((lib::bits::work::getUnsigned(u, data, 0, 32), u == 0x12345678));
    TEST((lib::bits::work::getUnsigned(u, data,32, 32), u == 0x9ABCDEF5));

    TEST((lib::bits::work::getUnsigned(u, data, 0, 64), u == 0x123456789ABCDEF5L));


    //--------------------------------------------------------------------------
    //  Testing on nybble boundary                                              
    //--------------------------------------------------------------------------
    TEST((lib::bits::work::getUnsigned(u, data,  4, 8), u == 0x23));
    TEST((lib::bits::work::getUnsigned(u, data, 12, 8), u == 0x45));
    TEST((lib::bits::work::getUnsigned(u, data, 20, 8), u == 0x67));
    TEST((lib::bits::work::getUnsigned(u, data, 28, 8), u == 0x89));
    TEST((lib::bits::work::getUnsigned(u, data, 36, 8), u == 0xAB));
    TEST((lib::bits::work::getUnsigned(u, data, 44, 8), u == 0xCD));
    TEST((lib::bits::work::getUnsigned(u, data, 52, 8), u == 0xEF));
    TEST((lib::bits::work::getUnsigned(u, data, 60, 4), u == 0x5));


    //--------------------------------------------------------------------------
    //  Pull in unusually number of bits.                                       
    //--------------------------------------------------------------------------
    TEST((lib::bits::work::getUnsigned(u, data, 3, 3), u == 4));
    TEST((lib::bits::work::getUnsigned(u, data, 6, 3), u == 4));
    TEST((lib::bits::work::getUnsigned(u, data, 9, 3), u == 3));
    TEST((lib::bits::work::getUnsigned(u, data,12, 3), u == 2));
    TEST((lib::bits::work::getUnsigned(u, data,15, 3), u == 1));
    TEST((lib::bits::work::getUnsigned(u, data,18, 3), u == 2));
    TEST((lib::bits::work::getUnsigned(u, data,21, 3), u == 6));
    TEST((lib::bits::work::getUnsigned(u, data,24, 3), u == 3));

    TEST((lib::bits::work::getUnsigned(u, data, 61, 3), u == 5));


    TEST((lib::bits::work::getUnsigned(u, data , 0, 5), u == 0x02));
    TEST((lib::bits::work::getUnsigned(u, data , 4, 5), u == 0x04));
    TEST((lib::bits::work::getUnsigned(u, data , 8, 5), u == 0x06));
    TEST((lib::bits::work::getUnsigned(u, data, 60, 4), u == 0x05));

    TEST((lib::bits::work::getUnsigned(u, data, 1, 31), u == 0x12345678));

    //--------------------------------------------------------------------------
    //  Beyond first 32-bits ...                                                
    //--------------------------------------------------------------------------
    TEST((lib::bits::work::getUnsigned(u, data,  0, 64), u == 0x123456789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data,  4, 60), u == 0x23456789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data,  8, 56), u == 0x3456789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 12, 52), u == 0x456789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 16, 48), u == 0x56789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 20, 44), u == 0x6789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 24, 40), u == 0x789ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 28, 36), u == 0x89ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 32, 32), u == 0x9ABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 36, 28), u == 0xABCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 40, 24), u == 0xBCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 44, 20), u == 0xCDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 48, 16), u == 0xDEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 52, 12), u == 0xEF5));
    TEST((lib::bits::work::getUnsigned(u, data, 56,  8), u == 0xF5));
    TEST((lib::bits::work::getUnsigned(u, data, 60,  4), u == 0x5));

    TEST((lib::bits::work::getUnsigned(u, data,  0, 32), u == 0x12345678));
    TEST((lib::bits::work::getUnsigned(u, data,  4, 32), u == 0x23456789));
    TEST((lib::bits::work::getUnsigned(u, data,  8, 32), u == 0x3456789A));
    TEST((lib::bits::work::getUnsigned(u, data, 12, 32), u == 0x456789AB));
    TEST((lib::bits::work::getUnsigned(u, data, 16, 32), u == 0x56789ABC));
    TEST((lib::bits::work::getUnsigned(u, data, 20, 32), u == 0x6789ABCD));
    TEST((lib::bits::work::getUnsigned(u, data, 24, 32), u == 0x789ABCDE));
    TEST((lib::bits::work::getUnsigned(u, data, 28, 32), u == 0x89ABCDEF));

}


//------------------------------------------------------------------------------
///@brief   Get the signed vaule.                                               
//------------------------------------------------------------------------------
void Test::getSigned()
{
    //--------------------------------------------------------------------------
    //  The most significant bit is the sign bit (0 = positive; 1 = negative).  
    //  The value is held in 2's compliment (see lib_bits_work.cpp).            
    //--------------------------------------------------------------------------
    std::vector<uint8_t> data
    {
        0xFF                // the first  8 bits are negative 1
      , 0xFE                // the first 16 bits are negative 2
      , 0xFD                // this byte is -3
    };

    int64_t s;



    TEST((lib::bits::work::getSigned(s, data, 0,  2), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  3), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  4), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  5), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  6), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  7), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  8), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0,  9), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 10), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 11), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 12), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 13), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 14), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 15), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 0, 16), s == -2));

    TEST((lib::bits::work::getSigned(s, data, 1,  2), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  3), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  4), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  5), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  6), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  7), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  8), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1,  9), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1, 10), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1, 11), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1, 12), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1, 13), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1, 14), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 1, 15), s == -2));

    TEST((lib::bits::work::getSigned(s, data, 2,  2), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  3), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  4), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  5), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  6), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  7), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  8), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2,  9), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2, 10), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2, 11), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2, 12), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 2, 13), s == -1));

    TEST((lib::bits::work::getSigned(s, data, 3,  2), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  3), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  4), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  5), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  6), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  7), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  8), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3,  9), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3, 10), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3, 11), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3, 12), s == -1));
    TEST((lib::bits::work::getSigned(s, data, 3, 13), s == -2));

    TEST((lib::bits::work::getSigned(s, data, 16, 8), s == -3));

}


} // namespace test 
} // namespace work
} // namespace eu
} // namespace lib


