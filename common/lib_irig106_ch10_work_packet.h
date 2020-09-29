//------------------------------------------------------------------------------
///@file lib_irig106_ch10_work_packet.h                                         
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///@brief Holds the declaration of the lib::irig106::ch10::work::Packet class.  
//------------------------------------------------------------------------------
#ifndef LIB_IRIG106_CH10_WORK_PACKET_H
#define LIB_IRIG106_CH10_WORK_PACKET_H

#include "lib_ds_flags.h"
#include "lib_ds_shared_ptr.h"
#include "lib_irig106_ch10_ds.h"
#include "lib_irig106_ds.h"
#include "lib_irig106_ds_time.h"
#include "lib_time_work_instrumentationtime.h"

#include <limits>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <string.h>     // memcpy                                               
#include <vector>

namespace lib {
namespace irig106 {
namespace ch10 {
namespace work {

class Packet;

using PacketPtr      = lib::ds::shared_ptr<Packet>;
using ConstPacketPtr = lib::ds::shared_ptr<const Packet>;

//------------------------------------------------------------------------------
///                                                                             
///@brief   A chapter 10 packet (header, secondary header, body, trailer).      
///                                                                             
///@par Class: Packet                                                           
///                                                                             
///@par Purpose:                                                                
///         The Packet class represents a Chapter 10 packet.  The class itself  
///         really is borderline between a struct and a class.  We finally      
///         caved in making it a class, but we're *gasp* exposing the internal  
///         pointers ... because it's more a struct that a class.               
///                                                                             
///@warning Shallow Copy Use In Copy Constructor / Assignment Operator          
///         This has been mentioned several times already in warnings, but ...  
///         it is important to understand that the copy constructor's @e const  
///         and the assignment operator's @e const on the input parameter is    
///         horribly misleading.                                                
///\n\n                                                                         
///         We are copying @e pointers under the covers.  This means that you   
///         can change the value of the "const" parameter by changing the       
///         value of a different object.                                        
///\n\n                                                                         
///         We could do a deep copy, but that would be far more expensive than  
///         it is worth.  If you need a deep copy, use the @e copy method.      
///         At this point, the @e only reason that the copy constructor and     
///         assignment operator even exist is so that we can use std::map.      
///         That said, some of the type conversion also uses them.              
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///        // Read in a tmats file                                              
///        std::string file(                                                    
///            commandLine()["setup-tmats"].string()                            
///        );                                                                   
///                                                                             
///        std::ifstream inFile;                                                
///        inFile.open(file);                                                   
///                                                                             
///        std::stringstream strStream;                                         
///        strStream << inFile.rdbuf();                                         
///        std::string str = strStream.str();                                   
///                                                                             
///        const char *resultstr = str.c_str();                                 
///        char *res = new char[strlen(resultstr) + 1];                         
///        strcpy(res, resultstr);                                              
///                                                                             
///        // Use Constructor                                                   
///        lib::irig106::ch10::work::Packet p1(                                 
///              lib::irig106::ds::DTN_SETUP_RECORD // DataTypeName             
///            , res                     // res is the tmats data read from file
///            , str.size()                 // the data size                    
///        );                                                                   
///                                                                             
///       // Now do something with "packet"                                     
///       ...                                                                   
///                                                                             
///     @endcode                                                                
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2019-09-18  DHF     Added isSetUp()                                 
///                                                                             
///@version 2018-11-26  KJS     Added channelSpecificDataWord() method that     
///                             returns the first element in m_Body             
///                                                                             
///@version 2018-11-20  KJS     Channel specific data word added as a parameter 
///                             to the Packet constructor                       
///                                                                             
///@version 2018-11-09  KJS     Updated a expected usage for constructor and    
///                             moved constructor documentation to the .cpp     
///                                                                             
///@version 2018-08-31  KJS     Changed constructor to take in data type name,  
///                             packet data, data size, relative time counter,  
///                             and the header checksum                         
///                                                                             
///@version 2018-08-13  KJS     Created expected usage for                      
///                             Packet(data, packetheader) constructor          
///                                                                             
///@version 2016-06-09  MNT     Minor layout improvements. No code changes.     
///                                                                             
///@version 2016-05-24  DHF     channelid_t moved from ch10::ds to irig106::ds  
///                                                                             
///@version 2015-07-07  YBS     Used InstrumentationTime                        
///                                                                             
///@version 2014-06-16  DN      Removed inline setRTC and implemented it in .cpp
///                                                                             
///@version 2014-02-25  DHF     Added const to getChannelSpecificDataWord.      
///                                                                             
///@version 2014-02-25  DHF     Added toString levels.                          
///                                                                             
///@version 2014-02-07  DHF     Added toString for quasi-debug/dump purposes.   
///                                                                             
///@version 2013-11-13  DHF     Added cheapClone (again).                       
///                                                                             
///@version 2013-02-06  DHF     Added setRTC                                    
///                                                                             
///@version 2012-05-04  AGB     Added the timeEncoding method                   
///                                                                             
///@version 2012-04-25  AGB     Added the rtcToAbsolute method                  
///                                                                             
///@version 2011-06-01  DHF     Change m_AfterHeader from char* to uint8_t*     
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-10-28  DHF     Added copy, a deep copy method.                 
///                                                                             
///@version 2009-08-12  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Packet 
{
    public:
        //----------------------------------------------------------------------
        ///@exception InvalidPacketConversion  Types derived from Packet will   
        ///     want to throw this when someone tries to convert a PCM packet   
        ///     to a video packet.                                              
        //----------------------------------------------------------------------
        typedef std::bad_cast   InvalidPacketConversion;

        ds::PacketHeader*           m_Header;           ///< pointer to header. 
        ds::PacketSecondaryHeader*  m_SecondaryHeader;  ///< 2nd header if any  

        lib::time::work::InstrumentationTime   m_AbsoluteTime;     ///< computed absolute time

        uint8_t*           m_AfterHeader;          ///< after primary header    
        uint32_t*          m_Body;                 ///< pointer to package body 
        uint8_t*           m_Checksum8;            ///< ptr to 1-byte checksum  
        uint16_t*          m_Checksum16;           ///< ptr to 2-byte checksum  
        uint32_t*          m_Checksum32;           ///< ptr to 4-byte checksum  

        uint64_t           m_FileOffset;           ///< offset into the file    
        uint64_t           m_FilePacketNumber;     ///< 1 = 1st packet in file  

        uint64_t           m_RTC;                  ///< real-time counter       
        uint64_t           m_OriginalRTC;          ///< original if RTC smooth  

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        Packet();
        
        Packet(
              lib::irig106::ds::DATA_TYPE_NAME dtn
            , void*                            data
            , size_t                           data_size
            , uint64_t                         rtc                        = 0
            , uint32_t*                        checksum                   = 0
            , uint32_t                         channel_specific_data_word = 0
        );
        
        Packet(const ds::PacketHeader& header);
     

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        virtual ~Packet();

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        Packet& operator=(const ds::PacketHeader& header);

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        PacketPtr copy() const;
        
        //----------------------------------------------------------------------
        //  If you call isPacketChecksumValid and then change the underlying    
        //  packet data, you should call verifyPacketChecksum.  If you don't    
        //  change the underlying packet data, isPacketChecksumValid is faster. 
        //----------------------------------------------------------------------
        bool isPacketChecksumValid() const;
        bool verifyPacketChecksum() const;

        bool isHeaderChecksumValid() const;

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        void fixChecksum(bool correctly = true);
        void fixHeaderChecksum(bool correctly = true);

        template <typename CHANNEL_SPECIFIC_DATA_TYPE>
        void getChannelSpecificData(CHANNEL_SPECIFIC_DATA_TYPE& csd) const
        {
            static_assert(sizeof(CHANNEL_SPECIFIC_DATA_TYPE) == 4,"wrong size");
            *((uint32_t*)(&csd)) = m_Body[0];
        }
        uint32_t channelSpecificDataWord() const { return m_Body[0];} 

        lib::time::work::InstrumentationTime rtcToAbsolute(uint64_t rtc) const
        {
            return lib::time::work::InstrumentationTime(
                m_AbsoluteTime + lib::time::work::DeltaTime((rtc - m_RTC)/1e7)
            );
        }

        //----------------------------------------------------------------------
        /// Return the basic type of the data type.                             
        //----------------------------------------------------------------------
        inline irig106::ds::DATA_TYPE_NAME basicDataType() const 
        {
            return irig106::ds::DATA_TYPE_NAME(
                m_Header->m_DataType & irig106::ds::DTN_TYPE_MASK
            );
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a computer generated packet.           
        //----------------------------------------------------------------------
        inline bool isComputerGenerated() const {
            return basicDataType() == irig106::ds::DTN_COMPUTER_GENERATED; 
        }


        //----------------------------------------------------------------------
        ///@brief   ture if hte packet is a setup record.                       
        //----------------------------------------------------------------------
        inline bool isSetUp() const {
            return dataType() == irig106::ds::DTN_SETUP_RECORD;
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a computer generated packet.           
        //----------------------------------------------------------------------
        inline bool isIndex() const {
            return dataType() == irig106::ds::DTN_RECORDING_INDEX; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a PCM packet.                          
        //----------------------------------------------------------------------
        inline bool isPCM() const {
            return basicDataType() == irig106::ds::DTN_PCM; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a time packet.                         
        //----------------------------------------------------------------------
        inline bool isTime() const {
            return basicDataType() == irig106::ds:: DTN_TIME; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a 1553 packet.                         
        //----------------------------------------------------------------------
        inline bool is1553() const {
            return basicDataType() == irig106::ds:: DTN_MIL_STD_1553; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is an analog packet.                      
        //----------------------------------------------------------------------
        inline bool isAnalog() const {
            return basicDataType() == irig106::ds:: DTN_ANALOG; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a discrete packet.                     
        //----------------------------------------------------------------------
        inline bool isDiscrete() const {
            return basicDataType() == irig106::ds:: DTN_DISCRETE; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a message packet.                      
        //----------------------------------------------------------------------
        inline bool isMessage() const {
            return basicDataType() == irig106::ds:: DTN_MESSAGE; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is an ARINC packet.                       
        //----------------------------------------------------------------------
        inline bool isARINC() const {
            return basicDataType() == irig106::ds:: DTN_ARINC_429; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a video packet.                        
        //----------------------------------------------------------------------
        inline bool isVideo() const {
            return basicDataType() == irig106::ds:: DTN_VIDEO; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a UART packet.                         
        //----------------------------------------------------------------------
        inline bool isUART() const {
            return basicDataType() == irig106::ds:: DTN_UART; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a 1394 packet.                         
        //----------------------------------------------------------------------
        inline bool is1394() const {
            return basicDataType() == irig106::ds:: DTN_IEEE_1394; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is a parallel packet.                     
        //----------------------------------------------------------------------
        inline bool isParallel() const {
            return basicDataType() == irig106::ds:: DTN_PARALLEL; 
        }

        //----------------------------------------------------------------------
        /// Return true if the packet is an Ethernet packet.                    
        //----------------------------------------------------------------------
        inline bool isEthernet() const {
            return basicDataType() == irig106::ds:: DTN_ETHERNET; 
        }

        inline uint64_t syncPattern() const  { return m_Header->m_SyncPattern; }
        inline irig106::ds::channelid_t channelID() const
            { return m_Header->m_ChannelID; }
        inline uint32_t packetLength() const { return m_Header->m_PacketLength;}
        inline uint32_t dataLength() const   { return m_Header->m_DataLength; }

        inline uint8_t dataTypeVersion() const 
            { return m_Header->m_DataTypeVersion;}

        inline ds::sequencenumber_t sequenceNumber() const 
            { return m_Header->m_SequenceNumber; }

        uint8_t secondaryHeaderPresent() const 
            { return m_Header->m_SecondaryHeaderPresent; }

        uint8_t packetSecondaryHeaderTime() const 
            { return m_Header->m_PacketSecondaryHeaderTime; }

        uint8_t relativeTimeCounterSyncError() const
            { return m_Header->m_RelativeTimeCounterSyncError; }

        uint8_t dataOverflowError() const 
            { return m_Header->m_DataOverFlowError; }

        uint8_t secondaryTimeHeaderTimeFormat() const 
            { return m_Header->m_SecondaryTimeHeaderTimeFormat; }

        uint8_t checksumType() const { return m_Header->m_ChecksumType; }

        //uint8_t dataType() const { return m_Header->m_DataType; }             

        irig106::ds::DATA_TYPE_NAME dataType() const {
            return irig106::ds::DATA_TYPE_NAME(m_Header->m_DataType);
        }

        uint16_t headerChecksum() const { return m_Header->m_HeaderChecksum; }

        irig106::ds::TIME_ENCODING_SCHEME timeEncoding() const;

        void setRTC(uint64_t r, bool updateOriginal = true);

        ConstPacketPtr cheapClone(
            const lib::time::work::InstrumentationTime& absoluteTime 
          , uint64_t fileOffset          = std::numeric_limits<uint64_t>::max()
          , uint64_t filePacketNumber    = std::numeric_limits<uint64_t>::max()
          , uint64_t rtc                 = std::numeric_limits<uint64_t>::max()
        ) const;

        ConstPacketPtr cheapClone(
            uint64_t fileOffset
          , uint64_t filePacketNumber    = std::numeric_limits<uint64_t>::max()
          , uint64_t rtc                 = std::numeric_limits<uint64_t>::max()
        ) const;

        enum TO_STRING_LEVEL
        {
            TSL_HEADER
          , TSL_EVERYTHING
          , TSL_PARTS                               = 0x80000000
          , TSL_PART_FILE_OFFSET                    = 0x80000001
          , TSL_PART_PACKET_NUMBER                  = 0x80000002
          , TSL_PART_CHANNEL_ID                     = 0x80000004
          , TSL_PART_TYPE                           = 0x80000008

          , TSL_PART_MINIMAL                        = 0x8000000F
        };
        typedef lib::ds::Flags<TO_STRING_LEVEL> StringLevels;
        std::string toString(StringLevels level = TSL_PART_MINIMAL) const;

    protected:
        void handleWrongTypeError();
        virtual bool correctDataType() const;

    private:
        //----------------------------------------------------------------------
        //  Don't implement these.  They will be overly expensive or lead to    
        //  bugs.                                                               
        //----------------------------------------------------------------------
        Packet(const Packet& packet) = default;
        Packet& operator=(const Packet& that);

        lib::ds::shared_ptr<std::vector<uint8_t>>   m_Memory;
//        boost::shared_array<uint8_t>   m_Memory;

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        void construct(const ds::PacketHeader& header);

        //----------------------------------------------------------------------
        //  These values are used to speed up "isValid" methods.  They really   
        //  do not represent the state of the object and therefore are mutable. 
        //----------------------------------------------------------------------
        mutable bool    m_PacketChecksumChecked;
        mutable bool    m_PacketChecksumValid;

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        std::string toStringBodyEverything() const;
        std::string toStringDefaultEverything() const;
        std::string toStringHeaderEverything(bool oneLine = false) const;
        std::string toStringIndexEverything() const;

}; // class Packet //                                                           

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

}  // namespace work //                                                         
}  // namespace ch10 //                                                         
}  // namespace irig106 //                                                      
}  // namespace lib //                                                          

#endif // #ifndef LIB_IRIG106_CH10_WORK_PACKET_H //                             
