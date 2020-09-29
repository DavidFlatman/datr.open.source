//------------------------------------------------------------------------------
///                                                                             
///@file lib_irig106_ch10_ds.h                                                  
///                                                                             
///@par Classification:  UNCLASSIFED, OPEN SOURCE                               
///                                                                             
///@brief Holds the defintiions of various irig106 Chapter 10 data structures.  
///                                                                             
///@par Classification: unclassified                                            
///                                                                             
///@version 2020-09-21  DHF Made open source.                                   
///                                                                             
///@version 2018-11-06  MNT Added CH10_DATA_BETWEEN_PACKETS & CH10_BODY_CHECK...
///                                                                             
///@version 2018-05-31  MNT Added ETHERNET_FORMAT and ETHERNET_TIME_TAG_BITS    
///                                                                             
///@version 2018-05-03  MNT Added CAPTURED_DATA_CONTENT and ETHERNET_SPEED.     
///                                                                             
///@version 2017-09-06  YBS Added Fibre Channel specific Data and Intra-packet  
///                         Header                                              
///                                                                             
///@version 2016-12-07  YBS Replaced BOOST_ASSERT with static_assert            
///                                                                             
///@version 2016-12-07  YBS Added briefs in Ethernet                            
///                                                                             
///@version 2016-12-06  YBS Updated the ethernet header elements                
///                                                                             
///@version 2016-12-05  YBS Added ChannelSpecificDataEthernet                   
///                         Added IntraPacketHeaderEthernet                     
///                                                                             
///@version 2014-09-09  YBS Added MaximumDataPerPacketMilliseconds              
///                                                                             
///@version 2014-08-12  DHF Added file author/version information.              
///                         Added MaximumPacketDurationSeconds                  
///                         Added MaximumPacketWriteDelaySeconds                
///                                                                             
//------------------------------------------------------------------------------
#ifndef LIB_IRIG106_CH10_DS_H
#define LIB_IRIG106_CH10_DS_H
#include "lib_irig106_ds.h"
#include "lib_log_ds.h"

#include <stdint.h>

namespace lib {
namespace irig106 {
namespace ch10 {
namespace ds {

//typedef uint16_t    channelid_t;
typedef uint8_t     sequencenumber_t;

static const lib::log::ds::messageid_t CH10_DATA_BETWEEN_PACKETS (0);
static const lib::log::ds::messageid_t CH10_BODY_CHECKSUM_ERROR  (1);

//------------------------------------------------------------------------------
///@brief   The MaximumPacketWriteDelaySeconds is the maximum time allowed from 
///         when the recorder sees datum until it must be recorded.             
///                                                                             
///@note    We are declaring the value "static const" instead of "extern const" 
///         because some code needs to use the value as a default value for a   
///         passed parameter.                                                   
//------------------------------------------------------------------------------
static const double         MaximumPacketWriteDelaySeconds(1.0);

//------------------------------------------------------------------------------
///@brief   Specifies the maximum time duration that a packet's data may span.  
//------------------------------------------------------------------------------
extern const double         MaximumPacketDurationSeconds;

extern const uint16_t       SYNC_PATTERN;

#pragma pack(push, 4)
//------------------------------------------------------------------------------
/// See 10.6.1.1 of RCC Document 106-09, Telemetry Standard, April 2009         
//------------------------------------------------------------------------------
struct PacketHeader {
    uint16_t                 m_SyncPattern;     ///< should be 0xEB25           
    irig106::ds::channelid_t m_ChannelID;       ///<                            
    uint32_t                 m_PacketLength;    ///< length in bytes; n % 4 = 0 
    uint32_t                 m_DataLength;      ///< See 10-                    
    uint8_t                  m_DataTypeVersion; ///<                            
    sequencenumber_t         m_SequenceNumber;  ///< inc by 1, any start value. 

    union {
        uint8_t m_PacketFlags;
        struct {
            #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint8_t m_SecondaryHeaderPresent:1;
            uint8_t m_PacketSecondaryHeaderTime:1; // 0 = 48-bit RTC, 1 = absolute
            uint8_t m_RelativeTimeCounterSyncError:1;
            uint8_t m_DataOverFlowError:1;
            uint8_t m_SecondaryTimeHeaderTimeFormat:2;
            uint8_t m_ChecksumType:2;
            #else
            uint8_t m_ChecksumType:2;
            uint8_t m_SecondaryTimeHeaderTimeFormat:2;
            uint8_t m_DataOverFlowError:1;
            uint8_t m_RelativeTimeCounterSyncError:1;
            uint8_t m_PacketSecondaryHeaderTime:1;
            uint8_t m_SecondaryHeaderPresent:1;
            #endif
        };
    };

