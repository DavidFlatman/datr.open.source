//------------------------------------------------------------------------------
///@file lib_work_vectorwithoffsetcollectiontest.cpp                            
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2015-11-23  DHF     Added tests for modifying the collection.       
///                                                                             
///@version 2015-10-26  DHF     Added initial test code.                        
///                                                                             
///@version 2015-10-25  MTU     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
#include "lib_work_vectorwithoffsetcollectiontest.h"
#include "dev_test_work.h"
#include "dev_test_work_test.h"

#include "lib_config_work_filepaths.h"
#include "lib_ds_shared_ptr.h"
#include "lib_pod_work_vectorwithoffsetfilereader.h"
#include "lib_work_vectorwithoffsetcollection.h"

#include <string>
#include <string.h>             // memset
#include <stdint.h>

namespace lib {
namespace work {
namespace test {

//------------------------------------------------------------------------------
//  Register the test class for the dev_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::work::test::VectorWithOffsetCollectionTest);

using MY_INT8_PTR = lib::ds::shared_ptr <lib::ds::VectorWithOffset<uint8_t> >;

using CONST_MY_INT8_PTR = lib::ds::shared_ptr<const lib::ds::VectorWithOffset<uint8_t> >;
    
using  MY_INT32_PTR = lib::ds::shared_ptr<lib::ds::VectorWithOffset<uint32_t> >;
    
using  CONST_MY_INT32_PTR = lib::ds::shared_ptr<const lib::ds::VectorWithOffset<uint32_t> >;
    

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
VectorWithOffsetCollectionTest::VectorWithOffsetCollectionTest() 
    : Test("lib::work::VectorWithOffsetCollection")
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    VectorWithOffsetCollectionTest object to copy.              
//------------------------------------------------------------------------------
VectorWithOffsetCollectionTest::VectorWithOffsetCollectionTest(const VectorWithOffsetCollectionTest& that) : Test(that)
{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
VectorWithOffsetCollectionTest::~VectorWithOffsetCollectionTest()

{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
VectorWithOffsetCollectionTest& VectorWithOffsetCollectionTest::operator=(const VectorWithOffsetCollectionTest& that)
{
    Test::operator=(that);
    return *this;
} 


//------------------------------------------------------------------------------
/// @brief Execute the various test for VectorWithOffsetCollection.             
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::runTest()
{
    //--------------------------------------------------------------------------
    //  Why not initialize m_FileName in the constructor.                       
    //      Race condition.                                                     
    //                                                                          
    //      The TEST_REGISTER macro effectively creates an object in the global 
    //      space (before main is executed).  The unitTestDataFilePath also     
    //      creates/initializes data in the global space.  If we try to use     
    //      uniTestDataFilePath as part of the global initialization there is   
    //      no guarentee that the data will be ready by the time it is needed.  
    //--------------------------------------------------------------------------
    m_FileName = lib::config::work::unitTestDataFilePath(
        "irig106/ch10/1553-data.ch10"
    );

    VectorWithOffsetCollection();
    operatorBrackets();
    minimum();
    size();
    free();
    push_back();
    memory_used();
    memcpy();
    empty();
    vectorFreedEvent();

}


//------------------------------------------------------------------------------
/// @brief Tests for the VectorWithOffsetCollection::VectorWithOffsetCollection 
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::VectorWithOffsetCollection()
{
    //--------------------------------------------------------------------------
    /// @test  The constructor will be adequately tested by the other methods.  
    //--------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
/// @brief Tests for the VectorWithOffsetCollection::operatorBrackets           
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::operatorBrackets()
{
    {
        lib::pod::work::VectorWithOffsetFileReader<uint8_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        lib::work::VectorWithOffsetCollection<uint8_t> v;

        //----------------------------------------------------------------------
        //  Earlier verssion of Microsoft Visual Studio wouldn't allow you to   
        //      const int max_size(32);                                         
        //      int values[max_size];                                           
        //----------------------------------------------------------------------
        #define P_COUNT 10
        MY_INT8_PTR p[P_COUNT];

        for (size_t i=0; i < P_COUNT; ++i) {
            lib::new_shared(p[i]);
            reader.read(*p[i]);
            v.push_back(p[i]);
        }

        uint64_t offset_overall(0);

        for (size_t i=0; i < P_COUNT; ++i) {
            for (size_t j=0; j < p[i]->size(); ++j) {
                uint8_t p_byte = (*(p[i]))[j];
                uint8_t v_byte = v[offset_overall];
                TEST(p_byte == v_byte);
                
                ++offset_overall;
            }
        }
    }

    {
        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        lib::work::VectorWithOffsetCollection<uint32_t> v;

        MY_INT32_PTR p[P_COUNT];

        for (size_t i=0; i < P_COUNT; ++i) {
            lib::new_shared(p[i]);
            reader.read(*p[i]);
            v.push_back(p[i]);
        }

        uint64_t offset_overall(0);

        for (size_t i=0; i < P_COUNT; ++i) {
            for (size_t j=0; j < p[i]->size(); ++j) {
                uint32_t p_byte = (*(p[i]))[j];
                uint32_t v_byte = v[offset_overall];
                TEST(p_byte == v_byte);
                
                ++offset_overall;
            }
        }
    }

    //--------------------------------------------------------------------------
    //  This time jump ahead in the file.                                       
    //--------------------------------------------------------------------------
    {
        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        uint64_t offset = 164141716;

        reader.seek(offset);

        lib::work::VectorWithOffsetCollection<uint32_t> v;

        MY_INT32_PTR p[P_COUNT];

        uint64_t previousSize(0);

        for (size_t i=0; i < P_COUNT; ++i) {
            lib::new_shared(p[i]);
            reader.read(*p[i]);

            //------------------------------------------------------------------
            //  Make sure that the vector offsets are correct since we did a    
            //  seek.                                                           
            //------------------------------------------------------------------
            TEST(p[i]->m_FileOffset == offset + previousSize);
            previousSize += p[i]->size() * sizeof(uint32_t);
            v.push_back(p[i]);
        }

        uint64_t offset_overall(offset /sizeof(uint32_t));

        for (size_t i=0; i < P_COUNT; ++i) {
            for (size_t j=0; j < p[i]->size(); ++j) {
                uint32_t p_byte = (*(p[i]))[j];
                uint32_t v_byte = v[offset_overall];
                TEST(p_byte == v_byte);
                
                ++offset_overall;
            }
        }
    }
    
    //--------------------------------------------------------------------------
    //  Test out modifing the data.                                             
    //--------------------------------------------------------------------------
    {
        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        lib::work::VectorWithOffsetCollection<uint32_t> v;

        MY_INT32_PTR p[P_COUNT];

        for (size_t i=0; i < P_COUNT; ++i) {
            lib::new_shared(p[i]);
            reader.read(*p[i]);

            v.push_back(p[i]);
        }

        TEST(v.size() == p[0]->size() * P_COUNT);

        for (uint64_t i = 0; i < v.size(); ++i) {
            uint32_t original = v[i];
            v[i] = 0xFFFFFFFF ^ v[i];

            TEST(v[i] != original);
            TEST(v[i] == (original ^ 0xFFFFFFFF));
        }

    }
    
    #undef P_COUNT
}


//------------------------------------------------------------------------------
/// @brief Tests for the VectorWithOffsetCollection::minimum                    
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::minimum()
{
    //--------------------------------------------------------------------------
    //  Test with uint8_t (byte at a time)                                      
    //--------------------------------------------------------------------------
    {
        lib::work::VectorWithOffsetCollection<uint8_t> v;
        TEST(v.minimum() == uint64_t(-1));

        lib::pod::work::VectorWithOffsetFileReader<uint8_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;


        MY_INT8_PTR p;
        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.minimum() == 0);

        uint64_t sizeToSecond = p->size();

        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.minimum() == 0);

        for (size_t i=0; i < sizeToSecond + 10; ++i) {
            v.free(i);
            TEST(v.minimum() == i + 1);
        }

    }

    //--------------------------------------------------------------------------
    //  Test with uint32_t                                                      
    //--------------------------------------------------------------------------
    {
        lib::work::VectorWithOffsetCollection<uint32_t> v;
        TEST(v.minimum() == uint64_t(-1));

        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;


        MY_INT32_PTR p;
        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.minimum() == 0);

        uint64_t sizeToSecond = p->size();

        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.minimum() == 0);

        for (size_t i=0; i < sizeToSecond + 10; ++i) {
            v.free(i);
            TEST(v.minimum() == i + 1);
        }

    }
}


//------------------------------------------------------------------------------
/// @brief Tests for the VectorWithOffsetCollection::size                       
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::size()
{
    //--------------------------------------------------------------------------
    //  Test with uint8_t (byte at a time)                                      
    //--------------------------------------------------------------------------
    {
        lib::work::VectorWithOffsetCollection<uint8_t> v;
        TEST(v.size() == 0);

        lib::pod::work::VectorWithOffsetFileReader<uint8_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;


        MY_INT8_PTR p;
        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.size() == p->size());

        uint64_t sizeToSecond = p->size();

        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.size() == 2 * p->size());

