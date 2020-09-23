#ifndef LIB_IRIG106_WORK_TIME_H_FILE_GUARD
#define LIB_IRIG106_WORK_TIME_H_FILE_GUARD

//------------------------------------------------------------------------------
///@file lib_irig106_work_time.h                                                
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
///                                                                             
///@brief Time conversion functions for IRIG 106 time encoding schemes.         
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

#if 0   /// @warning  Do we need these?
void toCh10DMY(
    const lib::time::work::InstrumentationTime& time
  , uint32_t* pHw
  , uint32_t* pLw
);

void toCh10DMY(
    const lib::time::work::InstrumentationTime& time
  , uint64_t* i64
);
#endif

void toIEEE1588(
    const lib::time::work::InstrumentationTime& time
  , uint32_t* pSec
  , uint32_t* pNsec
);

#if 0
union CH4_BCD_HOT {
   uint16_t hot;
   struct {
       uint16_t m_onesMin:4;
       uint16_t m_tensMin:3;
       uint16_t m_onesHour:4;
       uint16_t m_tensHour:2;
       uint16_t m_onesDay:3;
   };
};
BOOST_STATIC_ASSERT(sizeof(CH4_BCD_HOT) == 2);

union CH4_BCD_LOT {
    uint16_t lot;
    struct {                                                                                                          Milli:4;
        uint16_t m_hundMilli:4;
        uint16_t m_onesSec:4;
        uint16_t m_tensSec:3;
        uint16_t m_unused:1;
    };
};
BOOST_STATIC_ASSERT(sizeof(CH4_BCD_LOT) == 2);

union CH10_DAY_HOT {
    uint16_t hot;
    struct {
        uint16_t m_onesDay:4;
        uint16_t m_tensDay:4;
        uint16_t m_hundDay:2;
        uint16_t m_unused:6;
    };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DAY_HOT) == 2);

union CH10_DAY_LOT {
   uint16_t lot;
   struct {
       uint16_t m_onesMin:4;
       uint16_t m_tensMin:3;
       uint16_t m_unused:1;
       uint16_t m_onesHour:4;
       uint16_t m_tensHour:2;
       uint16_t m_unused2:2;
   };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DAY_LOT) == 2);

union CH10_DAY_MOT {
   uint16_t mot;
   struct {
       uint16_t m_tensMilli:4;
       uint16_t m_hundMilli:4;
       uint16_t m_onesSec:4;
       uint16_t m_tensSec:4;
   };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DAY_MOT) == 2);

union CH10_DMY_HIGH_WORD {
    uint32_t hw;
    struct {
        uint32_t m_onesDay:4;
        uint32_t m_tensDay:4;
        uint32_t m_onesMonth:4;
        uint32_t m_tensMonth:1;
        uint32_t m_unused:3;
        uint32_t m_onesYear:4;
        uint32_t m_tensYear:4;
        uint32_t m_hundYear:4;
        uint32_t m_thouYear:2;
        uint32_t m_unused2:2;
    };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DMY_HIGH_WORD) == 4);

union CH10_DMY_LOW_WORD {
   uint32_t lw;
   struct {
       uint32_t m_tensMilli:4;
       uint32_t m_hundMilli:4;
       uint32_t m_onesSec:4;
       uint32_t m_tensSec:4;
       uint32_t m_onesMin:4;
       uint32_t m_tensMin:3;
       uint32_t m_unused:1;
       uint32_t m_onesHour:4;
       uint32_t m_tensHour:2;
       uint32_t m_unused2:2;
   };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DMY_LOW_WORD) == 4);
#endif

} // namespace time
} // namespace work
} // namespace irig106
} // namespace lib

#endif // #ifndef LIB_IRIG106_WORK_TIME_H_FILE_GUARD