    uint8_t        m_DataType;      /// See Table 10-7 Data Type Names & Desc   
    unsigned char  m_RTCChar[6];    ///< 6 byte time?!?                         
    uint16_t       m_HeaderChecksum;/// sum of all other 16-bit words in header 
}; // struct PacketHeader //
#pragma pack(pop)
static_assert(sizeof(PacketHeader) == 24, "PacketHeader size != 24");

enum DATA_TYPE_VERSION {
    DTV_IRIG106_04  = 0x01
  , DTV_IRIG106_05  = 0x02
  , DTV_IRIG106_07  = 0x03
  , DTV_IRIG106_09  = 0x04

  , DTV_INITIAL_RELEASE = DTV_IRIG106_04
};

#pragma pack(push, 2)
//------------------------------------------------------------------------------
/// See 10.6.1.2 of RCC Document 106-09, Telemetry Standard, April 2009         
//------------------------------------------------------------------------------
struct PacketSecondaryHeader {
    uint64_t    m_Time;
    uint16_t    m_Reserved;
    uint16_t    m_SecondaryHeaderChecksum;
}; // struct PacketSecondaryHeader //
#pragma pack(pop)
static_assert(sizeof(PacketSecondaryHeader) == 12, "PacketSecondaryHeader size != 12");

//------------------------------------------------------------------------------
/// See 10.6.1.4 of RCC Document 106-09, Telemetry Standard, April 2009         
//------------------------------------------------------------------------------
struct PacketTrailer {
    /// don't know how we're going to handle ...                                
};

//------------------------------------------------------------------------------
///  See 10.6.1.1.g of RCC Document 106-09, Telemetry Standard, April 2009      
//------------------------------------------------------------------------------
enum PACKET_CHECKSUM_TYPE {
    PACKET_CHECKSUM_NONE        = 0
  , PACKET_CHECKSUM_8           = 1
  , PACKET_CHECKSUM_16          = 2
  , PACKET_CHECKSUM_32          = 3
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
union ChannelSpecificDataComputer0{
    uint32_t    m_ChannelSpecificDataWord;
    
};
static_assert(sizeof(ChannelSpecificDataComputer0) == 4,
                       "ChannelSpecificDataComputer0 size != 4");
//------------------------------------------------------------------------------
///                                                                             
///     See Figure 10-14. PCM packet channel specific data format of the        
///     RCC Document 106-09, Telemetry Standard, Chapter 10, April 2009         
/// <pre>                                                                       
/// ----------------------------------------------------------------------------
/// | msb                                                                  lsb |
/// | 31     30     29   28  27       24 23        18 17                     0 |
/// ----------------------------------------------------------------------------
/// |  R  |  IPH  | MA | MI |  LOCKST   |    MODE    |          SYNCOFFSET     |
/// ----------------------------------------------------------------------------
/// </pre>                                                                      
///                                                                             
///     -#  (bit 31) Reserved.                                                  
///     -#  (bit 30) Intra-Packet Header (IPH).  Indicates if Intra-Packet      
///         Headers (Intra-Packet Time Stamp and Intra-Packet Data Header) are  
///         inserted before each minor frame. Intra-Packet Headers are only     
///         optional because of the mode selection. This determines whether     
///         Intra-Packet Headers are included or omitted.                       
///             -#  0 = Intra-Packet Headers are omitted for Throughput Mode.   
///             -#  1 = Intra-Packet Headers are required for Packed Data and   
///                 Unpacked Data modes.                                        
///     -#  (bit 29)Major Frame Indicator (MA).  Indicates if the first word    
///         in the packet is the beginning of a major frame. Not valid for      
///         Throughput Mode.                                                    
///             -#  0 = First word is not the beginning of a major frame.       
///             -#  1 = First word is the beginning of a major frame.           
///     -#  (bit 28)Minor Frame Indicator (MI).  Indicates if the first word    
///         in the packet is the beginning of a minor frame. Not valid for      
///         Throughput Mode.                                                    
///             -#  0 = First word is not the beginning of a minor frame.       
///             -#  1 = First word is the beginning of a minor frame.           
///     -#  (bits 27-24)Lock Status (LOCKST).  Indicates the lock status of     
///         the frame synchronizer. Not valid for Throughput Mode.              
///             -#  Bits 27-26: Indicate Minor Frame Status.                    
///                     -#  00 = Reserved.                                      
///                     -#  01 = Reserved.                                      
///                     -#  10 = Minor Frame Check (after losing Lock).         
///                     -#  11 = Minor Frame Lock.                              
///             -#  Bits 25-24: Indicate Major Frame Status.                    
///                     -#  00 = Major Frame Not Locked.                        
///                     -#  01 = Reserved.                                      
///                     -#  10 = Major Frame Check (after losing Lock).         
///                     -#  11 = Major Frame Lock.                              
///     -#  Mode (MODE). (bits 23-18) indicates the data packing mode.          
///             -#     Bits 23-22: Reserved.                                    
///             -#     Bit 21: Alignment Mode.                                  
///                      -#  0 = 16 Bit Alignment Mode enabled.                 
///                      -#  1 = 32 Bit Alignment Mode enabled.                 
///             -#     Bit 20: Indicates Throughput Data Mode.                  
///                      -#  0 = Throughput Data Mode not enabled.              
///                      -#  1 = Throughput Data Mode enabled.                  
///             -#     Bit 19: Indicates Packed Data Mode.                      
///                      -#  0 = Packed Data Mode not enabled.                  
///                      -#  1 = Packed Data Mode enabled.                      
///             -#     Bit 18: Indicates Unpacked Data Mode.                    
///                      -#  0 = Unpacked Data Mode not enabled.                
///                      -#  1 = Unpacked Data Mode enabled.                    
///     -#  Sync Offset (SYNCOFFSET). (bits 17-0) contains an 18-bit binary     
///         value representing the Word offset into the major frame for the     
///         first data word in the packet. Not valid for Packed or Throughput   
///         Mode.                                                               
///                                                                             
///@note Minor Frame Definition.                                                
///         The minor frame is defined as the data                              
///         structure in time sequence from the beginning of a minor frame      
///         synchronization pattern to the beginning of the next minor frame    
///         synchronization pattern. Please reference IRIG-106 Chapter 4,       
///         paragraph 4.3.2 for minor/major frame definition.                   
///                                                                             
//------------------------------------------------------------------------------
union ChannelSpecificDataPcm1 {
    uint32_t    m_ChannelSpecificDataPcm1;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t    m_Reserved:1;                   //  bit 31              
            uint32_t    m_IntraPacketHeader:1;          //  bit 30              
            uint32_t    m_MajorFrameIndicator:1;        //  bit 29              
            uint32_t    m_MinorFrameIndicator:1;        //  bit 28              
            // uint32_t    m_LockStatus:4;              //  bit 27-24           
                uint32_t    m_MinorFrameLockStatus:2;       //  bit 27-26       
                uint32_t    m_MajorFrameLockStatus:2;       //  bit 25-24       
            // uint32_t    m_PackingMode:6;             // bit 23-18            
                uint32_t    m_ModeReserved:2;               //  bit 23-22       
                uint32_t    m_AlignmentMode:1;              //  bit 21          
                uint32_t    m_ThroughputDataMode:1;         //  bit 20          
                uint32_t    m_PackedDataMode:1;             //  bit 19          
                uint32_t    m_UnpackedDataMode:1;           //  bit 18          
            uint32_t    m_SyncOffset:18;                //  bit 17-0            
        #else
            uint32_t    m_SyncOffset:18;                //  bit 17-0            
            // uint32_t    m_PackingMode:6;             // bit 23-18            
                uint32_t    m_UnpackedDataMode:1;           //  bit 18          
                uint32_t    m_PackedDataMode:1;             //  bit 19          
                uint32_t    m_ThroughputDataMode:1;         //  bit 20          
                uint32_t    m_AlignmentMode:1;              //  bit 21          
                uint32_t    m_ModeReserved:2;               //  bit 23-22       
            // uint32_t    m_LockStatus:4;              //  bit 27-24           
                uint32_t    m_MajorFrameLockStatus:2;       //  bit 25-24       
                uint32_t    m_MinorFrameLockStatus:2;       //  bit 27-26       
            uint32_t    m_MinorFrameIndicator:1;        //  bit 28              
            uint32_t    m_MajorFrameIndicator:1;        //  bit 29              
            uint32_t    m_IntraPacketHeader:1;          //  bit 30              
            uint32_t    m_Reserved:1;                   //  bit 31              
        #endif
    };
}; // union ChannelSpecificDataPcm //
static_assert(sizeof(ChannelSpecificDataPcm1) == 4, 
                                    "ChannelSpecificDataPcm1 size != 4");
//------------------------------------------------------------------------------
/// See Figure 10-22                                                            
///     -#  (bits 31-12)    Reserved.                                           
///     -#  (bits 11-10)    Date - reserved                                     
///     -#  (bits 9)        date format                                         
///                             -#  0 = IRIG day available                      
///                             -#  1 = Month and Year available                
///     -#  (bit 8)         Indicates if this is a leap year.                   
///     -#  (bits 7-4)      Indicates the Time Data packet format.              
///                             -#  0x0 = IRIG-B                                
///                             -#  0x1 = IRIG-A                                
///                             -#  0x2 = IRIG-G                                
///                             -#  0x3 = real-time clock                       
///                             -#  0x4 = UTC time from GPS                     
///                             -#  0x5 = native GPS time                       
///                             -#  0x6- 0xE = reserved                         
///                             -#  0xF = none                                  
///     -#  (bits 3-0)      Indiciates the source of the time in the payload    
///                             -#  0x0 = internal                              
///                             -#  0x1 = external                              
///                             -#  0x2 = internal from RMM                     
///                             -#  0x3 - 0xE reserved                          
///                             -#  0xF = none                                  
//------------------------------------------------------------------------------
union ChannelSpecificDataTime {
    uint32_t    m_ChannelSpecificDataTime;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t    m_ReservedTime:20;
            uint32_t    m_ReservedDate:2;
            uint32_t    m_DateFormat:1;
            uint32_t    m_LeapYear:1;
            uint32_t    m_Format:4;
            uint32_t    m_Source:4;
        #else
            uint32_t    m_Source:4;
            uint32_t    m_Date:4;
            uint32_t    m_LeapYear:1;
            uint32_t    m_DateFormat:1;
            uint32_t    m_ReservedDate:2;
            uint32_t    m_Format:4;
        #endif
    };
}; // union ChannelSpecificDataTime //
static_assert(sizeof(ChannelSpecificDataTime) == 4,
                                    "ChannelSpecificDataTime size != 4");


enum TIME_FORMAT {
    TIME_FORMAT_IRIG_B      = 0x00
  , TIME_FORMAT_IRIG_A      = 0x01
  , TIME_FORMAT_IRIG_G      = 0x02
  , TIME_FORMAT_REAL_TIME   = 0x03
  , TIME_FORMAT_UTC_GPS     = 0x04
  , TIME_FORMAT_NATIVE_GPS  = 0x05