        for (size_t i=0; i < sizeToSecond + 10; ++i) {
            v.free(i);
            TEST(v.size() == 2 * p->size());
        }

    }

    //--------------------------------------------------------------------------
    //  Test with uint32_t                                                      
    //--------------------------------------------------------------------------
    {
        lib::work::VectorWithOffsetCollection<uint32_t> v;
        TEST(v.size() == 0);

        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;


        MY_INT32_PTR p;
        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.size() == p->size());

        uint64_t sizeToSecond = p->size();

        lib::new_shared(p);
        TEST(reader.read(*p));
        v.push_back(p);
        TEST(v.size() == 2 * p->size());

        for (size_t i=0; i < sizeToSecond + 10; ++i) {
            v.free(i);
            TEST(v.size() == 2 * p->size());
        }

    }
}

//------------------------------------------------------------------------------
/// @brief Tests for the VectorWithOffsetCollection::free                       
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::free()
{
    //--------------------------------------------------------------------------
    //  The free method is being tested in the minimum, size, and operator[]    
    //--------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
/// @brief Tests for the VectorWithOffsetCollection::push_back                  
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::push_back()
{
    //--------------------------------------------------------------------------
    //  The push_back method is being tested in the minimum, size, and          
    //  operator[]                                                              
    //--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
///@brief  Test for the VectorwithOffsetCollection::memory_used method.         
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::memory_used()
{
    {
        lib::pod::work::VectorWithOffsetFileReader<uint8_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        lib::work::VectorWithOffsetCollection<uint8_t> v;

        //----------------------------------------------------------------------
        //  Earlier verssion of Microsoft Visual Studio wouldn't allow you to   
        //      const int max_size(32);                                         
        //      int values[max_size];                                           
        //----------------------------------------------------------------------
        #define P_COUNT 3
        MY_INT8_PTR p[P_COUNT];

        TEST(v.memory_used() == 0);

        lib::new_shared(p[0]);
        reader.read(*p[0]);
        v.push_back(p[0]);
        TEST(v.memory_used() == (p[0]->size()) * sizeof(uint8_t));

        lib::new_shared(p[1]);
        reader.read(*p[1]);
        v.push_back(p[1]);
        TEST(
            v.memory_used() == (p[0]->size() + p[1]->size()) * sizeof(uint8_t)
        );

        lib::new_shared(p[2]);
        reader.read(*p[2]);
        v.push_back(p[2]);
        TEST(
            v.memory_used() == (p[0]->size() + p[1]->size() + p[2]->size()) * sizeof(uint8_t)
        );

        //----------------------------------------------------------------------
        //  10 is less than p[0]->size so no memory is actually released.       
        //----------------------------------------------------------------------
        TEST((
            v.free(10)
          , v.memory_used() == (p[0]->size() + p[1]->size() + p[2]->size()) 
                * sizeof(uint8_t)
        ));

        TEST((
            v.free(p[0]->size())
          , v.memory_used() == (p[1]->size() + p[2]->size()) * sizeof(uint8_t)
        ));

        TEST((
            v.free(p[0]->size() + 1)
          , v.memory_used() == (p[1]->size() + p[2]->size()) * sizeof(uint8_t)
        ));

        TEST((
            v.free(p[0]->size() + p[1]->size())
          , v.memory_used() == (p[2]->size()) * sizeof(uint8_t)
        ));


    }

    {
        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        lib::work::VectorWithOffsetCollection<uint32_t> v;

        //----------------------------------------------------------------------
        //  Earlier verssion of Microsoft Visual Studio wouldn't allow you to   
        //      const int max_size(32);                                         
        //      int values[max_size];                                           
        //----------------------------------------------------------------------
        MY_INT32_PTR p[P_COUNT];

        TEST(v.memory_used() == 0);

        lib::new_shared(p[0]);
        reader.read(*p[0]);
        v.push_back(p[0]);
        TEST(v.memory_used() == (p[0]->size()) * sizeof(uint32_t));

        lib::new_shared(p[1]);
        reader.read(*p[1]);
        v.push_back(p[1]);
        TEST(
            v.memory_used() == (p[0]->size() + p[1]->size()) * sizeof(uint32_t)
        );

        lib::new_shared(p[2]);
        reader.read(*p[2]);
        v.push_back(p[2]);
        TEST(
            v.memory_used() == (p[0]->size() + p[1]->size() + p[2]->size()) * sizeof(uint32_t)
        );

        //----------------------------------------------------------------------
        //  10 is less than p[0]->size so no memory is actually released.       
        //----------------------------------------------------------------------
        TEST((
            v.free(10)
          , v.memory_used() == (p[0]->size() + p[1]->size() + p[2]->size()) 
                * sizeof(uint32_t)
        ));

        TEST((
            v.free(p[0]->size())
          , v.memory_used() == (p[1]->size() + p[2]->size()) * sizeof(uint32_t)
        ));

        TEST((
            v.free(p[0]->size() + 1)
          , v.memory_used() == (p[1]->size() + p[2]->size()) * sizeof(uint32_t)
        ));

        TEST((
            v.free(p[0]->size() + p[1]->size())
          , v.memory_used() == (p[2]->size()) * sizeof(uint32_t)
        ));


    }
    #undef P_COUNT
} // void VectorWithOffsetCollectionTest::memory_used()


//------------------------------------------------------------------------------
///@brief   Test the VectorWithOffsetCollection::memcpy method.                 
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::memcpy()
{
    struct Header
    {
        uint16_t    m_SyncPattern;
        uint16_t     m_ChannelID;
        uint32_t    m_PacketLength;
        uint32_t    m_DataLength; 
        uint8_t     m_DataTypeVersion;
        uint8_t     m_SequenceNumber;
        uint8_t     m_PacketFlags;

        uint8_t        m_DataType;
        unsigned char  m_RTCChar[6];
        uint16_t       m_HeaderChecksum;
    };

    //--------------------------------------------------------------------------
    //  Test with byte access level ... the offset and the index are the same.  
    //--------------------------------------------------------------------------
    {
        lib::pod::work::VectorWithOffsetFileReader<uint8_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        MY_INT8_PTR    p;
        Header          header;
        lib::work::VectorWithOffsetCollection<uint8_t> v;
        memset(&header, 0, sizeof(header));

        //----------------------------------------------------------------------
        //  We haven't added any memory, so we shouldn't be able to get 24 bytes
        //  out.                                                                
        //----------------------------------------------------------------------
        TEST(v.memcpy(&header, 0, 24) == false);

        //----------------------------------------------------------------------
        //  Add 12 (of the 24) bytes that we want ... still can't get 24 out.   
        //----------------------------------------------------------------------
        lib::new_shared(p);
        reader.read(*p, 12);
        v.push_back(p);
        TEST(v.size() == 12);
        TEST(v.memcpy(&header, 0, 24) == false);

        //----------------------------------------------------------------------
        //  Add 12 more.  We should now get 24 bytes out.                       
        //----------------------------------------------------------------------
        lib::new_shared(p);
        reader.read(*p, 12);
        v.push_back(p);
        TEST(v.size() == 24);
        TEST(v.memcpy(&header, 0, 24) == true);
        TEST(header.m_SyncPattern == 0xEB25);
        TEST(header.m_ChannelID == 0x0000);
        TEST(header.m_PacketLength == 0x000029A8);
        TEST(header.m_DataLength == 0x00002990);
        TEST(header.m_DataTypeVersion == 0x01);
        TEST(header.m_SequenceNumber == 0x00);
        TEST(header.m_PacketFlags == 0x00);
        TEST(header.m_DataType == 0x01);
        TEST(header.m_RTCChar[0] == 0x2d);
        TEST(header.m_RTCChar[1] == 0x3e);
        TEST(header.m_RTCChar[2] == 0x87);
        TEST(header.m_RTCChar[3] == 0x4c);
        TEST(header.m_RTCChar[4] == 0x00);
        TEST(header.m_RTCChar[5] == 0x00);
        TEST(header.m_HeaderChecksum == 0xCA12);

        //----------------------------------------------------------------------
        //  Get to the point where the next packet's header should be held.     
        //                                                                      
        //  Note:  A dump of the unit test file should show:                    
        //              0   setup record                                        
        //          10664   time packet  (byte offset 10664; index 2666)        
        //          10700   1553B packet (byte offset 10700; index 2675)        
        //          12780   1553B packet (byte offset 12780; index 3195)        
        //----------------------------------------------------------------------
        while (v.size() < 12780 + sizeof(header)) {
            lib::new_shared(p);
            reader.read(*p, 12);
            v.push_back(p);
        }

        Header header2;

        memset(&header,  0, sizeof(header));
        memset(&header2, 0, sizeof(header2));

        TEST(v.memcpy(&header, 10664, 24));
        TEST(header.m_SyncPattern == 0xEB25);
        TEST(header.m_ChannelID == 0x0280);
        TEST(header.m_PacketLength == 0x00000024);
        TEST(header.m_DataLength == 0x0000000a);
        TEST(header.m_DataTypeVersion == 0x01);
        TEST(header.m_SequenceNumber == 0x00);
        TEST(header.m_PacketFlags == 0x00);
        TEST(header.m_DataType == 0x11);
        TEST(header.m_RTCChar[0] == 0x9f);
        TEST(header.m_RTCChar[1] == 0x70);
        TEST(header.m_RTCChar[2] == 0xD9);
        TEST(header.m_RTCChar[3] == 0x4c);
        TEST(header.m_RTCChar[4] == 0x00);
        TEST(header.m_RTCChar[5] == 0x00);
        TEST(header.m_HeaderChecksum == 0xBC4C);

        uint64_t used = v.memory_used();
        v.free(2665);
        TEST(v.memory_used() < used);

        TEST(v.memcpy(&header2, 10664, 24));
        TEST(header2.m_SyncPattern == 0xEB25);
        TEST(header2.m_ChannelID == 0x0280);
        TEST(header2.m_PacketLength == 0x00000024);
        TEST(header2.m_DataLength == 0x0000000a);
        TEST(header2.m_DataTypeVersion == 0x01);
        TEST(header2.m_SequenceNumber == 0x00);
        TEST(header2.m_PacketFlags == 0x00);
        TEST(header2.m_DataType == 0x11);
        TEST(header2.m_RTCChar[0] == 0x9f);
        TEST(header2.m_RTCChar[1] == 0x70);
        TEST(header2.m_RTCChar[2] == 0xD9);
        TEST(header2.m_RTCChar[3] == 0x4c);
        TEST(header2.m_RTCChar[4] == 0x00);
        TEST(header2.m_RTCChar[5] == 0x00);
        TEST(header2.m_HeaderChecksum == 0xBC4C);

        TEST(header.m_SyncPattern == header2.m_SyncPattern);
        TEST(header.m_ChannelID == header2.m_ChannelID);
        TEST(header.m_PacketLength == header2.m_PacketLength);
        TEST(header.m_DataLength == header2.m_DataLength);
        TEST(header.m_DataTypeVersion == header2.m_DataTypeVersion);
        TEST(header.m_SequenceNumber == header2.m_SequenceNumber);
        TEST(header.m_PacketFlags == header2.m_PacketFlags);
        TEST(header.m_DataType == header2.m_DataType);
        TEST(header.m_RTCChar[0] == header2.m_RTCChar[0]);
        TEST(header.m_RTCChar[1] == header2.m_RTCChar[1]);
        TEST(header.m_RTCChar[2] == header2.m_RTCChar[2]);
        TEST(header.m_RTCChar[3] == header2.m_RTCChar[3]);
        TEST(header.m_RTCChar[4] == header2.m_RTCChar[4]);
        TEST(header.m_RTCChar[5] == header2.m_RTCChar[5]);
        TEST(header.m_HeaderChecksum == header2.m_HeaderChecksum);

        memset(&header,  0, sizeof(header));
        memset(&header2, 0, sizeof(header2));

        TEST(v.memcpy(&header, 10700, 24));
        TEST(header.m_SyncPattern == 0xEB25);
        TEST(header.m_ChannelID == 0x0680);
        TEST(header.m_PacketLength == 0x00000820);
        TEST(header.m_DataLength == 0x00000806);
        TEST(header.m_DataTypeVersion == 0x01);
        TEST(header.m_SequenceNumber == 0x00);
        TEST(header.m_PacketFlags == 0x00);
        TEST(header.m_DataType == 0x19);
        TEST(header.m_RTCChar[0] == 0x48);
        TEST(header.m_RTCChar[1] == 0xdB);
        TEST(header.m_RTCChar[2] == 0xd4);
        TEST(header.m_RTCChar[3] == 0x4C);
        TEST(header.m_RTCChar[4] == 0x00);
        TEST(header.m_RTCChar[5] == 0x00);

        used = v.memory_used();
        v.free(10668);
        TEST(v.memory_used() < used);

        TEST(v.memcpy(&header2, 10700, 24));
        TEST(header2.m_SyncPattern == 0xEB25);
        TEST(header2.m_ChannelID == 0x0680);
        TEST(header2.m_PacketLength == 0x00000820);
        TEST(header2.m_DataLength == 0x00000806);
        TEST(header2.m_DataTypeVersion == 0x01);
        TEST(header2.m_SequenceNumber == 0x00);
        TEST(header2.m_PacketFlags == 0x00);
        TEST(header2.m_DataType == 0x19);
        TEST(header2.m_RTCChar[0] == 0x48);
        TEST(header2.m_RTCChar[1] == 0xdB);
        TEST(header2.m_RTCChar[2] == 0xd4);
        TEST(header2.m_RTCChar[3] == 0x4C);
        TEST(header2.m_RTCChar[4] == 0x00);
        TEST(header2.m_RTCChar[5] == 0x00);
        TEST(header2.m_HeaderChecksum == 0x42E8);

        //----------------------------------------------------------------------
        //  The start memory has already been freed.                            
        //----------------------------------------------------------------------
        TEST(v.memcpy(&header2, 10664, 24) == false);

    }


    //--------------------------------------------------------------------------
    //  Test with the byte offset and the index referencing two different values
    //--------------------------------------------------------------------------
    {
        lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
        TEST(reader.is_open());
        if (!reader.is_open()) return;

        MY_INT32_PTR    p;
        Header          header;
        lib::work::VectorWithOffsetCollection<uint32_t> v;
        memset(&header, 0, sizeof(header));

        //----------------------------------------------------------------------
        //  We haven't added any memory, so we shouldn't be able to get 24 bytes
        //  out.                                                                
        //----------------------------------------------------------------------
        TEST(v.memcpy(&header, 0, 24) == false);

        //----------------------------------------------------------------------
        //  Add 12 (of the 24) bytes that we want ... still can't get 24 out.   
        //----------------------------------------------------------------------
        lib::new_shared(p);
        reader.read(*p, 12);
        v.push_back(p);
        TEST(v.size() == 3);
        TEST(v.memcpy(&header, 0, 24) == false);

        //----------------------------------------------------------------------
        //  Add 12 more.  We should now get 24 bytes out.                       
        //----------------------------------------------------------------------
        lib::new_shared(p);
        reader.read(*p, 12);
        v.push_back(p);
        TEST(v.size() == 6);
        TEST(v.memcpy(&header, 0, 24) == true);
        TEST(header.m_SyncPattern == 0xEB25);
        TEST(header.m_ChannelID == 0x0000);
        TEST(header.m_PacketLength == 0x000029A8);
        TEST(header.m_DataLength == 0x00002990);
        TEST(header.m_DataTypeVersion == 0x01);
        TEST(header.m_SequenceNumber == 0x00);
        TEST(header.m_PacketFlags == 0x00);
        TEST(header.m_DataType == 0x01);
        TEST(header.m_RTCChar[0] == 0x2d);
        TEST(header.m_RTCChar[1] == 0x3e);
        TEST(header.m_RTCChar[2] == 0x87);
        TEST(header.m_RTCChar[3] == 0x4c);
        TEST(header.m_RTCChar[4] == 0x00);
        TEST(header.m_RTCChar[5] == 0x00);
        TEST(header.m_HeaderChecksum == 0xCA12);

        //----------------------------------------------------------------------
        //  Get to the point where the next packet's header should be held.     
        //                                                                      
        //  Note:  A dump of the unit test file should show:                    
        //              0   setup record                                        
        //          10664   time packet  (byte offset 10664; index 2666)        
        //          10700   1553B packet (byte offset 10700; index 2675)        
        //          12780   1553B packet (byte offset 12780; index 3195)        
        //----------------------------------------------------------------------
        while (v.size() < 3195 + sizeof(header) / sizeof(uint32_t)) {
            lib::new_shared(p);
            reader.read(*p, 12);
            v.push_back(p);
        }

        Header header2;

        memset(&header,  0, sizeof(header));
        memset(&header2, 0, sizeof(header2));

        TEST(v.memcpy(&header, 2666, 24));
        TEST(header.m_SyncPattern == 0xEB25);
        TEST(header.m_ChannelID == 0x0280);
        TEST(header.m_PacketLength == 0x00000024);
        TEST(header.m_DataLength == 0x0000000a);
        TEST(header.m_DataTypeVersion == 0x01);
        TEST(header.m_SequenceNumber == 0x00);
        TEST(header.m_PacketFlags == 0x00);
        TEST(header.m_DataType == 0x11);
        TEST(header.m_RTCChar[0] == 0x9f);
        TEST(header.m_RTCChar[1] == 0x70);
        TEST(header.m_RTCChar[2] == 0xD9);
        TEST(header.m_RTCChar[3] == 0x4c);
        TEST(header.m_RTCChar[4] == 0x00);
        TEST(header.m_RTCChar[5] == 0x00);
        TEST(header.m_HeaderChecksum == 0xBC4C);

        uint64_t used = v.memory_used();
        v.free(2665);
        TEST(v.memory_used() < used);

        TEST(v.memcpy(&header2, 2666, 24));
        TEST(header2.m_SyncPattern == 0xEB25);
        TEST(header2.m_ChannelID == 0x0280);
        TEST(header2.m_PacketLength == 0x00000024);
        TEST(header2.m_DataLength == 0x0000000a);
        TEST(header2.m_DataTypeVersion == 0x01);
        TEST(header2.m_SequenceNumber == 0x00);
        TEST(header2.m_PacketFlags == 0x00);
        TEST(header2.m_DataType == 0x11);
        TEST(header2.m_RTCChar[0] == 0x9f);
        TEST(header2.m_RTCChar[1] == 0x70);
        TEST(header2.m_RTCChar[2] == 0xD9);
        TEST(header2.m_RTCChar[3] == 0x4c);
        TEST(header2.m_RTCChar[4] == 0x00);
        TEST(header2.m_RTCChar[5] == 0x00);
        TEST(header2.m_HeaderChecksum == 0xBC4C);

        TEST(header.m_SyncPattern == header2.m_SyncPattern);
        TEST(header.m_ChannelID == header2.m_ChannelID);
        TEST(header.m_PacketLength == header2.m_PacketLength);
        TEST(header.m_DataLength == header2.m_DataLength);
        TEST(header.m_DataTypeVersion == header2.m_DataTypeVersion);
        TEST(header.m_SequenceNumber == header2.m_SequenceNumber);
        TEST(header.m_PacketFlags == header2.m_PacketFlags);
        TEST(header.m_DataType == header2.m_DataType);
        TEST(header.m_RTCChar[0] == header2.m_RTCChar[0]);
        TEST(header.m_RTCChar[1] == header2.m_RTCChar[1]);
        TEST(header.m_RTCChar[2] == header2.m_RTCChar[2]);
        TEST(header.m_RTCChar[3] == header2.m_RTCChar[3]);
        TEST(header.m_RTCChar[4] == header2.m_RTCChar[4]);
        TEST(header.m_RTCChar[5] == header2.m_RTCChar[5]);
        TEST(header.m_HeaderChecksum == header2.m_HeaderChecksum);

        memset(&header,  0, sizeof(header));
        memset(&header2, 0, sizeof(header2));

        TEST(v.memcpy(&header, 2675, 24));
        TEST(header.m_SyncPattern == 0xEB25);
        TEST(header.m_ChannelID == 0x0680);
        TEST(header.m_PacketLength == 0x00000820);
        TEST(header.m_DataLength == 0x00000806);
        TEST(header.m_DataTypeVersion == 0x01);
        TEST(header.m_SequenceNumber == 0x00);
        TEST(header.m_PacketFlags == 0x00);
        TEST(header.m_DataType == 0x19);
        TEST(header.m_RTCChar[0] == 0x48);
        TEST(header.m_RTCChar[1] == 0xdB);
        TEST(header.m_RTCChar[2] == 0xd4);
        TEST(header.m_RTCChar[3] == 0x4C);
        TEST(header.m_RTCChar[4] == 0x00);
        TEST(header.m_RTCChar[5] == 0x00);

        used = v.memory_used();
        v.free(2670);
        TEST(v.memory_used() < used);

        TEST(v.memcpy(&header2, 2675, 24));
        TEST(header2.m_SyncPattern == 0xEB25);
        TEST(header2.m_ChannelID == 0x0680);
        TEST(header2.m_PacketLength == 0x00000820);
        TEST(header2.m_DataLength == 0x00000806);
        TEST(header2.m_DataTypeVersion == 0x01);
        TEST(header2.m_SequenceNumber == 0x00);
        TEST(header2.m_PacketFlags == 0x00);
        TEST(header2.m_DataType == 0x19);
        TEST(header2.m_RTCChar[0] == 0x48);
        TEST(header2.m_RTCChar[1] == 0xdB);
        TEST(header2.m_RTCChar[2] == 0xd4);
        TEST(header2.m_RTCChar[3] == 0x4C);
        TEST(header2.m_RTCChar[4] == 0x00);
        TEST(header2.m_RTCChar[5] == 0x00);
        TEST(header2.m_HeaderChecksum == 0x42E8);

    }
                                                                                

} // void VectorWithOffsetCollectionTest::memcpy()


//------------------------------------------------------------------------------
///@brief   Test the VectorWithOffsetCollection::empty method.                  
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::empty()
{
    lib::work::VectorWithOffsetCollection<uint32_t> v;
    TEST(v.empty() == true);

    lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader(m_FileName);
    TEST(reader.is_open());
    if (!reader.is_open()) return;

    MY_INT32_PTR    p;
    lib::new_shared(p);
    TEST(reader.read(*p));
    TEST((v.push_back(p),       v.empty() == false));
    TEST((v.free(v.size() / 2), v.empty() == false));
    TEST((v.free(v.size()),     v.empty() == true));

    

} // void VectorWithOffsetCollectionTest::empty()


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class TestVectorFreedEvent
    : public lib::work::VectorWithOffsetCollection<uint16_t>
{
    public:
        TestVectorFreedEvent() : m_Count(0) { }

        lib::ds::shared_ptr< const lib::ds::VectorWithOffset<uint16_t> > m_Last;
        uint64_t m_Count;

    protected:
        void vectorFreedEvent(ConstPointer p)
        {
            m_Last = p;
            ++m_Count;
        }
};


//------------------------------------------------------------------------------
///@brief   Test out the vectorFreedEvent method.                               
//------------------------------------------------------------------------------
void VectorWithOffsetCollectionTest::vectorFreedEvent()
{
    //--------------------------------------------------------------------------
    //  Only get 10 items per read.                                             
    //--------------------------------------------------------------------------
    lib::pod::work::VectorWithOffsetFileReader<uint16_t> reader(m_FileName, 10);
    TEST(reader.is_open());
    if (!reader.is_open()) return;

    //--------------------------------------------------------------------------
    //  Simple test to make sure that the correct things are being sent to the  
    //  event.                                                                  
    //--------------------------------------------------------------------------
    {
        TestVectorFreedEvent tvfe;
        lib::ds::shared_ptr< lib::ds::VectorWithOffset<uint16_t> > p;

        for (int i = 0; i < 20; ++i)
        {
            reader.get(p);
            tvfe.push_back(p);
        }

        TEST(tvfe.m_Last.get() == NULL);

        TEST((tvfe.free(0), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(1), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(2), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(3), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(4), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(5), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(6), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(7), tvfe.m_Last.get() == NULL));
        TEST((tvfe.free(8), tvfe.m_Last.get() == NULL));
        TEST(tvfe.m_Count == 0);

        TEST((tvfe.free( 9), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(10), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(11), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(12), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(13), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(14), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(15), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(16), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(17), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST((tvfe.free(18), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 0));
        TEST(tvfe.m_Count == 1);


        TEST((tvfe.free(19), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(20), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(21), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(22), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(23), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(24), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(25), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(26), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST((tvfe.free(27), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 20));
        TEST(tvfe.m_Count == 2);


        TEST((tvfe.free(29), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(30), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(31), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(32), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(33), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(34), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(35), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(36), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(37), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST((tvfe.free(38), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 40));
        TEST(tvfe.m_Count == 3);

        TEST((tvfe.free(39), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(40), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(41), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(42), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(43), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(44), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(45), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(46), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(47), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST((tvfe.free(48), tvfe.m_Last.get() != NULL && tvfe.m_Last->m_FileOffset == 60));
        TEST(tvfe.m_Count == 4);
    }

    //--------------------------------------------------------------------------
    //  Modify the vector and see if the modifications are being passed to the  
    //  event.                                                                  
    //--------------------------------------------------------------------------
    {
        reader.rewind();

        TestVectorFreedEvent tvfe;
        lib::ds::shared_ptr< lib::ds::VectorWithOffset<uint16_t> > p;

        std::vector<
            lib::ds::shared_ptr< lib::ds::VectorWithOffset<uint16_t> >
        > vector;

        for (int i = 0; i < 20; ++i)
        {
            reader.get(p);
            tvfe.push_back(p);
            vector.push_back(p);
        }

        TEST((
            tvfe[0] = 2015
          , tvfe[1] = 10
          , tvfe[2] = 29
          , tvfe[3] = 1776
          , tvfe[4] = 7
          , tvfe[5] = 4
          , tvfe.free(9)
          , tvfe.m_Last != NULL && (*(tvfe.m_Last))[0] == 2015
        ));
        if (tvfe.m_Last == NULL) return;

        TEST((*(tvfe.m_Last))[1] == 10);
        TEST((*(tvfe.m_Last))[2] == 29);
        TEST((*(tvfe.m_Last))[3] == 1776);
        TEST((*(tvfe.m_Last))[4] == 7);
        TEST((*(tvfe.m_Last))[5] == 4);

        //----------------------------------------------------------------------
        //  Since we pased in non-const pointers, their data should also have   
        //  changed.                                                            
        //----------------------------------------------------------------------
        TEST( vector[0]->at(0) == 2015);
        TEST( vector[0]->at(1) == 10);
        TEST( vector[0]->at(2) == 29);
        TEST( vector[0]->at(3) == 1776);
        TEST( vector[0]->at(4) == 7);
        TEST( vector[0]->at(5) == 4);
    }

    //--------------------------------------------------------------------------
    //  Do the same test, but this time put const pointers ... the external     
    //  const pointers should not change.                                       
    //--------------------------------------------------------------------------
    {
        reader.rewind();

        TestVectorFreedEvent tvfe;
        lib::ds::shared_ptr< lib::ds::VectorWithOffset<uint16_t> > p;

        std::vector<
            lib::ds::shared_ptr< lib::ds::VectorWithOffset<uint16_t> >
        > vector;

        for (int i = 0; i < 20; ++i)
        {
            reader.get(p);
            tvfe.push_back(lib::ds::shared_ptr<const lib::ds::VectorWithOffset<uint16_t> >(p));
            vector.push_back(p);
        }

        TEST((
            tvfe[0] = 2015
          , tvfe[1] = 10
          , tvfe[2] = 29
          , tvfe[3] = 1776
          , tvfe[4] = 7
          , tvfe[5] = 4
          , tvfe.free(9)
          , tvfe.m_Last != NULL && (*(tvfe.m_Last))[0] == 2015
        ));
        TEST(tvfe.m_Count == 1);
        TEST(tvfe.m_Last.get() != NULL);

        if (tvfe.m_Last == NULL) return;

        TEST((*(tvfe.m_Last))[1] == 10);
        TEST((*(tvfe.m_Last))[2] == 29);
        TEST((*(tvfe.m_Last))[3] == 1776);
        TEST((*(tvfe.m_Last))[4] == 7);
        TEST((*(tvfe.m_Last))[5] == 4);

        //----------------------------------------------------------------------
        //  Since we pased in const pointers, their data should not have        
        //  changed.                                                            
        //----------------------------------------------------------------------
        TEST( vector[0]->at(0) != 2015);
        TEST( vector[0]->at(1) != 10);
        TEST( vector[0]->at(2) != 29);
        TEST( vector[0]->at(3) != 1776);
        TEST( vector[0]->at(4) != 7);
        TEST( vector[0]->at(5) != 4);

    }
} // void VectorWithOffsetCollectionTest::vectorFreedEvent()


} // namespace test 
} // namespace work
} // namespace lib


