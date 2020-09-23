#ifndef LIB_IRIG106_DS_H
#define LIB_IRIG106_DS_H
//------------------------------------------------------------------------------
///UNCLASSIFIED, OPEN SOURCE                                                                             
///@file lib_irig106_ds.h                                                       
///                                                                             
///@brief   Hold the data structures (ds) for IRIG-106 common between chapters. 
///                                                                             ///                                                                             
///@version 2014-01-08  JAH     ChannelInfo moved to lib_irig106_ch9_ds.h       
///                                                                             
///@version 2012-??-??  AGB     Time encoding enum moved to lib_irig106_ds_time 
///                                                                             
///@version 2012-03-28  AGB     Added an enum documenting time encoding schemes 
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-10-28  DHF     File creation from ...ch10_ds.h when ch9_ds.h   
///                             needs the same info.                            
//------------------------------------------------------------------------------

#include <set>
#include <string>
#include <stdint.h>

namespace lib {
namespace irig106 {
namespace ds {

//------------------------------------------------------------------------------
/// See Table 10-7 of RCC Document 106-09, Telemetry Standard, April 2009       
//------------------------------------------------------------------------------
enum DATA_TYPE_NAME {
    DTN_COMPUTER_GENERATED                  = 0x00
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_0    = 0x00
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_1
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_2
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_3
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_4
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_5
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_6
  , DTN_COMPUTER_GENERATED_DATA_FORMAT_7

  , DTN_PCM                                 = 0x08
  , DTN_PCM_DATA_FORMAT_0                   = 0x08
  , DTN_PCM_DATA_FORMAT_1
  , DTN_PCM_DATA_FORMAT_2
  , DTN_PCM_DATA_FORMAT_3
  , DTN_PCM_DATA_FORMAT_4
  , DTN_PCM_DATA_FORMAT_5
  , DTN_PCM_DATA_FORMAT_6
  , DTN_PCM_DATA_FORMAT_7

  , DTN_TIME                                = 0x10
  , DTN_TIME_DATA_FORMAT_0                  = 0x10
  , DTN_TIME_DATA_FORMAT_1
  , DTN_TIME_DATA_FORMAT_2
  , DTN_TIME_DATA_FORMAT_3
  , DTN_TIME_DATA_FORMAT_4
  , DTN_TIME_DATA_FORMAT_5
  , DTN_TIME_DATA_FORMAT_6
  , DTN_TIME_DATA_FORMAT_7

  , DTN_MIL_STD_1553                        = 0x18
  , DTN_MIL_STD_1553_DATA_FORMAT_0          = 0x18
  , DTN_MIL_STD_1553_DATA_FORMAT_1
  , DTN_MIL_STD_1553_DATA_FORMAT_2
  , DTN_MIL_STD_1553_DATA_FORMAT_3
  , DTN_MIL_STD_1553_DATA_FORMAT_4
  , DTN_MIL_STD_1553_DATA_FORMAT_5
  , DTN_MIL_STD_1553_DATA_FORMAT_6
  , DTN_MIL_STD_1553_DATA_FORMAT_7

  , DTN_ANALOG                              = 0x20
  , DTN_ANALOG_DATA_FORMAT_0                = 0x20
  , DTN_ANALOG_DATA_FORMAT_1
  , DTN_ANALOG_DATA_FORMAT_2
  , DTN_ANALOG_DATA_FORMAT_3
  , DTN_ANALOG_DATA_FORMAT_4
  , DTN_ANALOG_DATA_FORMAT_5
  , DTN_ANALOG_DATA_FORMAT_6
  , DTN_ANALOG_DATA_FORMAT_7

  , DTN_DISCRETE                            = 0x28
  , DTN_DISCRETE_DATA_FORMAT_0              = 0x28
  , DTN_DISCRETE_DATA_FORMAT_1
  , DTN_DISCRETE_DATA_FORMAT_2
  , DTN_DISCRETE_DATA_FORMAT_3
  , DTN_DISCRETE_DATA_FORMAT_4
  , DTN_DISCRETE_DATA_FORMAT_5
  , DTN_DISCRETE_DATA_FORMAT_6
  , DTN_DISCRETE_DATA_FORMAT_7

  , DTN_MESSAGE                             = 0x30
  , DTN_MESSAGE_DATA_FORMAT_0               = 0x30
  , DTN_MESSAGE_DATA_FORMAT_1
  , DTN_MESSAGE_DATA_FORMAT_2
  , DTN_MESSAGE_DATA_FORMAT_3
  , DTN_MESSAGE_DATA_FORMAT_4
  , DTN_MESSAGE_DATA_FORMAT_5
  , DTN_MESSAGE_DATA_FORMAT_6
  , DTN_MESSAGE_DATA_FORMAT_7

  , DTN_ARINC_429                           = 0x38
  , DTN_ARINC_429_DATA_FORMAT_0             = 0x38
  , DTN_ARINC_429_DATA_FORMAT_1
  , DTN_ARINC_429_DATA_FORMAT_2
  , DTN_ARINC_429_DATA_FORMAT_3
  , DTN_ARINC_429_DATA_FORMAT_4
  , DTN_ARINC_429_DATA_FORMAT_5
  , DTN_ARINC_429_DATA_FORMAT_6
  , DTN_ARINC_429_DATA_FORMAT_7

  , DTN_VIDEO                               = 0x40
  , DTN_VIDEO_DATA_FORMAT_0                 = 0x40
  , DTN_VIDEO_DATA_FORMAT_1
  , DTN_VIDEO_DATA_FORMAT_2
  , DTN_VIDEO_DATA_FORMAT_3
  , DTN_VIDEO_DATA_FORMAT_4
  , DTN_VIDEO_DATA_FORMAT_5
  , DTN_VIDEO_DATA_FORMAT_6
  , DTN_VIDEO_DATA_FORMAT_7