  , TIME_FORMAT_NONE        = 0x0f
  , TIME_FORMAT_INVALID     = 0x0f
};

enum TIME_SOURCE {
    TIME_SOURCE_INTERNAL    = 0x00
  , TIME_SOURCE_EXTERNAL    = 0x01
  , TIME_SOURCE_INTERNAL_RMM= 0x02
  , TIME_SOURCE_NONE        = 0x0f
};

extern uint32_t RTC_FREQUENCY;

//------------------------------------------------------------------------------
///  See Figure 10-25                                                           
///     -#  (bits 31-30)    Time Tag Bits (TTB) indeicate which bit of the      
///                         MIL-STD_1553 message was Intra-Packet Header time   
///                         tags:                                               
///                             -#  00 = last bit of the last word of the msg.  
///                             -#  01 = first bit of the first word of the msg.
///                             -#  10 = last bit of the first (command) word.  
///                             -#  11 = reserved                               
///     -#  (bits 29-24)    Reserved.                                           
///     -#  (bits 23-0)     A packet with n MIL-STD-1553 messages.              
//------------------------------------------------------------------------------
union ChannelSpecificMilStd1553 {
    uint32_t    m_ChannelSpecificMilStd1553;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t    m_TimeTagBits:2;
            uint32_t    m_Reserved:6;
            uint32_t    m_MessageCount:24;
        #else
            uint32_t    m_MessageCount:24;
            uint32_t    m_Reserved:6;
            uint32_t    m_TimeTagBits:2;
        #endif
    };
};

enum TIME_TAGGED_BITS {
    TIME_TAGGED_LAST_BIT_LAST_WORD      = 0x00
  , TIME_TAGGGED_FIRST_BIT_FIRST_WORD   = 0x01
  , TIME_TAGGGED_LAST_BIT_FIRST_WORD    = 0x02
};

#pragma pack(push, 2)
struct IntraPacketHeaderMilStd1553 {
    uint64_t    m_TimeStamp;

