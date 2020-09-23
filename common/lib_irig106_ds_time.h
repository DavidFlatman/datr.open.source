//------------------------------------------------------------------------------
///@file lib_irig106_ds_time.h                                                  
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------

#ifndef LIB_IRIG106_DS_TIME_H_FILE_GUARD
#define LIB_IRIG106_DS_TIME_H_FILE_GUARD

#include <boost/scoped_ptr.hpp>
#include <boost/static_assert.hpp>
#include <iostream>
#include <stdint.h>

namespace lib {
namespace irig106 {
namespace ds {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Time                                                             
///                                                                             
///@brief   Data structures relating to IRIG106 time values.                    
///                                                                             
///@par Thread Safety:  unknown none class object                               
///                                                                             
///@version 2012-04-25  AGB     Added functions to convert TIME_ENCODING_SCHEME 
///                             values to and from text.                        
///                                                                             
///@version 2012-03-26  DHF     File creation based on Andy's work.             
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Time Encoding Schemes                                                       
//------------------------------------------------------------------------------
enum TIME_ENCODING_SCHEME {
      TES_CH4_BINARY
    , TES_CH4_BCD
    , TES_CH10_DAY
    , TES_CH10_DMY
    , TES_CH10_RELATIVE
    , TES_CH10_IEEE1588
    , TES_UNDEFINED
};

std::ostream& operator<<(std::ostream& out, TIME_ENCODING_SCHEME e);
std::istream& operator>>(std::istream& in, TIME_ENCODING_SCHEME& e);

//------------------------------------------------------------------------------
///@brief   Chapter 4 BCD high order time bit layout.                           
//------------------------------------------------------------------------------
union CH4_BCD_HOT {
   uint16_t m_Word;
   struct {
       #ifdef BIT_FIELD_HIGH_BIT_FIRST
           uint16_t m_OnesDay:3;
           uint16_t m_TensHour:2;
           uint16_t m_OnesHour:4;
           uint16_t m_TensMin:3;
           uint16_t m_OnesMin:4;
       #else
           uint16_t m_OnesMin:4;
           uint16_t m_TensMin:3;
           uint16_t m_OnesHour:4;
           uint16_t m_TensHour:2;
           uint16_t m_OnesDay:3;
        #endif
   };
};
BOOST_STATIC_ASSERT(sizeof(CH4_BCD_HOT) == 2);

//------------------------------------------------------------------------------
///@brief   Chapter 4 BCD low order time bit layout.                            
//------------------------------------------------------------------------------
union CH4_BCD_LOT {
    uint16_t m_Word;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint16_t m_Unused:1;
            uint16_t m_TensSec:3;
            uint16_t m_OnesSec:4;
            uint16_t m_HundMilli:4;
            uint16_t m_TensMilli:4;
        #else
            uint16_t m_TensMilli:4;
            uint16_t m_HundMilli:4;
            uint16_t m_OnesSec:4;
            uint16_t m_TensSec:3;
            uint16_t m_Unused:1;
        #endif
    };
};
BOOST_STATIC_ASSERT(sizeof(CH4_BCD_LOT) == 2);

//------------------------------------------------------------------------------
///@brief   10.6.3.2.b  (See Figure 10-23)  Time data packet format, day format 
///         upper 16-bit word bit layout.                                       
//------------------------------------------------------------------------------
union CH10_DAY_HOT {
    uint16_t m_Word;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint16_t m_Unused:6;
            uint16_t m_HundDay:2;
            uint16_t m_TensDay:4;
            uint16_t m_OnesDay:4;
        #else
            uint16_t m_OnesDay:4;
            uint16_t m_TensDay:4;
            uint16_t m_HundDay:2;
            uint16_t m_Unused:6;
        #endif
    };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DAY_HOT) == 2);

