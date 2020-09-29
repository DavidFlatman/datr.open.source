#include "lib_irig106_ch10_ds.h"
#include "lib_si_prefixes.h"
//------------------------------------------------------------------------------
///                                                                             
///@file lib_irig106_ch10_ds.cpp                                                
///                                                                             
///@par Classification:  UNCLASSIFED, OPEN SOURCE                               
///                                                                             
///@version 2020-09-21  DHF Made open source.                                   
///                                                                             
///@version 2014-09-09  YBS Added MaximumDataPerPacketMilliseconds              
///                                                                             
///@version 2014-08-12  DHF Added file author/version information.              
///                         Added MaximumPacketDurationSeconds                  
///                         Added MaximumPacketWriteDelaySeconds                
///                                                                             
//------------------------------------------------------------------------------
namespace lib {
namespace irig106 {
namespace ch10 {
namespace ds {

const uint16_t      SYNC_PATTERN(0xEB25);
const double MaximumPacketDurationSeconds(1e-1);

uint32_t RTC_FREQUENCY(10000000);

//------------------------------------------------------------------------------
///@brief   Specifies 100 milliseconds worth of data in a ch10 packet           
//------------------------------------------------------------------------------
const uint64_t MaximumDataPerPacketMilliseconds(MaximumPacketDurationSeconds * 1000); //'int' to be compatible

}  // namespace ds //
}  // namespace ch10 //
}  // namespace irig106 //
}  // namespace lib //