    //--------------------------------------------------------------------------
    ///  See Figure 10-28                                                       
    //--------------------------------------------------------------------------
    union {
        uint16_t    m_BlockStatusWord;
        struct {
            #ifdef BIT_FIELD_HIGH_BIT_FIRST
                uint16_t    m_Reserved:2;
                uint16_t    m_Side:1;           // 0 = side A, 1 = side B       
                uint16_t    m_MessageError:1;   // 0 = no error; 1 = msg error  
                uint16_t    m_RTtoRT:1;         // 0 = no RT to RT; 1 = RT to RT
                uint16_t    m_FormatError:1;    // 0 = no format error; 1 = err 
                uint16_t    m_ResponseTimeOut:1;// 0 = no response time out     
                uint16_t    m_Reserved2:3;      
                uint16_t    m_WordCountError:1; // 0 = no word count error      
                uint16_t    m_SyncTypeError:1;  // 0 = no sync type error       
                uint16_t    m_InvalidWordError:1; // 0 = no invalid word error  
                uint16_t    m_Reserved3:3;      
            #else
                uint16_t    m_Reserved3:3;      
                uint16_t    m_InvalidWordError:1; // 0 = no invalid word error  
                uint16_t    m_SyncTypeError:1;  // 0 = no sync type error       
                uint16_t    m_WordCountError:1; // 0 = no word count error      
                uint16_t    m_Reserved2:3;      
                uint16_t    m_ResponseTimeOut:1;// 0 = no response time out     
                uint16_t    m_FormatError:1;    // 0 = no format error; 1 = err 
                uint16_t    m_RTtoRT:1;         // 0 = no RT to RT; 1 = RT to RT
                uint16_t    m_MessageError:1;   // 0 = no error; 1 = msg error  
                uint16_t    m_Side:1;           // 0 = side A, 1 = side B       
                uint16_t    m_Reserved:2;
            #endif
        };
    };
    //--------------------------------------------------------------------------
    /// The Gap Times Word indicates the number of tenths of microseconds in    
    /// length of the internal gaps within a single transaction.  For most      
    /// messages, only GAP1 is meaningful.  It measures the time between the    
    /// command or data word and the first (and only) status word in the        
    /// message.  For RT-to-RT messages, GAP2 measurest he time between the     
    /// last data word and the second status word.  The Gap Times Word bit      
    /// definitions are shown in Figure 10-29.                                  
    //--------------------------------------------------------------------------
    union {
        uint16_t    m_GapTimesWord;
        struct {
            #ifdef BIT_FIELD_HIGH_BIT_FIRST
                uint16_t    m_Gap2:8;   // time between last data word & 2nd sta
                uint16_t    m_Gap1:8;   // time before first 
            #else
                uint16_t    m_Gap1:8;   // time before first 
                uint16_t    m_Gap2:8;   // time between last data word & 2nd sta
            #endif
        };
    };