  , DTN_IMAGE                               = 0x48
  , DTN_IMAGE_DATA_FORMAT_0                 = 0x48
  , DTN_IMAGE_DATA_FORMAT_1
  , DTN_IMAGE_DATA_FORMAT_2
  , DTN_IMAGE_DATA_FORMAT_3
  , DTN_IMAGE_DATA_FORMAT_4
  , DTN_IMAGE_DATA_FORMAT_5
  , DTN_IMAGE_DATA_FORMAT_6
  , DTN_IMAGE_DATA_FORMAT_7

  , DTN_UART                                = 0x50
  , DTN_UART_DATA_FORMAT_0                  = 0x50
  , DTN_UART_DATA_FORMAT_1
  , DTN_UART_DATA_FORMAT_2
  , DTN_UART_DATA_FORMAT_3
  , DTN_UART_DATA_FORMAT_4
  , DTN_UART_DATA_FORMAT_5
  , DTN_UART_DATA_FORMAT_6
  , DTN_UART_DATA_FORMAT_7

  , DTN_IEEE_1394                           = 0x58
  , DTN_IEEE_1394_DATA_FORMAT_1
  , DTN_IEEE_1394_DATA_FORMAT_2
  , DTN_IEEE_1394_DATA_FORMAT_3
  , DTN_IEEE_1394_DATA_FORMAT_4
  , DTN_IEEE_1394_DATA_FORMAT_5
  , DTN_IEEE_1394_DATA_FORMAT_6
  , DTN_IEEE_1394_DATA_FORMAT_7

  , DTN_PARALLEL                            = 0x60
  , DTN_PARALLEL_DATA_FORMAT_0              = 0x60
  , DTN_PARALLEL_DATA_FORMAT_1
  , DTN_PARALLEL_DATA_FORMAT_2
  , DTN_PARALLEL_DATA_FORMAT_3
  , DTN_PARALLEL_DATA_FORMAT_4
  , DTN_PARALLEL_DATA_FORMAT_5
  , DTN_PARALLEL_DATA_FORMAT_6
  , DTN_PARALLEL_DATA_FORMAT_7

  , DTN_ETHERNET                            = 0x68
  , DTN_ETHERNET_DATA_FORMAT_0              = 0x68
  , DTN_ETHERNET_DATA_FORMAT_1
  , DTN_ETHERNET_DATA_FORMAT_2
  , DTN_ETHERNET_DATA_FORMAT_3
  , DTN_ETHERNET_DATA_FORMAT_4
  , DTN_ETHERNET_DATA_FORMAT_5
  , DTN_ETHERNET_DATA_FORMAT_6
  , DTN_ETHERNET_DATA_FORMAT_7

  //----------------------------------------------------------------------------
  // Aliases                                                                    
  //----------------------------------------------------------------------------
  , DTN_USER_DEFINED                    = DTN_COMPUTER_GENERATED_DATA_FORMAT_0
  , DTN_SETUP_RECORD                    = DTN_COMPUTER_GENERATED_DATA_FORMAT_1
  , DTN_RECORDING_EVENTS                = DTN_COMPUTER_GENERATED_DATA_FORMAT_2
  , DTN_RECORDING_INDEX                 = DTN_COMPUTER_GENERATED_DATA_FORMAT_3

  , DTN_PCM_IRIG106_4_8                 = DTN_PCM_DATA_FORMAT_1

  , DTN_TIME_IRIG                       = DTN_TIME_DATA_FORMAT_1
  , DTN_TIME_GPC                        = DTN_TIME_DATA_FORMAT_2
  , DTN_TIME_RTC                        = DTN_TIME_DATA_FORMAT_3

  , DTN_1553B                           = DTN_MIL_STD_1553_DATA_FORMAT_1
  , DTN_16PP194_BUS                     = DTN_MIL_STD_1553_DATA_FORMAT_2

  , DTN_ANALOG_DATA                     = DTN_ANALOG_DATA_FORMAT_1

  , DTN_DISCRETE_DATA                   = DTN_DISCRETE_DATA_FORMAT_1

  , DTN_GENERIC_MESSAGE                 = DTN_MESSAGE_DATA_FORMAT_0

  , DTN_ARINC_429_DATA                  = DTN_ARINC_429_DATA_FORMAT_0

  , DTN_VIDEO_MPEG2                     = DTN_VIDEO_DATA_FORMAT_0
  , DTN_VIDEO_H264                      = DTN_VIDEO_DATA_FORMAT_0
  , DTN_VIDEO_ISSO13818                 = DTN_VIDEO_DATA_FORMAT_1
  , DTN_VIDEO_MPEG4                     = DTN_VIDEO_DATA_FORMAT_2
  , DTN_VIDEO_AVC_H264                  = DTN_VIDEO_DATA_FORMAT_2

  , DTN_TYPE_MASK                       = 0xF8
  , DTN_ERROR                           = 0xFF

}; // enum DATA_TYPE_NAME //
std::string toString(DATA_TYPE_NAME dtn);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
enum RecordingTechnique {
    RT_FMFM
  , RT_HDDR
  , RT_PRE_D
  , RT_DIRECT
  , RT_FMWBI
  , RT_FMWBII
  , RT_FM_IM
  , RT_FM_NB
  , RT_DOUBEN
  , RT_RO_K
  , RT_RO_MUX
  , RT_SSR
  , RT_OTHER
};

enum DataDirection {
    DD_FWD
  , DD_REV
};

typedef uint16_t    channelid_t;
typedef std::set<channelid_t>  ChannelSet;

} // namespace ds //
} // namespace irig106 //
} // namespace lib //

#endif // #ifndef LIB_IRIG106_DS_H
