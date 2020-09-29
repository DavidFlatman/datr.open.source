//------------------------------------------------------------------------------
///@file lib_irig106_ch10_work_packet.cpp                                       
///                                                                             
///@par Classification:  UNCLASSIFED, OPEN SOURCE                               
///                                                                             
///@brief Holds the definition of the lib::irig106::ch10::work::Packet class.   
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2019-12-11  KCG     Added step in setRTC that updates the checksum  
///                                                                             
///@version 2019-08-20  MNT     Sorted methods to better match order in .h.     
///                                                                             
///@version 2018-11-20  KJS     Channel specific data word added as a parameter 
///                             to the Packet constructor                       
///                                                                             
///@version 2018-11-13  KJS     Added constructor that takes in data type name, 
///                             data, data size, rtc, and checksum              
///                                                                             
///@version 2015-10-22  DHF     The checksum needs to include the filler.       
///                                                                             
///@version 2015-07-07  YBS     Used InstrumentationTime                        
///                                                                             
///@version 2014-06-16  DMN     Added setRTC implementation                     
///                                                                             
///@version 2014-02-24  DHF     Added toString set of methods.                  
///                                                                             
///@version 2014-01-29  DHF     Fixed bug in cheapClone where not all of the    
///                             pointers were being copied.                     
///                                                                             
///@version 2013-11-13  DHF     Added cheapClone (again).                       
///                                                                             
///@version 2012-05-04  AGB     Added the timeEncoding method                   
///                                                                             
///@version 2012-04-25  AGB     Added the rtcToAbsolute method                  
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-09-03  DHF     File header inserted.                           
///                                                                             
//------------------------------------------------------------------------------
#include <string.h>         // memcpy 

#include "lib_irig106_ch10_work_packet.h"

#include "lib_cast.h"
#include "lib_irig106_ch10_ds.h"
#include "lib_irig106_checksum.h"
#include "lib_string.h"

#include "debug.h"