    //--------------------------------------------------------------------------
    /// The Length of the message is the total number of bytes in the message.  
    /// A message conssits of command words, data words, and status words.      
    //--------------------------------------------------------------------------
    uint16_t    m_Length;
};
#pragma pack(pop)
static_assert(sizeof(IntraPacketHeaderMilStd1553) == 14,
                    "IntraPacketHeaderMilStd1553 size != 14");
//------------------------------------------------------------------------------
/// See figure 10-57 of RCC Document 106-09, Telemetry Standard, April 2009     
///     -#  (bit 31)    Index Type (IT). Indicates the type of index packet.    
///                         -#  0 = root index                                  
///                         -#  1 = node index                                  
///     -#  (bit 30)    File Size Present (FSP). Indicates if the file size at  
///                     the time the index packet was created is present.       
///     -#  (bit 29)    Index Intra-Packet Data Header (IPDH). Indicates the    
///                     presence of the Intra-Packet Data header.               
///     -#  (28-16)     Reserved. Bits 28-16 are reserved.                      
///     -#  (bits 15-0) Index Entry Count. Indicate the unsigned binary value of
///                     the number of index entries included in the packet.  An 
///                     integral number of complete index entries will be in    
//------------------------------------------------------------------------------
union ChannelSpecificDataIndex {
    uint32_t    m_Word;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIELD_FIRST
            uint32_t    m_IndexType:1;                          //  bit  31     
            uint32_t    m_FileSizePresent:1;                    //  bit  30     
            uint32_t    m_IntraPacketDataHeaderPresent:1;       //  bit  29     
            uint32_t    m_Reserved:13;                          //  bits 28-16  
            uint32_t    m_EntryCount:16;                        //  bits 15-0   
        #else
            uint32_t    m_EntryCount:16;                        //  bits 15-0   
            uint32_t    m_Reserved:13;                          //  bits 28-16  
            uint32_t    m_IntraPacketDataHeaderPresent:1;       //  bit  29     
            uint32_t    m_FileSizePresent:1;                    //  bit  30     
            uint32_t    m_IndexType:1;                          //  bit  31     
        #endif
    };

}; // union ChannelSpecificDataIndex 
static_assert(sizeof(ChannelSpecificDataIndex) == 4,
                        "ChannelSpecificDataIndex size != 4");

enum INDEX_TYPE {
    IT_ROOT     = 0x00
  , IT_NODE     = 0x01
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
union NodeIndexEntry {
    uint32_t    m_Word;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIELD_FIRST
            uint32_t    m_Reserved:8;
            uint32_t    m_DataType:8;
            uint32_t    m_ChannelID:16;
        #else
            uint32_t    m_ChannelID:16;
            uint32_t    m_DataType:8;
            uint32_t    m_Reserved:8;
        #endif
    };
};
static_assert(sizeof(NodeIndexEntry) == 4, "NodeIndexEntry size != 4");
#pragma pack(push, 4)
struct NodeIndexEntryNoHeader {
    uint64_t        m_IntraPacketTimeStamp;
    NodeIndexEntry  m_Entry;
    uint64_t        m_Offset;
};
#pragma pack(pop)
static_assert(sizeof(NodeIndexEntryNoHeader) == 20,
                                    "NodeIndexEntryNoHeader size != 20");



#pragma pack(push, 4)
struct NodeIndexEntryWithHeader {
    uint64_t        m_IntraPacketTimeStamp;
    uint64_t        m_IntraPacketDataHeader;
    NodeIndexEntry  m_Entry;
    uint64_t        m_Offset;
};
#pragma pack(pop)
static_assert(sizeof(NodeIndexEntryWithHeader) == 28,
                            "NodeIndexEntryWithHeader size != 28");

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct RootIndexEntryNoHeader {
    uint64_t    m_IntraPacketTimeStamp;
    uint64_t    m_Offset;
};
static_assert(sizeof(RootIndexEntryNoHeader) == 16,
                            "RootIndexEntryNoHeader size != 16");


struct RootIndexEntryWithHeader {
    uint64_t    m_IntraPacketTimeStamp;
    uint64_t    m_IntraPacketDataHeader;
    uint64_t    m_Offset;
};
static_assert(sizeof(RootIndexEntryWithHeader) == 24, 
                    "RootIndexEntryWithHeader size != 24");


//------------------------------------------------------------------------------
/// See figure 10-72 of RCC Document 106-09, Telemetry Standard, April 2009     
///                                                                             
///     -#  (bit 31) Embedded Time (ET).  Indicates if embedded time is present 
///         in the MPEG-2 video data.                                           
///             -#  0 = No embedded time present                                
///             -#  1 = Embedded time present                                   
///     -#  (bit 30) Intra-Packet Header (IPH).  Indicates if Intra-Packet Time 
///         Stamps are inserted before each Transport Packet.                   
///             -#  0 = Intra-Packet Times Not Present                          
///             -#  1 = Intra-Packet Times Present                              
///     -#  (bit 29) SCR/RTC Sync (SRS).  Indicates if the MPEG-2 SCR is RTC.   
///             -#  0 = SCR is not synchronized with the 10 MHz RTC             
///             -#  1 = SCR is synchronized with the 10 MHz RTC                 
///     -#  (bit 28) KLV.  Indicates if KLV Metadata is present in the MPEG-2   
///         video data.                                                         
///             -#  0 = No KLV Metadata present                                 
///             -#  1 = KLV Metadata is present                                 
///     -#  (bit 27-24) Payload.  Indicates the payload type within the MPEG-2  
///         stream per MISP Xon2:                                               
///             -#  0000 = MPEG-2 MP\@ML                                        
///             -#  0001 = H.264 MP\@L2.1                                       
///             -#  0010 = H.264 MP\@L2.2                                       
///             -#  0011 = H.264 MP\@L3                                         
///             -#  0100-1111 = Reserved.                                       
///     -#  (bit 23) Byte Alignment.  Indicates the MPEG-2 data payload         
///         byte alignment within 16-bit words.                                 
///             -#  0 = Little endian as referenced in Figure 10-74.            
///             -#  1 = Big endian as referenced in Figure 10-75.               
///     -#  (bits 22-0) Reserved.                                               
///                                                                             
//------------------------------------------------------------------------------
union ChannelSpecificDataVideo0 {
    uint32_t    m_ChannelSpecificDataVideo0;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t    m_EmbeddedTime:1;               //   bit 31             
            uint32_t    m_IntraPacketHeader:1;          //   bit 30             
            uint32_t    m_SRCRTCSync:1;                 //   bit 29             
            uint32_t    m_KLV:1;                        //   bit 28             
            uint32_t    m_PlayloadType:4;               //   bit 27-24          
            uint32_t    m_ByteAlignment:1;              //   bit 23             
            uint32_t    m_ReservedVideo0:23;            //   bit 22-0           
        #else
            uint32_t    m_ReservedVideo0:23;            //   bit 22-0           
            uint32_t    m_ByteAlignment:1;              //   bit 23             
            uint32_t    m_PlayloadType:4;               //   bit 27-24          
            uint32_t    m_KLV:1;                        //   bit 28             
            uint32_t    m_SRCRTCSync:1;                 //   bit 29             
            uint32_t    m_IntraPacketHeader:1;          //   bit 30             
            uint32_t    m_EmbeddedTime:1;               //   bit 31             
        #endif
    };
}; // union ChannelSpecificDataPCM //
static_assert(sizeof(ChannelSpecificDataVideo0) == 4,
                    "ChannelSpecificDataVideo0 size != 4");

