#ifndef LIB_IRIG106_WORK_TIME_H_FILE_GUARD
#define LIB_IRIG106_WORK_TIME_H_FILE_GUARD

//------------------------------------------------------------------------------
///                                                                             
///@file lib_irig106_work_time.h                                                
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@brief Time conversion functions for IRIG 106 time encoding schemes.         
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2015-07-08  MNT     Replaced DateTime by InstrumentationTime        
///                                                                             
///@version 2012-03-02  AGB     Initial release.                                
///                                                                             
//------------------------------------------------------------------------------

#include <stdint.h>

#include <boost/static_assert.hpp>

#include "lib_time_work_instrumentationtime.h"
#include "lib_irig106_ds.h"

namespace lib {
namespace irig106 {
namespace work {
namespace time {

lib::time::work::InstrumentationTime fromCh4BCD(
    uint16_t hot
  , uint16_t lot
  , uint16_t mot
);

lib::time::work::InstrumentationTime fromCh10Day(
    uint16_t hot
  , uint16_t lot
  , uint16_t mot
);

lib::time::work::InstrumentationTime fromCh10DMY(
    uint32_t hw
  , uint32_t lw
);

lib::time::work::InstrumentationTime fromIEEE1588(
    uint32_t sec
  , uint32_t nsec
);

void toCh4Binary(
    const lib::time::work::InstrumentationTime& time
  , uint16_t* pHot
  , uint16_t* pLot
  , uint16_t* pMot
);

void toCh4BCD(
    const lib::time::work::InstrumentationTime& time
  , uint16_t* pHot
  , uint16_t* pLot
  , uint16_t* pMot);
  
void toCh10Day(
    const lib::time::work::InstrumentationTime& time
  , uint16_t* pHot
  , uint16_t* pLot
  , uint16_t* pMot
);

void toCh10Day(
    const lib::time::work::InstrumentationTime& time
  , uint64_t* i64
);

void toIEEE1588(
    const lib::time::work::InstrumentationTime& time
  , uint32_t* pSec
  , uint32_t* pNsec
);


} // namespace time
} // namespace work
} // namespace irig106
} // namespace lib

#endif // #ifndef LIB_IRIG106_WORK_TIME_H_FILE_GUARD

