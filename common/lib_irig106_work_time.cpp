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
///@version 2014-09-25  DHF     Removed jsecondsToYseconds in fromCh4Binary     
///                             and equivalent in toCh4Binary.                  
///                                                                             
///@version 2012-03-02  AGB     Initial release.                                
///                                                                             
//------------------------------------------------------------------------------

#include "lib_irig106_work_time.h"
#include "lib_irig106_ds_time.h"
#include "lib_si_prefixes.h"
#include "lib_time_ds.h"
#include <math.h>

#include "lib_math_himath.h"

#include "debug.h"

namespace lib {
namespace irig106 {
namespace work {
namespace time {

//------------------------------------------------------------------------------
//  Julidan day has an index of 1 (January1st.julian_day == 1)                  
//  yda()       has an index of 0 (January1st.yday       == 0)                  
//------------------------------------------------------------------------------
int64_t jdayToYday(int64_t x)
{
    return x - 1;
}

double ysecondsToJseconds(double x)
{
    double result =  x + lib::time::ds::secondsPerDay;
    if (result > 365.0 * lib::time::ds::secondsPerDay) {
        result -= lib::time::ds::secondsPerYear;
    }
    return result;
}

double jsecondsToYseconds(double x)
{
    double result = x - lib::time::ds::secondsPerDay;

    if (result < 0) {
        result += 365.0 * lib::time::ds::secondsPerDay;
    }

    return result;

}

//------------------------------------------------------------------------------
///@brief Converts a 3-word Ch. 4 Binary time to a InstrumentationTime          
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime fromCh4Binary(
    int16_t hot
  , uint16_t lot
  , uint16_t mot
  )
{
    lib::time::work::InstrumentationTime ans(
//        jsecondsToYseconds(hot * 655.36 + lot * 0.01 + mot * 1e-6)            
        double(hot) * 655.36 + double(lot) * 0.01 + double(mot) * 1e-6
    );
    return ans;
}

//------------------------------------------------------------------------------
///@brief Converts a 3-word Ch. 4 BCD time to a InstrumentationTime             
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime fromCh4BCD(
    uint16_t hot
  , uint16_t lot
  , uint16_t mot
)
{
    ds::CH4_BCD_HOT hotWord;
    ds::CH4_BCD_LOT lotWord;
    double seconds;

    hotWord.m_Word = hot;
    lotWord.m_Word = lot;

    int jday = hotWord.m_OnesDay;
    int yday = (jday + 9) % 10;     // substract 1; roll over to 9 if negative  

//    seconds =  hotWord.m_OnesDay  * lib::time::ds::secondsPerDay;             
    seconds  =               yday * lib::time::ds::secondsPerDay;
    seconds += hotWord.m_TensHour * lib::time::ds::secondsPerHour * 10;
    seconds += hotWord.m_OnesHour * lib::time::ds::secondsPerHour;
    seconds += hotWord.m_TensMin  * lib::time::ds::secondsPerMinute *10;
    seconds += hotWord.m_OnesMin  * lib::time::ds::secondsPerMinute;

    seconds += lotWord.m_TensSec * 10.0;
    seconds += lotWord.m_OnesSec;
    seconds += lotWord.m_HundMilli * 0.1;
    seconds += lotWord.m_TensMilli * 0.01;

    seconds += mot * 1e-6;

    return lib::time::work::InstrumentationTime(seconds);

}

//------------------------------------------------------------------------------
///@brief Converts a 3-word Ch. 10 "day format" time to a InstrumentationTime   
///                                                                             
/// Note well:  hot is the word that contains the hundreds, tens, and ones of   
/// days field, lot is the word that contains the hours and minutes fields, and 
/// mot is the word that contains the seconds and fractions of seconds fields.  
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime fromCh10Day(
    uint16_t hot
  , uint16_t lot
  , uint16_t mot
)
{
    ds::CH10_DAY_HOT hotWord;
    ds::CH10_DAY_LOT lotWord;
    ds::CH10_DAY_MOT motWord;

    double seconds;

    hotWord.m_Word = hot;
    lotWord.m_Word = lot;
    motWord.m_Word = mot;

    seconds =  hotWord.m_HundDay * lib::time::ds::secondsPerDay * 100;
    seconds += hotWord.m_TensDay * lib::time::ds::secondsPerDay *  10;
    seconds += hotWord.m_OnesDay * lib::time::ds::secondsPerDay *   1;

    seconds += lotWord.m_TensHour * lib::time::ds::secondsPerHour * 10;
    seconds += lotWord.m_OnesHour * lib::time::ds::secondsPerHour *  1;

    seconds += lotWord.m_TensMin  * lib::time::ds::secondsPerMinute * 10;
    seconds += lotWord.m_OnesMin  * lib::time::ds::secondsPerMinute *  1;

    seconds += motWord.m_TensSec * 10.0;
    seconds += motWord.m_OnesSec;
    seconds += motWord.m_HundMilli * 0.1;
    seconds += motWord.m_TensMilli * 0.01;

    //--------------------------------------------------------------------------
    //  Convert jday to yday                                                    
    //--------------------------------------------------------------------------
    return lib::time::work::InstrumentationTime(jsecondsToYseconds(seconds));
}

//------------------------------------------------------------------------------
///@brief Converts a 2-long-word Ch. 10 "day month year" format time to a       
///     InstrumentationTime                                                     
///                                                                             
/// The means by which this function identifies leap years will be incorrect in 
/// the year 2100.  By then, I will have retired, died, and will probably no    
/// longer care whether or not this code works.                                 
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime fromCh10DMY(uint32_t hw, uint32_t lw)
{
    int year, jday;
    uint64_t nano;

    ds::CH10_DMY_LOW_WORD lowWord;
    ds::CH10_DMY_HIGH_WORD highWord;

    BOOST_STATIC_ASSERT(sizeof(highWord) == 4);

    lowWord.m_Word  = lw;
    highWord.m_Word = hw;

    year = highWord.m_ThouYear*1000 + highWord.m_HundYear*100 
         + highWord.m_TensYear*10 + highWord.m_OnesYear;
    jday = math::perpCalendar( highWord.m_TensMonth*10 + highWord.m_OnesMonth
                             , highWord.m_TensDay*10 + highWord.m_OnesDay
                             , year%4 == 0
                             );
    nano = lowWord.m_HundMilli*1e8 + lowWord.m_TensMilli*1e7;

    lib::time::work::DateTime dt;
    dt.set( year
           , jdayToYday(jday)
           , lowWord.m_TensHour*10 + lowWord.m_OnesHour
           , lowWord.m_TensMin*10 + lowWord.m_OnesMin
           , lowWord.m_TensSec*10 + lowWord.m_OnesSec
           , nano
           );

    lib::time::work::InstrumentationTime ans(dt);

    return ans;
}

//------------------------------------------------------------------------------
///@brief   Converts Ch. 10's implementation of an IEEE 1558 time to            
///         InstrumentationTime                                                 
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime fromIEEE1588(uint32_t sec, uint32_t nsec)
{
    lib::time::work::InstrumentationTime result;

    result.tv_sec = sec;
    result.tv_nsec = nsec;

    return result;

}

//------------------------------------------------------------------------------
//  Function:  toCh4BCD                                                         
//------------------------------------------------------------------------------
void toCh4BCD(
    const lib::time::work::InstrumentationTime& time
  , uint16_t* pHot
  , uint16_t* pLot
  , uint16_t* pMot
)
{
    ds::CH4_BCD_HOT hotWord;
    ds::CH4_BCD_LOT lotWord;

    uint32_t hour = time.hour();
    uint32_t min = time.min();

    //int jday = ydayToJday(time.yday());                                       
    int jday = time.jday();

    hotWord.m_OnesDay  = jday%10;

    hotWord.m_TensHour = hour / 10;
    hotWord.m_OnesHour = hour % 10;

    hotWord.m_TensMin  = min  / 10;
    hotWord.m_OnesMin  = min  % 10;

    *pHot = hotWord.m_Word;

    uint32_t sec = time.sec();
    uint32_t milli = time.millisec();

    lotWord.m_Unused = 0;

    lotWord.m_TensSec = sec / 10;
    lotWord.m_OnesSec = sec % 10;

    lotWord.m_HundMilli = milli /100;
    lotWord.m_TensMilli = (milli%100)/10;

    *pLot = lotWord.m_Word;
    
    *pMot = time.microsec() - lotWord.m_TensMilli*1e4 - lotWord.m_HundMilli*1e5;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void toCh10Day(const lib::time::work::InstrumentationTime& time, uint64_t* i64)
{
    uint16_t* i16 = (uint16_t*) i64;
    *i64 = 0;
    toCh10Day(time, &i16[2], &i16[1], &i16[0]);
}

//------------------------------------------------------------------------------
//  Function:  toCh10Day                                                        
//------------------------------------------------------------------------------
void toCh10Day(
    const lib::time::work::InstrumentationTime& time
  , uint16_t *pHot
  , uint16_t *pLot
  , uint16_t *pMot
)
{
    ds::CH10_DAY_HOT hotWord;
    ds::CH10_DAY_LOT lotWord;
    ds::CH10_DAY_MOT motWord;

    //--------------------------------------------------------------------------
    //  time.yday() returns the number of days since January 1st (with 0 being  
    //  January 1st).  What we're looking for is the Julian Day which has       
    //  January 1st as 1 (versus 0).                                            
    //--------------------------------------------------------------------------
    //uint32_t jday = ydayToJday(time.yday());                                  
    uint32_t jday = time.jday();

    hotWord.m_Unused = 0;
    hotWord.m_HundDay = jday / 100;
    hotWord.m_TensDay = (jday % 100) / 10;
    hotWord.m_OnesDay = jday % 10;
    *pHot = hotWord.m_Word;

    uint32_t hour = time.hour();
    uint32_t min  = time.min();

    lotWord.m_Unused2 = 0;
    lotWord.m_TensHour = hour / 10;
    lotWord.m_OnesHour = hour % 10;
    lotWord.m_Unused = 0;
    lotWord.m_TensMin = min / 10;
    lotWord.m_OnesMin = min % 10;
    *pLot = lotWord.m_Word;

    uint32_t sec = time.sec();
    uint32_t mil = time.millisec();

    motWord.m_TensSec = sec / 10;
    motWord.m_OnesSec = sec % 10;
    motWord.m_HundMilli = mil / 100;
    motWord.m_TensMilli = (mil % 100) / 10;
    *pMot = motWord.m_Word;
}

//------------------------------------------------------------------------------
//  Function:  toCh10DMY                                                        
//------------------------------------------------------------------------------
#if 0   //  SEE IF WE NEED THIS  @warning @warning @warning                     
void toCh10DMY(const lib::time::work::InstrumentationTime& time, uint64_t* i64)
{
    uint32_t* i32 = (uint32_t*) i64;

    ///@warning i32[0] and i32[1] may be flipped                                
    toCh10DMY(time, &i32[1], &i32[0]);
}

//------------------------------------------------------------------------------
//  Function:  toCh10DMY                                                        
//------------------------------------------------------------------------------
void toCh10DMY(
    const lib::time::work::InstrumentationTime& time
  , uint32_t *pHw
  , uint32_t *pLw
)
{
    ds::CH10_DMY_LOW_WORD lowWord;
    ds::CH10_DMY_HIGH_WORD highWord;

    uint32_t year  = 0; // time.year();     ///@todo Ask Andy!                  
    uint32_t month = time.month();
    uint32_t mday  = time.mday();

    highWord.m_Unused2 = 0;
    highWord.m_ThouYear = year          / 1000;
    highWord.m_HundYear = (year % 1000) /  100;
    highWord.m_TensYear = (year % 100)  /   10;
    highWord.m_OnesYear = year % 10;
    highWord.m_Unused = 0;

    highWord.m_TensMonth = (month + 1) / 10;
    highWord.m_OnesMonth = (month + 1) % 10;

    highWord.m_TensDay = mday / 10;
    highWord.m_OnesDay = mday % 10;
    *pHw = highWord.m_Word;

    uint32_t hour = time.hour();
    uint32_t min  = time.min();
    uint32_t sec  = time.sec();
    uint32_t mil  = time.millisec();

    lowWord.m_Unused2 = 0;
    lowWord.m_TensHour = hour / 10;
    lowWord.m_OnesHour = hour % 10;
    lowWord.m_Unused = 0;
    lowWord.m_TensMin = min / 10;
    lowWord.m_OnesMin = min % 10;
    lowWord.m_TensSec = sec / 10;
    lowWord.m_OnesSec = sec % 10;
    lowWord.m_HundMilli = (mil % 1000)/100;
    lowWord.m_TensMilli = (mil % 100)/10;
    *pLw = lowWord.m_Word;
}
#endif

//------------------------------------------------------------------------------
//  Function:  toIEEE1588                                                       
//------------------------------------------------------------------------------
void toIEEE1588(
    const lib::time::work::InstrumentationTime& time
  , uint32_t *pSec
  , uint32_t *pNsec
)
{
    *pSec = time.tv_sec;
    *pNsec = time.tv_nsec;

}

} // namespace time                                                             
} // namespace work                                                             
} // namespace irig106                                                          
} // namespace lib                                                              