//------------------------------------------------------------------------------
/// Table 10-7.  Data Type Names and Descriptions (p 10-32).                    
//------------------------------------------------------------------------------
enum CURRENT_DATA_TYPE_VERSION {
    CDTV_USER_DEFINED           = 0x02
  , CDTV_SETUP_RECORD           = 0x03
  , CDTV_RECORDING_EVENTS       = 0x03
  , CDTV_RECORDING_INDEX        = 0x03
  , CDTV_PCM_CH4_CH8            = 0x03
  , CDTV_TIME_DATA_FMT_1        = 0x03
  , CDTV_MIL_STD_1553B          = 0x03
  , CDTV_16PP194_BUS            = 0x03
  , CDTV_ANALOG_FMT_1           = 0x02
  , CDTV_DISCRETE_DATA_FMT_1    = 0x02
  , CDTV_GENERIC_MSESSAGE_DATA  = 0x02
  , CDTV_ARINC429_DATA          = 0x02
  , CDTV_MPEG2_H264             = 0x04
  , CDTV_MPEG2_ISO13818         = 0x02
  , CDTV_MPEG2_ISO14496         = 0x03
};

//------------------------------------------------------------------------------
///@brief   Specifies 100 milliseconds worth of data in a ch10 packet           
//------------------------------------------------------------------------------
extern const uint64_t MaximumDataPerPacketMilliseconds;   //'int' to be compatible