//------------------------------------------------------------------------------
///@brief   10.6.3.2.b  (See Figure 10-23)  Time data packet format, day format 
///         middle 16-bit word bit layout.                                      
//------------------------------------------------------------------------------
union CH10_DAY_LOT {
   uint16_t m_Word;
   struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
           uint16_t m_Unused2:2;
           uint16_t m_TensHour:2;
           uint16_t m_OnesHour:4;
           uint16_t m_Unused:1;
           uint16_t m_TensMin:3;
           uint16_t m_OnesMin:4;
       #else
           uint16_t m_OnesMin:4;
           uint16_t m_TensMin:3;
           uint16_t m_Unused:1;
           uint16_t m_OnesHour:4;
           uint16_t m_TensHour:2;
           uint16_t m_Unused2:2;
       #endif
   };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DAY_LOT) == 2);

//------------------------------------------------------------------------------
///@brief   10.6.3.2.b  (See Figure 10-23)  Time data packet format, day format 
///         least significant 16-bit word bit layout.                           
//------------------------------------------------------------------------------
union CH10_DAY_MOT {
   uint16_t m_Word;
   struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
           uint16_t m_TensSec:4;
           uint16_t m_OnesSec:4;
           uint16_t m_HundMilli:4;
           uint16_t m_TensMilli:4;

        #else
           uint16_t m_TensMilli:4;
           uint16_t m_HundMilli:4;
           uint16_t m_OnesSec:4;
           uint16_t m_TensSec:4;
        #endif
   };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DAY_MOT) == 2);

//------------------------------------------------------------------------------
///@brief   An object representing the upper 32-bits of an IRIG-106, Chapter 10 
///         time word in the day-month-year format.                             
//------------------------------------------------------------------------------
union CH10_DMY_HIGH_WORD {
    uint32_t m_Word;
    struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
            uint32_t m_Unused2:2;
            uint32_t m_ThouYear:2;
            uint32_t m_HundYear:4;
            uint32_t m_TensYear:4;
            uint32_t m_OnesYear:4;
            uint32_t m_Unused:3;
            uint32_t m_TensMonth:1;
            uint32_t m_OnesMonth:4;
            uint32_t m_TensDay:4;
            uint32_t m_OnesDay:4;

        #else
            uint32_t m_OnesDay:4;
            uint32_t m_TensDay:4;
            uint32_t m_OnesMonth:4;
            uint32_t m_TensMonth:1;
            uint32_t m_Unused:3;
            uint32_t m_OnesYear:4;
            uint32_t m_TensYear:4;
            uint32_t m_HundYear:4;
            uint32_t m_ThouYear:2;
            uint32_t m_Unused2:2;

        #endif
    };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DMY_HIGH_WORD) == 4);

//------------------------------------------------------------------------------
///@brief   An object representing the lower 32-bits of an IRIG-106, Chapter 10 
///         time word in the day-month-year format.                             
//------------------------------------------------------------------------------
union CH10_DMY_LOW_WORD {
   uint32_t m_Word;
   struct {
        #ifdef BIT_FIELD_HIGH_BIT_FIRST
           uint32_t m_Unused2:2;
           uint32_t m_TensHour:2;
           uint32_t m_OnesHour:4;
           uint32_t m_Unused:1;
           uint32_t m_TensMin:3;
           uint32_t m_OnesMin:4;
           uint32_t m_TensSec:4;
           uint32_t m_OnesSec:4;
           uint32_t m_HundMilli:4;
           uint32_t m_TensMilli:4;

       #else
           uint32_t m_TensMilli:4;
           uint32_t m_HundMilli:4;
           uint32_t m_OnesSec:4;
           uint32_t m_TensSec:4;
           uint32_t m_OnesMin:4;
           uint32_t m_TensMin:3;
           uint32_t m_Unused:1;
           uint32_t m_OnesHour:4;
           uint32_t m_TensHour:2;
           uint32_t m_Unused2:2;

       #endif
   };
};
BOOST_STATIC_ASSERT(sizeof(CH10_DMY_LOW_WORD) == 4);

} // namespace ds
} // namespace irig106
} // namespace lib

#endif // #ifndef LIB_IRIG106_DS_TIME_H_FILE_GUARD