namespace lib {
namespace irig106 {
namespace ch10 {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Packet::Packet()
  : m_Header(NULL)
  , m_SecondaryHeader(NULL)
  , m_AfterHeader(NULL)
  , m_Body(NULL)
  , m_Checksum8(NULL)
  , m_Checksum16(NULL)
  , m_Checksum32(NULL)
  , m_FileOffset(0)
  , m_FilePacketNumber(0)
  , m_RTC(0)
  , m_OriginalRTC(0)
  , m_PacketChecksumChecked(false)
  , m_PacketChecksumValid(false)
{ 
}

//------------------------------------------------------------------------------
///@brief   Used to instantiate Packet objects that contain a Chapter 10        
///         header and data                                                     
///                                                                             
///@note    Packet Header (second parameter) - 24 bytes                         
///                                                                             
///         Packet Header:                                                      
///             packet_sync_pattern   - 2 bytes (assumed value: 0xEB25)         
///             channel_id            - 2 bytes                                 
///             packet_length         - 4 bytes                                 
///             data_length           - 4 bytes                                 
///             data_type_version     - 1 byte                                  
///             sequence_number       - 1 byte                                  
///             packet_flag           - 1 byte                                  
///             data_type             - 1 byte                                  
///             relative_time_counter - 6 bytes                                 
///             header_checksum       - 2 bytes (can be calculated)             
///                                                                             
///@param   dtn     Data type name                                              
///                                                                             
///@param   data    The data of the packet                                      
///                                                                             
///@param   data_size   The size of the data                                    
///                                                                             
///@param   rtc         Relative time counter                                   
///                                                                             
///@param   checksum    Ensures that the size of packet contents is correct     
///                                                                             
///@param   channel_specific_data_word Channel Specific Data Word               
//------------------------------------------------------------------------------
Packet::Packet(
      lib::irig106::ds::DATA_TYPE_NAME dtn
    , void*                            data
    , size_t                           data_size
    , uint64_t                         rtc      
    , uint32_t*                        checksum 
    , uint32_t                         channel_specific_data_word
)
{

    //--------------------------------------------------------------------------
    //  Initialize packet header                                                
    //--------------------------------------------------------------------------
    ds::PacketHeader            header;

    header.m_SyncPattern     = ds::SYNC_PATTERN;
    header.m_ChannelID       = 0;
    
    //--------------------------------------------------------------------------
    //  + 4             channel-specific-data-word                              
    //  + 3 / 4 * 4     Even 32 bit boundary                                    
    //--------------------------------------------------------------------------
    header.m_PacketLength    = (sizeof(header) + data_size + 4 + 3) / 4 * 4;
    header.m_DataLength      = data_size + 4;

    header.m_SequenceNumber  = 0;

    header.m_PacketFlags     = 0;
    header.m_DataType        = dtn;

    //--------------------------------------------------------------------------
    // Construct packet header                                                  
    //--------------------------------------------------------------------------
    construct(header);

    m_Body[0] = channel_specific_data_word;
    memcpy(&m_Body[1], data, data_size);

    //--------------------------------------------------------------------------
    //  Initialize packet values                                                
    //--------------------------------------------------------------------------
    m_RTC                   = rtc;
    m_Checksum32            = checksum;

}

//------------------------------------------------------------------------------
///@warning Shallow Copy / Underlying Structures are Pointers                   
///         The @e const qualifier on the parameter is hideously misleading.    
///         It is const as far as the compiler is concerned, but the reality    
///         is that it is not const.                                            
///                                                                             
///         Consider,                                                           
///         @code                                                               
///             void myFunction(Packet& packet)                                 
///             {                                                               
///                 Packet copied(packet);                                      
///                                                                             
///                 //----------------------------------------------------------
///                 //  The following line changes the packet parameter, even   
///                 //  though we aren't touching it directly.                  
///                 //----------------------------------------------------------
///                 memset(copied.m_Header, 0, 100);                            
///             }                                                               
///         @endcode                                                            
///                                                                             
///         The reality is that we're copying pointers, but not the underlying  
///         data.                                                               
//------------------------------------------------------------------------------
#if 0
Packet::Packet(const Packet& packet)
  : m_Header(packet.m_Header)
  , m_SecondaryHeader(packet.m_SecondaryHeader)
  , m_Body(packet.m_Body)
  , m_Checksum8(packet.m_Checksum8)
  , m_Checksum16(packet.m_Checksum16)
  , m_Checksum32(packet.m_Checksum32)
  , m_FileOffset(packet.m_FileOffset)
  , m_FilePacketNumber(packet.m_FilePacketNumber)
  , m_RTC(packet.m_RTC)
  , m_OriginalRTC(packet.m_OriginalRTC)
  , m_Memory(packet.m_Memory)
  , m_PacketChecksumChecked(packet.m_PacketChecksumChecked)
  , m_PacketChecksumValid(packet.m_PacketChecksumValid)
{
    if (!correctDataType()) {
        handleWrongTypeError();
    }
}
#endif

//------------------------------------------------------------------------------
///@brief   Allocate memory and assign pointers.                                
///                                                                             
///         The constructor is really why we caved in and made Packet a         
///         class.  The constructor examines the given header and               
///         determines the size of of the overall packet;  it sets the          
///         internal (and exposed) pointers; and copies the header data         
///         into the newly allocated memory.                                    
///                                                                             
///@param header    A chapter 10 PacketHeader that informs us how big           
///                 the packet is.                                              
//------------------------------------------------------------------------------
Packet::Packet(const ds::PacketHeader& header) 
  : m_Header(NULL)
  , m_SecondaryHeader(NULL)
  , m_Body(NULL)
  , m_Checksum8(NULL)
  , m_Checksum16(NULL)
  , m_Checksum32(NULL)
  , m_FilePacketNumber(0)
  , m_RTC(0)
  , m_OriginalRTC(0)
  , m_PacketChecksumChecked(false)
  , m_PacketChecksumValid(false)
{
    construct(header);

    if (!correctDataType()) {
        handleWrongTypeError();
    }
}

#if 0
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Packet& Packet::operator=(const Packet& that)
{
    m_Memory = that.m_Memory;
    m_Header = that.m_Header;
    m_SecondaryHeader = that.m_SecondaryHeader;
    m_Body = that.m_Body;
    m_Checksum8 = that.m_Checksum8;
    m_Checksum16 = that.m_Checksum16;
    m_Checksum32 = that.m_Checksum32;
    m_FilePacketNumber = that.m_FilePacketNumber;
    m_RTC = that.m_RTC;
    m_OriginalRTC = that.m_OriginalRTC;
    m_PacketChecksumChecked = that.m_PacketChecksumChecked;
    m_PacketChecksumValid = that.m_PacketChecksumValid;

    if (!correctDataType()) {
        handleWrongTypeError();
    }

    return *this;
}
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Packet::~Packet() 
{ 
    //--------------------------------------------------------------------------
    //  The only allocated memory is m_Memory ... everything else points to     
    //  somewhere in that memory.  Since m_Memory is a smart pointer, we don't  
    //  need to worry about freeing it.                                         
    //--------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Packet& Packet::operator=(const ds::PacketHeader& that)
{
    construct(that);

    if (!correctDataType()) {
        handleWrongTypeError();
    }

    return *this;
}

//------------------------------------------------------------------------------
///@brief   Create a new packet and perform a deep copy of this packet.         
///@return  A pointer to the newly created packet.                              
///@warning The calling routine is responsible for "owning" the pointer.        
//------------------------------------------------------------------------------
PacketPtr Packet::copy() const
{
    PacketPtr result(new Packet(*m_Header));
    uint8_t* result_memory = &(*result->m_Memory)[0];
    uint8_t* memory        = &(*m_Memory)[0];

    memcpy(result_memory, memory, m_Header->m_PacketLength);

//    memcpy(result->m_Memory.get(), m_Memory.get(), m_Header->m_PacketLength);
    
    result->m_AbsoluteTime          = m_AbsoluteTime;
    result->m_FileOffset            = m_FileOffset;
    result->m_FilePacketNumber      = m_FilePacketNumber;
    result->m_RTC                   = m_RTC;
    result->m_OriginalRTC           = m_OriginalRTC;
    result->m_PacketChecksumChecked = m_PacketChecksumChecked;
    result->m_PacketChecksumValid   = m_PacketChecksumValid;

    return result;
} 

#if 0
PacketPtr Packet::dataToPacket(Type type) const
{
    lib::irig106::ch10::work::PacketPtr packet;
    return packet; // change this..
}
#endif 

//------------------------------------------------------------------------------
///@brief   Finish constructing the packet based on the given header.           
///                                                                             
///@par 10.6.1.1.d                                                              
///         Data Length. These 4 bytes contain a value representing the valid   
///         data length within the packet. This value shall be represented in   
///         bytes. Valid data length includes channel- specific data, IPDHs,    
///         intra-packet time stamp(s) (IPTS), and data but does not include    
///         packet trailer filler and data checksum.                            
///                                                                             
//------------------------------------------------------------------------------
void Packet::construct(const ds::PacketHeader& header)
{
    //--------------------------------------------------------------------------
    // Compile time check that the structures are the correct size.             
    //--------------------------------------------------------------------------
    static_assert(sizeof(ds::PacketHeader) == 24, "unexpected size");
    static_assert(sizeof(ds::PacketSecondaryHeader) == 12, "unexpected size");

    m_Memory = lib::ds::shared_ptr<std::vector<uint8_t>>(
        new std::vector<uint8_t>(header.m_PacketLength, 0)
    );

//    m_Memory = boost::shared_array<uint8_t>(new uint8_t[header.m_PacketLength]);

    //--------------------------------------------------------------------------
    //  If the memory allocation succeeded...                                   
    //--------------------------------------------------------------------------
    if (m_Memory.get() != NULL) {
//      memset(&m_Memory[header.m_PacketLength-4], 0, 4);

        //----------------------------------------------------------------------
        //  Get a handle to the raw memory that m_Memory points to.  Everything 
        //  else in the "packet" utlimately is based on m_Memory.               
        //----------------------------------------------------------------------
        uint8_t* raw = &(*m_Memory)[0];

        m_AfterHeader = &raw[sizeof(ds::PacketHeader)];

        const int HeaderSize1 = sizeof(ds::PacketHeader);
        const int HeaderSize2 = sizeof(ds::PacketSecondaryHeader);

        m_Header = (ds::PacketHeader*) raw;
        memcpy(raw, &header, sizeof(header));

        //----------------------------------------------------------------------
        //  If the secondary header exists, that shoves everything else down    
        //  the line.                                                           
        //----------------------------------------------------------------------
        if (m_Header->m_SecondaryHeaderPresent) {
            m_SecondaryHeader = (ds::PacketSecondaryHeader*) &raw[HeaderSize1];
            m_Body = (uint32_t*) &raw[HeaderSize1 + HeaderSize2];
        } else {
            m_SecondaryHeader = NULL;
            m_Body = (uint32_t*) &raw[HeaderSize1];
        }

        //----------------------------------------------------------------------
        //  Determine which checksum type is enforce and setup it's pointer.    
        //----------------------------------------------------------------------
        switch(header.m_ChecksumType) {
            case 0:
                break;
            case 1:
                m_Checksum8 = (uint8_t*) &raw[header.m_PacketLength-1];
                break;
            case 2:
                m_Checksum16 = (uint16_t*) &raw[header.m_PacketLength-2];
                break;
            case 3:
                m_Checksum32 = (uint32_t*) &raw[header.m_PacketLength-4];
                break;
        }

        m_RTC = 0;
        memcpy(&m_RTC, m_Header->m_RTCChar, sizeof(m_Header->m_RTCChar));
        m_OriginalRTC = m_RTC;

    } // if (m_Memory != NULL)  //

} // Packet::Packet(const PacketHeader& header) //

//------------------------------------------------------------------------------
///@brief  Return true if the packet checksum is valid.                         
///                                                                             
///     The isPacketChecksumValid checks to see if the checksum has already     
///     been verified.  If it has, it returns the previous results (faster).    
///     If it has not been verified, then it verifies the checksum and          
///     returns true if the packet checksum is correct.                         
//------------------------------------------------------------------------------
bool Packet::isPacketChecksumValid() const
{
    if (!m_PacketChecksumChecked)  {
        verifyPacketChecksum();
    }

    return m_PacketChecksumValid;
}

//------------------------------------------------------------------------------
///@brief  Verify the checksum and return true if it is valid.                  
///                                                                             
///     The verifyPacketChecksum always verifies the checksum before            
///     returning the result.                                                   
///                                                                             
///@note  From 10.6.1.1.d of IRIG106-13                                         
///         Data Length. These 4 bytes contain a value representing the valid   
///         data length within the packet. This value shall be represented in   
///         bytes. Valid data length includes channel- specific data, IPDHs,    
///         intra-packet time stamp(s) (IPTS), and data but does not include    
///         packet trailer filler and data checksum.                            
///                                                                             
///@note  From 10.6.1.4 of IRIG106-9                                            
///         The inclusion of the data checksum is optional as well and is       
///         indicated by the Packet Flags setting. When included, the packet    
///         trailer contains either an 8-bit, 16-bit, or 32-bit Data Checksum.  
///         Depending on the Packet Flags option selected, the Data Checksum is 
///         the arithmetic sum of all of the bytes (8 bits), words (16 bits),   
///         or long words (32 bits) in the packet excluding the 24 bytes of     
///         Packet Header, Packet Secondary Header (if enabled) and the Data    
///         Checksum. Stated another way, the Data Checksum includes everything 
///         in the packet body plus all added filler.                           
///                                                                             
///@par When to Use                                                             
///     @li The verifyPacketChecksum should be used instead of                  
///         isPacketChecksumValid if the contents of the packet has changed     
///         since the last verification.                                        
///                                                                             
///     @li The isPacketChecksumValid is faster if the checksum has already     
///         been verified.  (Though if the contents have changed, it will       
///         return erroneous results.)                                          
///                                                                             
///     @li If it is the first time that either isPacketChecksumValid or        
///         verifyPacketChecksum is called, performance differences are         
///         roughly equivalent.                                                 
///                                                                             
//------------------------------------------------------------------------------
bool Packet::verifyPacketChecksum() const
{
    if (m_Memory.get() == NULL) {
        //----------------------------------------------------------------------
        //  I had a professor that would not have liked the fact that we checked
        //  for NULL first ... but Dr. O was a bit daft.                        
        //----------------------------------------------------------------------
        m_PacketChecksumChecked = false;
        m_PacketChecksumValid = false;

    } else {
        //----------------------------------------------------------------------
        //  "size" is used to determine how many items to send to the checksum. 
        //  This value needs to include all of the data and the filler bytes.   
        //                                                                      
        //  Note that the filler bytes have a value of 0.                       
        //                                                                      
        //  If we have an 8-bit checksum, the size can be the same as           
        //  m_DataLength (which ignores the 0-value filler words).              
        //                                                                      
        //  If we have a 16-bit checksum and an even number of data-bytes,      
        //  we can ignore the 0-value filler words and just use half the        
        //  m_DataLength.  However, if we m_DataLength is odd then we have to   
        //  use half+1 of m_DataLength to ensure that we include the last of    
        //  the data (the last 16-bit value will be 8-bits of data in the       
        //  lower byte and 8-bits of filler in the upper byte).                 
        //                                                                      
        //  In order to cleanly handle either an even or odd number of bytes    
        //  with a 16-bit checksum, we take half of m_DataLength + 1.  If       
        //  m_DataLength is even, the extra 0.5 will be discarded.  If          
        //  m_DataLength is odd, the extra 0.5 will cause the 0.5 to round      
        //  up.                                                                 
        //                                                                      
        //  Consider the following byte layout of the data with a data length   
        //  of five (5):                                                        
        //      0x01            // first byte of data                           
        //      0x02            // second byte of data                          
        //      0x03            // third byte of data                           
        //      0x04            // fourth byte of data                          
        //      0x05            // fifth byte of data                           
        //      0x00            // filler                                       
        //      0x00            // filler                                       
        //      0x00            // filler                                       
        //                                                                      
        //  If we look at the values as 16-bit Little Endian values to sum,     
        //  we have:                                                            
        //      0x0201                                                          
        //    + 0x0403                                                          
        //    + 0x0005                                                          
        //    + 0x0000      // this has no impact                               
        //                                                                      
        //  The above concept for 16-bit checksum applies for 32-bit.  We need  
        //  to round up on the data length.                                     
        //----------------------------------------------------------------------
        uint32_t size(0);
        const uint32_t& length(m_Header->m_DataLength); // just for convenience

        switch (m_Header->m_ChecksumType) {
            //------------------------------------------------------------------
            //  No packet checksum ... consider it good.                        
            //------------------------------------------------------------------
            case lib::irig106::ch10::ds::PACKET_CHECKSUM_NONE:
                m_PacketChecksumValid = true;
                break;

            //------------------------------------------------------------------
            //  uint8_t checksum                                                
            //------------------------------------------------------------------
            case lib::irig106::ch10::ds::PACKET_CHECKSUM_8:
                //--------------------------------------------------------------
                //  Divide by the size of the checksum and round up.            
                //--------------------------------------------------------------
                size = (length + sizeof(uint8_t) - 1) / sizeof(uint8_t);
                m_PacketChecksumValid = (
                    *m_Checksum8 == lib::irig106::checksum(
                        (uint8_t*) m_Body
                      , size
                    )
                );
                break;

            //------------------------------------------------------------------
            //  uint16_t checksum                                               
            //------------------------------------------------------------------
            case lib::irig106::ch10::ds::PACKET_CHECKSUM_16:
                //--------------------------------------------------------------
                //  Divide by the size of the checksum and round up.            
                //--------------------------------------------------------------
                size = (length + sizeof(uint16_t) - 1) / sizeof(uint16_t);

                m_PacketChecksumValid = (
                    *m_Checksum16 == lib::irig106::checksum(
                        (uint16_t*) m_Body
                      , size
                    )
                );
                break;

            //------------------------------------------------------------------
            //  uint32_t checksum                                               
            //------------------------------------------------------------------
            case lib::irig106::ch10::ds::PACKET_CHECKSUM_32:
                //--------------------------------------------------------------
                //  Divide by the size of the checksum and round up.            
                //--------------------------------------------------------------
                size = (length + sizeof(uint32_t) - 1) / sizeof(uint32_t);

                m_PacketChecksumValid = (
                    *m_Checksum32 == lib::irig106::checksum(
                        (uint32_t*) m_Body
                      , size
                    )
                );
                break;

        } // switch (m_Header->m_ChecksumType) //

        m_PacketChecksumChecked = true;

    } // if (m_Memory == NULL) //

    return m_PacketChecksumValid;

} // bool Packet::verifyPacketChecksum() const //

//------------------------------------------------------------------------------
///@brief Centralize the handling of bad cast job.                              
//------------------------------------------------------------------------------
void Packet::handleWrongTypeError()
{
    #if 0       // eliminated copy-contructor
    Packet empty;
    *this = empty;
    #endif

    throw InvalidPacketConversion();
}

//------------------------------------------------------------------------------
///@brief   Return true if the packet has the correct data type.  It is expected
///         that this will be overloaded by derived types.                      
//------------------------------------------------------------------------------
bool Packet::correctDataType() const 
{ 
    return true;
}

//------------------------------------------------------------------------------
///@brief 
//------------------------------------------------------------------------------
bool Packet::isHeaderChecksumValid() const
{
    uint32_t checksum32(0);
    uint16_t* header((uint16_t*) m_Header);

    for (size_t i=0; i < sizeof(ds::PacketHeader)/2 - 1; ++i) {
        checksum32 += header[i];
    }

    return m_Header->m_HeaderChecksum == (checksum32 & 0xFFFF);
}

//------------------------------------------------------------------------------
///@brief   Fix the checksum (presumably after someone has modified it).        
///@param   correctly   If true, change the checksum to the expected value else 
///                     make it one off (to preserve checksum correctness).     
//------------------------------------------------------------------------------
void Packet::fixChecksum(bool correctly)
{
    if (m_Memory.get() == NULL) return;

    switch(m_Header->m_ChecksumType)
    {
        case ds::PACKET_CHECKSUM_NONE:
            break;
        case ds::PACKET_CHECKSUM_8:
            *m_Checksum8 = checksum((uint8_t*) m_Body, m_Header->m_DataLength);
            if (!correctly) ++(*m_Checksum8);
            break;

        case ds::PACKET_CHECKSUM_16:
            *m_Checksum16 = checksum(
                (uint16_t*)m_Body, m_Header->m_DataLength / 2
            );
            if (!correctly) ++(*m_Checksum16);
            break;

        case ds::PACKET_CHECKSUM_32:
            *m_Checksum32 = checksum(
                (uint32_t*)m_Body, m_Header->m_DataLength / 4
            );
            if (!correctly) ++(*m_Checksum32);
            break;

    }
}

//------------------------------------------------------------------------------
///@brief   Change the header checksum to the correct(?) value.                 
//------------------------------------------------------------------------------
void Packet::fixHeaderChecksum(bool correctly)
{
    uint32_t    checksum32(0);
    uint16_t*   header = (uint16_t*) m_Header;

    for (size_t i=0; i < sizeof(ds::PacketHeader)/2 - 1; ++i) 
    {
        checksum32 += header[i];
    }
    
    m_Header->m_HeaderChecksum = checksum32;

}

//------------------------------------------------------------------------------
///@brief Determine the time encoding scheme for the intra-packet data headers  
//------------------------------------------------------------------------------
irig106::ds::TIME_ENCODING_SCHEME Packet::timeEncoding() const
{
    if (!m_Header->m_PacketSecondaryHeaderTime) 
    {
        return irig106::ds::TES_CH10_RELATIVE;
    }
    else if (m_Header->m_SecondaryTimeHeaderTimeFormat == 0) 
    {
        return irig106::ds::TES_CH4_BINARY;
    }
    else if (m_Header->m_SecondaryTimeHeaderTimeFormat == 1)
    {
        return irig106::ds::TES_CH10_IEEE1588;
    }

    //--------------------------------------------------------------------------
    ///@todo    What is the proper default here?!?                              
    //--------------------------------------------------------------------------
    return irig106::ds::TES_CH10_IEEE1588;
}

//------------------------------------------------------------------------------
///@brief   Set the real-time counter to the given value, complete with changing
///         internal RTC values.                                                
///@warning Currently the internal RTC values are only changed for 1553 packets.
//------------------------------------------------------------------------------
void Packet::setRTC(uint64_t r, bool updateOriginal)
{
    //0. rollover 48-bit new RTC
    //1. update originalRTC 
    //2. update TimeStamp
    //3. update Checksum
    //4. update RTC
    //5. update RTCChar
    
    uint64_t max48bitValue = 0xFFFFFFFFFFFF;
    
    //--------------------------------------------------------------------------
    //0. rollover new RTC if it's value is higher than 48-bit                   
    //--------------------------------------------------------------------------
    r = r & max48bitValue;
    
    //--------------------------------------------------------------------------
    //1. update orginal RTC                                                     
    //--------------------------------------------------------------------------
    if(updateOriginal) m_OriginalRTC = m_RTC;

    //--------------------------------------------------------------------------
    //2. update TimeStamp                                                       
    //--------------------------------------------------------------------------
    
     if (is1553()) { 
        lib::irig106::ch10::ds::ChannelSpecificMilStd1553* csw;
        lib::cast(csw, &m_Body[0]); 

        uint32_t maxDataLen = m_Header->m_DataLength - sizeof(*csw);
        uint32_t offset=0;
        uint8_t* smallBody;
        lib::cast(smallBody, &m_Body[1]);
        for(unsigned i=0; i<csw->m_MessageCount && offset<maxDataLen; ++i){
            lib::irig106::ch10::ds::IntraPacketHeaderMilStd1553* iph;
            lib::cast(iph, &smallBody[offset]);
            iph->m_TimeStamp = (iph->m_TimeStamp + r - m_RTC) & max48bitValue;

            offset+= sizeof(*iph) + iph->m_Length;
        }
    } 
    #ifdef MOVE_FROM_MUTLIPLE_FILE_READER_TO_SET_RTC
    else if (dataType() == irig106::ds::DTN_RECORDING_EVENTS) 
    {
        //------------------------------------------------------------------
        //      Event Packets allow the occurrence of discrete events to be 
        //      recorded ina Ch. 10 file.  An Event Packet will contain one 
        //      or more evnets (the count of which are doucmented in the    
        //      packets' channel Specific Data Word).  Each event definition
        //      will include an Inra-Packet Time Stamp.  If the bit 6 of    
        //      the packet's Pakcet Flag byte is 0, the Intra-Packet Time   
        //      Stamp will be an occurrence of the RTC.  Support for Event  
        //      Pakctes involes only updating these RTC values as described 
        //      above.                                                      
        //------------------------------------------------------------------
        if (m_Header->m_PacketSecondaryHeaderTime == 0) {
            m_SecondaryHeader->m_Time += (r - m_RTC);
        } 
    } else if (dataType() == irig106::ds::DTN_RECORDING_INDEX) {
            #pragma message "WARNING: Nothing done about index packets"
    }
    #endif

    //--------------------------------------------------------------------------
    //3. update Checksum (if needed)                                            
    //--------------------------------------------------------------------------
    bool ignoreChecksum = (checksumType() == ds::PACKET_CHECKSUM_NONE);
    bool isChecksumGood = ignoreChecksum || verifyPacketChecksum();   
    if (!ignoreChecksum) fixChecksum(isChecksumGood); 

    //--------------------------------------------------------------------------
    //4. update RTC                                                             
    //--------------------------------------------------------------------------
    m_RTC = r;

    //--------------------------------------------------------------------------
    //5. update RTCChar                                                         
    //                                                                          
    ///@warning     This only works on little endian systems.                   
    //--------------------------------------------------------------------------
    memcpy(m_Header->m_RTCChar, &m_RTC, sizeof(m_Header->m_RTCChar));

} // void Packet::setRTC(uint64_t r, bool updateOriginal)

//------------------------------------------------------------------------------
///@brief Return the native contents of the packet as a blob.                   
//------------------------------------------------------------------------------
        ///@todo make magic happen                                              
/*
BlobPtr Packet::toNative() const
{
    assert(m_Header != NULL);

    switch (m_Header->m_DataType) {
    } // switch (m_Header->m_DataType) //

} // BlobPtr Packet::toNative() const
*/

template <typename TYPE>
inline TYPE value(TYPE p, TYPE default_value)
{
    TYPE result(default_value);
    if (p != std::numeric_limits<TYPE>::max()) result = p;
    return result;
}

//------------------------------------------------------------------------------
///@brief   Create a clone of the CH10 data, but change the meta data.          
//------------------------------------------------------------------------------
ConstPacketPtr Packet::cheapClone(
    const lib::time::work::InstrumentationTime& absoluteTime 
  , uint64_t fileOffset          
  , uint64_t filePacketNumber    
  , uint64_t rtc                 
) const
{
    Packet* p;
    ConstPacketPtr result(p = new Packet);

    p->m_Header = m_Header;
    p->m_SecondaryHeader = m_SecondaryHeader;
    p->m_AfterHeader = m_AfterHeader;
    p->m_Body = m_Body;
    p->m_Checksum8  = m_Checksum8;
    p->m_Checksum16 = m_Checksum16;
    p->m_Checksum32 = m_Checksum32;
    p->m_OriginalRTC = m_OriginalRTC;
    p->m_Memory =  m_Memory;
    p->m_PacketChecksumChecked = m_PacketChecksumChecked;
    p->m_PacketChecksumValid  = m_PacketChecksumValid;

    p->m_AbsoluteTime = absoluteTime;
    p->m_FileOffset = value(fileOffset, m_FileOffset);
    p->m_FilePacketNumber = value(filePacketNumber, m_FilePacketNumber);
    p->m_RTC = value(rtc, m_RTC);

    return result;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ConstPacketPtr Packet::cheapClone(
    uint64_t fileOffset          
  , uint64_t filePacketNumber    
  , uint64_t rtc                 
) const
{
    return cheapClone(m_AbsoluteTime, fileOffset, filePacketNumber, rtc);
}

#define FILE_OFFSET_FORMAT  "\n    %010d:  "

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static std::string hex_dump(
    void* memory
  , int wordCount
  , uint64_t fileOffset
  , int newLine = 8)
{
    std::string result;

    uint16_t* m((uint16_t*) memory);

    for (int i=0; i < wordCount; ++i) {
        if (!(i % newLine)) {
            result += lib::format(FILE_OFFSET_FORMAT, i + fileOffset);
        }
        result += lib::format(
            "%04x "
          , m[i]
        );
    }

    return result;
}

//------------------------------------------------------------------------------
///@brief   Convert the packet to a stirng for dump / debug purposes.           
//------------------------------------------------------------------------------
std::string Packet::toString(StringLevels level) const
{
    std::string result;

    if (level == TSL_EVERYTHING) {
        switch(dataType()) {
            case lib::irig106::ds::DTN_RECORDING_INDEX:
                result = toStringIndexEverything();
                break;
            default:
                result = toStringDefaultEverything();
                break;
        }
    } else if (level & TSL_PARTS) {
        if (level & TSL_PART_FILE_OFFSET) {
            result += format("%10d. ", m_FileOffset);
        }
        if (level & TSL_PART_CHANNEL_ID) {
            result += format("chan %4d; ", channelID());
        }
        if (level & TSL_PART_PACKET_NUMBER) {
            result += format("pkt #%6d; ", m_FilePacketNumber);
        }
        if (level & TSL_PART_FILE_OFFSET) {
            result += format(
                "typ: %02x(%s) "
              , (uint8_t) dataType()
              , irig106::ds::toString(dataType()).c_str()
            );
        }
    }
    return result;
}

//------------------------------------------------------------------------------
///@brief   Hex dump the body.                                                  
//------------------------------------------------------------------------------
std::string Packet::toStringBodyEverything() const
{
    return hex_dump(m_AfterHeader, (packetLength() >> 1) - 12, m_FileOffset + 24);
}

//------------------------------------------------------------------------------
///@brief   Default convert this packet to a string with *EVERYTHING*           
//------------------------------------------------------------------------------
std::string Packet::toStringDefaultEverything() const
{
    return "";
}

//------------------------------------------------------------------------------
///@brief   Convert everything about this header to a string.                   
//------------------------------------------------------------------------------
std::string Packet::toStringHeaderEverything(bool oneLine) const
{
    return lib::format(
            "0x%04x chan: %4d p-len: %5d (0x%08x)  d-len: %5d (0x%08x) d-ver: %d"
            "%sseq: 0x%02x p-flg:  0x%02x typ:%5s(%02x)   rtc: 0x%02x%02x%02x%02x%02x%02x%scheck: 0x%04x"

          , m_Header->m_SyncPattern
          , m_Header->m_ChannelID
          , m_Header->m_PacketLength
          , m_Header->m_PacketLength
          , m_Header->m_DataLength
          , m_Header->m_DataLength
          , m_Header->m_DataTypeVersion
          , oneLine ? "  " : "\n        "
          , m_Header->m_SequenceNumber
          , m_Header->m_PacketFlags
          , lib::irig106::ds::toString((irig106::ds::DATA_TYPE_NAME) m_Header->m_DataType).c_str()
          , m_Header->m_DataType
          , m_Header->m_RTCChar[5]
          , m_Header->m_RTCChar[4]
          , m_Header->m_RTCChar[3]
          , m_Header->m_RTCChar[2]
          , m_Header->m_RTCChar[1]
          , m_Header->m_RTCChar[0]
          , oneLine ? " " : "     "
          , m_Header->m_HeaderChecksum
        ) 
      + hex_dump(m_Header, 12, m_FileOffset)
    ;
}

template <typename TYPE>
std::string indexNodeToString(TYPE n)
{
    return lib::format(
        "ipts: 0x%012x "
        "d-type: 0x%02x (%s) "
        "channel id: 0x%04x (%4d) "
        "offset: 0x%016x (%6d)"
      , n->m_IntraPacketTimeStamp
      , n->m_Entry.m_DataType
      , lib::irig106::ds::toString((irig106::ds::DATA_TYPE_NAME) n->m_Entry.m_DataType).c_str()
      , n->m_Entry.m_ChannelID
      , n->m_Entry.m_ChannelID
      , n->m_Offset
      , n->m_Offset
    );

}

std::string ipdhToString(uint64_t ipdh)
{
    return lib::format(
        "%03x:%02x:%02x:%02x.%02x"
      , (ipdh >> 32) & 0x0FFF
      , (ipdh >> 24) & 0x00FF
      , (ipdh >> 16) & 0x00FF
      , (ipdh >>  8) & 0x00FF
      , (ipdh >>  0) & 0x00FF
    );
}

//------------------------------------------------------------------------------
///@brief   Convert everything about this index packet to a string.             
//------------------------------------------------------------------------------
std::string Packet::toStringIndexEverything() const
{
    //--------------------------------------------------------------------------
    //  Get the header                                                          
    //--------------------------------------------------------------------------
    std::string result(toStringHeaderEverything());

    //--------------------------------------------------------------------------
    //  Channel Specific Data Word                                              
    //--------------------------------------------------------------------------
    ds::ChannelSpecificDataIndex csdw;
    getChannelSpecificData(csdw);

    result += lib::format(
        "\n" 
        FILE_OFFSET_FORMAT 
        "%04x %04x     csdw: %08x type: %s;  file size: %s;  IPDH: %s;  count: %d"
      , 24 + m_FileOffset
      , csdw.m_Word & 0xFFFF
      , (csdw.m_Word >> 16)
      , csdw.m_Word 
      , csdw.m_IndexType ? "node" : "root"
      , csdw.m_FileSizePresent ? "present" : "absent "
      , csdw.m_IntraPacketDataHeaderPresent ? "present" : "absent "
      , csdw.m_EntryCount
    );

    int body_byte_offset = sizeof(csdw) + 8;    // +8 for recording time tag
    uint8_t* m8((uint8_t*) m_Body);

    for (int i=0; i < csdw.m_EntryCount; ++i) {
        int hex_bytes = 0;
        std::string entry_dump;

        entry_dump += "\n        ";

        if (csdw.m_IndexType == ds::IT_NODE) {
            if (!csdw.m_IntraPacketDataHeaderPresent) 
            {
                ds::NodeIndexEntryNoHeader* n;
                lib::cast(n, &m8[body_byte_offset]);
                entry_dump += indexNodeToString(n);
                hex_bytes = sizeof(ds::NodeIndexEntryNoHeader);

            } else {
                ds::NodeIndexEntryWithHeader* n;
                lib::cast(n, &m8[body_byte_offset]);
                entry_dump += indexNodeToString(n) 
                    + "; ipdh: " + ipdhToString(n->m_IntraPacketDataHeader)
                ;
                hex_bytes = sizeof(ds::NodeIndexEntryWithHeader);
            }
        } else {
            if (!csdw.m_IntraPacketDataHeaderPresent) 
            {
                ds::RootIndexEntryNoHeader* r;
                lib::cast(r, &m8[body_byte_offset]);
                entry_dump += lib::format(
                    "ipts: %012x; offset: %016x"
                  , r->m_IntraPacketTimeStamp
                  , r->m_Offset
                );
                hex_bytes = sizeof(*r);
            } else {
                ds::RootIndexEntryWithHeader* r;
                lib::cast(r, &m8[body_byte_offset]);
                entry_dump += lib::format(
                    "ipts: %012x; offset: %016x"
                  , r->m_IntraPacketTimeStamp
                  , r->m_Offset
                );
                entry_dump += "; ipdh: " + ipdhToString(r->m_IntraPacketDataHeader);
                hex_bytes = sizeof(*r);
            }
        }

//        result += "\n        ";
#if 0
uint16_t* m16((uint16_t*)&m8[body_byte_offset]);
for (int h = 0; h < hex_bytes; h += 2) {
    result += lib::format("%04x ", m16[h]);
}
#endif
        result += entry_dump;
        body_byte_offset += hex_bytes;

    } // for (int i=0; i < csdw.m_EntryCount; ++i) //

    return result + toStringBodyEverything();
}
    
}  // namespace work //
}  // namespace ch10 //
}  // namespace irig106 //
}  // namespace lib //