//------------------------------------------------------------------------------
///@brief Ethernet Data Packet - Channel Specific Data Word                     
///                                                                             
/// It indicates the format of the Ethernet data packet contents                
//------------------------------------------------------------------------------
union ChannelSpecificDataEthernet {
    uint32_t    m_ChannelSpecificDataEthernet;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t    m_Format : 4;               // bit 31-28
            uint32_t    m_TTB : 3;                  // bit 27-25
            uint32_t    m_Reserved : 9;             // bit 24-16
            uint32_t    m_NumberOfFrames : 16;      // bit 15-0;
        #else
            uint32_t    m_NumberOfFrames : 16;      // bit 15-0;
            uint32_t    m_Reserved : 9;            // bit 24-16
            uint32_t    m_TTB : 3;                  // bit 27-25
            uint32_t    m_Format : 4;               // bit 31-28
        #endif
    };
}; // union ChannelSpecificDataPcm //
static_assert(sizeof(ChannelSpecificDataEthernet) == 4, 
                                "ChannelSpecificDataEthernet size != 4");

enum ETHERNET_FORMAT {
    IEEE_802_3_MAC_FRAME  = 0
};

enum ETHERNET_TIME_TAG_BITS {
    TIME_TAG_FIRST_BIT_DEST_ADDR    = 0
  , TIME_TAG_LAST_BIT_CHECKSUM      = 1
  , TIME_TAG_FIRST_BIT_PAYLOAD      = 2
  , TIME_TAG_LAST_BIT_PAYLOAD       = 3
};


//------------------------------------------------------------------------------
///@brief Ethernet Data Packet - Intra-packet header                            
///                                                                             
/// After the channel-specific data, Ethernet data is inserted into the packet. 
/// Each frame is preceded by an Intra-packet header.                           
//------------------------------------------------------------------------------
#pragma pack(push, 1)
struct IntraPacketHeaderEthernet {
    uint64_t    m_TimeStamp;

    union {
        uint32_t    m_FrameId;
        struct {
            #ifdef BIT_FIELD_HIGH_BIT_FIRST
                uint32_t    m_FrameCRCError : 1;    // bit 31 (0=no frame CRC error/1=frame error encountered
                uint32_t    m_FrameError : 1;       // bit 30 (0=no frame error/1=frame error encountered
                uint32_t    m_Content : 2;          // bit 29-28 
                uint32_t    m_Speed : 4;            // bit 27-24
                uint32_t    m_NetId : 8;            // bit 23-16
                uint32_t    m_DataCRCError : 1;     // bit 15
                uint32_t    m_DataLengthError : 1;  // bit 14
                uint32_t    m_DataLength : 14;      // bit 13-0 (length of the frame in bytes)
            #else
                uint32_t    m_DataLength : 14;      // bit 13-0
                uint32_t    m_DataLengthError : 1;  // bit 14
                uint32_t    m_DataCRCError : 1;     // bit 15
                uint32_t    m_NetId : 8;            // bit 23-16
                uint32_t    m_Speed : 4;            // bit 27-24
                uint32_t    m_Content : 2;          // bit 29-28 
                uint32_t    m_FrameError : 1;       // bit 30
                uint32_t    m_FrameCRCError : 1;    // bit 31 (0=no frame CRC error/1=frame error encountered
            #endif

        };
    };

};
static_assert(sizeof(IntraPacketHeaderEthernet ) == 12, 
                                "IntraPacketHeaderEthernet size != 12");

#pragma pack(pop)

enum CAPTURED_DATA_CONTENT {
    FULL_MAC_FRAME  = 0
  , PAYLOAD_ONLY    = 1
};

enum ETHERNET_SPEED {
    AUTO        = 0
  , MBPS10      = 1
  , MBPS100     = 2
  , GBPS1       = 3
  , GBPS10      = 4
};


//------------------------------------------------------------------------------
///@brief Fibre Channel Data Packet - Channel Specific Data Word                
///                                                                             
/// It indicates the format of the Ethernet data packet contents                
//------------------------------------------------------------------------------
union ChannelSpecificDataFibreChannel {
    uint32_t    m_ChannelSpecificDataFibreChannel;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t    m_Format : 4;               
            uint32_t    m_Reserved : 12; 
            uint32_t    m_NumberOfFrames : 16;  
        #else
            uint32_t    m_NumberOfFrames : 16;     
            uint32_t    m_Reserved : 12;          
            uint32_t    m_Format : 4;            
        #endif
    };
}; // union ChannelSpecificDataPcm //
static_assert(sizeof(ChannelSpecificDataFibreChannel) == 4, 
                                "ChannelSpecificDataFibreChannel size != 4");

//------------------------------------------------------------------------------
///@brief Fibre Channel Intra-packet header                                     
///                                                                             
/// After the channel-specific data, Fibre Channel data is inserted into the    
/// packet. Each frame is preceded by an Intra-packet header.                   
//------------------------------------------------------------------------------
#pragma pack(push, 4)
struct IntraPacketHeaderFibreChannel {
    uint64_t    m_TimeStamp;                        // Intra-packet time-stamp

    union {
        uint32_t    m_FrameId;

