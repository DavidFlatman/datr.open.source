//------------------------------------------------------------------------------
///                                                                             
///@file lib_time_ds.h                                                          
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Hold the data structures and constants for lib::time:: namesapce.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-10-02  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_TIME_DS_H_FILE_GUARD
#define LIB_TIME_DS_H_FILE_GUARD

namespace lib {
namespace time {
namespace ds {

static const uint32_t hoursPerDay       (24);
static const uint32_t minutesPerHour    (60);
static const uint32_t secondsPerMinute  (60);
static const uint32_t secondsPerHour    (3600);
static const uint32_t secondsPerDay     (86400);
static const uint32_t secondsPerYear    (secondsPerDay * 365);

static const uint32_t secondsInHundredDays(secondsPerDay * 100);
static const uint32_t secondsInTenDays(secondsPerDay * 10);
static const uint32_t secondsInOneDay(secondsPerDay * 1);

static const uint32_t secondsInTenHours(secondsPerHour * 10);
static const uint32_t secondsInOneHour(secondsPerHour * 1);

static const uint32_t secondsInTenMinutes(secondsPerMinute * 10);
static const uint32_t secondsInOneMinute(secondsPerMinute * 1);

} // namesapce ds   
} // namespace time 
} // namespace lib  

#endif // #ifndef LIB_TIME_DS_H_FILE_GUARD