        struct {
            #ifdef BIT_FIELD_HIGH_BIT_FIRST
                uint32_t    m_FrameError : 1;       // bit 31 (0=no frame error/1=frame error encountered
                uint32_t    m_FrameCRCError : 1;    // bit 30 (0=no frame CRC error/1=frame error encountered
                uint32_t    m_OverrunError : 1;     // bit 29 (0=no overrun error/1=overrun error detected prior to this Fibre Channel Frame

                //--------------------------------------------------------------
                // Bit 28 indicates whether the Fibre Channel frame delimeters, 
                // header, and CRC are removed, resulting in a stripped Fibre   
                // Channel frame with data payload only                         
                //                                                              
                // (0=stripped mode; only Fibre Channel data payload is present)
                // (1=non-stripped mode; complete Fibre Channel frame is present)
                //--------------------------------------------------------------
                uint32_t    m_StrippedMode : 1;    


                //--------------------------------------------------------------
                // Bits 27-26 specify the type of the Fibre Channel frame.      
                // Content bits are only valid when in non-stripped mode (bit 28
                // = 1)                                                         
                //                                                              
                // (00 = complete data frame)                                   
                // (01 = complete link control frame)                           
                // (10-11 = reserved                                            
                //--------------------------------------------------------------
                uint32_t    m_Content : 2;

                //--------------------------------------------------------------
                // Bits 25-24 specify the Fibre Channel topology of frame from  
                // the port.                                                    
                //--------------------------------------------------------------
                uint32_t    m_Topology : 2; // (00=passive, 01-11 = reserved)

                uint32_t    m_Reserved : 5;

                //--------------------------------------------------------------
                // Bits 18-16 indicate a binary value for the end-of-frame      
                // delimeter that terminated the Fibre Channel frame.  This is  
                // applicable only in stripped mode.                            
                //                                                              
                // 000 = EOF Normal                                             
                // 001 = EOF Terminate                                          
                // 010 = EOF Disconnect Terminate                               
                // 011 = EOF Abort                                              
                // 100 = EOF Normal Invalid                                     
                // 101 = EOF EOF Disconnect Terminate Invalid                   
                // 110 = EOF Remove Terminate                                   
                // 111 = EOF Remove Terminate Invalid                           
                //--------------------------------------------------------------
                uint32_t    m_EndOfFrame : 3;

                //--------------------------------------------------------------
                // Bits 15-12 indicate a binary value for the start-of-frame    
                // delimeter that began the Fibre Channel frame.  This is       
                // applicable only in stripped mode.                            
                //                                                              
                // 0000 – (0): SOF Connect Class-1 (SOFc1)                      
                // 0001 – (1): SOF Initiate Class-1 (SOFi1)                     
                // 0010 – (2): SOF Normal Class-1 (SOFn1)                       
                // 0011 – (3): SOF Initiate Class-2 (SOFi2)                     
                // 0100 – (4): SOF Normal Class-2 (SOFn2)                       
                // 0101 – (5): SOF Initiate Class-3 (SOFi3)                     
                // 0110 – (6): SOF Normal Class-3 (SOFn3)                       
                // 0111 – (7): SOF Activate Class-4 (SOFc4)                     
                // 1000 – (8): SOF Initiate Class-4 (SOFi4)                     
                // 1001 – (9): SOF Normal Class-4 (SOFn4)                       
                // 1010 – (A): SOF Fabric (SOFf)                                
                // 1011 – 1111(B-F): Reserved– 1111(B-F): Reserved              
                //--------------------------------------------------------------
                uint32_t    m_StartOfFrame : 4;

                //--------------------------------------------------------------
                // Frame Length. In stripped mode, bits 11-0 indicate the length
                // of the frame’s data payload in bytes (excluding the SOF and  
                // EOF delimiters and CRC word). In stripped mode, maximum      
                // length is 2112 bytes. In non-stripped mode, bits 11-0        
                // indicate the length of the entire Fibre Channel frame in     
                // bytes. The frame length must be divisible by 4.              
                //--------------------------------------------------------------
                uint32_t    m_FrameLength : 12;
            #else
                uint32_t    m_FrameLength : 12;
                uint32_t    m_StartOfFrame : 4;
                uint32_t    m_EndOfFrame : 3;
                uint32_t    m_Topology : 2; // (00=passive, 01-11 = reserved)
                uint32_t    m_Reserved : 5;
                uint32_t    m_Content : 2;
                uint32_t    m_StrippedMode : 1; 

                uint32_t    m_OverrunError : 1;     // bit 29 (0=no overrun error/1=overrun error detected prior to this Fibre Channel Frame
                uint32_t    m_FrameCRCError : 1;    // bit 30 (0=no frame CRC error/1=frame error encountered
                uint32_t    m_FrameError : 1;       // bit 31 (0=no frame error/1=frame error encountered


            #endif

        };


    };

};
static_assert(sizeof(IntraPacketHeaderFibreChannel) == 12, 
                                "IntraPacketHeaderFibreChannel size != 12");


#pragma pack(pop)




}  // namespace ds //
}  // namespace ch10 //
}  // namespace irig106 //
}  // namespace lib //

#endif 
