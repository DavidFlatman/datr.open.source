//------------------------------------------------------------------------------
///@file lib_time_work_deltatime.cpp                                            
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Class representing the different between two times.                 
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-09-23  JRS     replace boost references with std references.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-10-07  MNT     Reimplemented                                   
///                                 -   fromString                              
///                                 -   isValid                                 
///                                                                             
///@version 2014-10-01  DHF     Break out class from lib_datetime.h             
///                                                                             
///@version 2014-09-24  DHF     Added DeltaTime:                                
///                                 -   fromString                              
///                                 -   isValid                                 
///                                 -   operator=(const std::string&)           
///                                                                             
//------------------------------------------------------------------------------

#include "lib_time_work_deltatime.h"
#include "lib_time_work_datetime.h"
#include "lib_si_ds_prefixes.h"
#include "lib_string.h"                     // lib::format      
#include "lib_time_ds.h"

#include <regex>
#include "debug.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DeltaTime::DeltaTime(const std::string& time)
{
    fromString(time);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DeltaTime::operator double() const
{
    return tv_sec + double(tv_nsec) / lib::si::ds::nano;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string DeltaTime::toString(bool stopAtSeconds) const
{
    int64_t seconds = tv_sec;

    int64_t days = seconds / lib::time::ds::secondsPerDay;
    seconds -= days * lib::time::ds::secondsPerDay;

    int64_t hours = seconds / lib::time::ds::secondsPerHour;
    seconds -= hours * lib::time::ds::secondsPerHour;

    int64_t minutes = seconds / lib::time::ds::secondsPerMinute;
    seconds -= minutes * lib::time::ds::secondsPerMinute;

    std::string result;

    if (stopAtSeconds) {
        result = lib::format(
            "%3d %02d:%02d:%02d"
          , int(days)
          , int(hours)
          , int(minutes)
          , int(seconds)
        );
    } else {
        result = lib::format(
            "%3d %02d:%02d:%02d.%09d"
          , int(days)
          , int(hours)
          , int(minutes)
          , int(seconds)
          , int(tv_nsec)
        );
    }

    return result;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool DeltaTime::operator==(const DeltaTime& that) const
{
    return tv_sec == that.tv_sec && tv_nsec == that.tv_nsec;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool DeltaTime::operator< (const DeltaTime& that) const
{
    if (tv_sec > that.tv_sec) {
        return false;

    } else if (tv_sec < that.tv_sec) {
        return true;

    } else {
        return tv_nsec < that.tv_nsec;

    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool DeltaTime::operator<=(const DeltaTime& that) const
{
    if (tv_sec > that.tv_sec) {
        return false;

    } else if (tv_sec < that.tv_sec) {
        return true;

    } else {
        return tv_nsec <= that.tv_nsec;

    }
}

//------------------------------------------------------------------------------
///                                                                             
///@brief   Return true if the given string represents a valid delta time.      
///                                                                             
///@param time  A string holding a delta time in one of the following formats:  
///             ddd:hh:mm:ss                                                    
///             ddd:hh:mm:ss.ffffff                                             
///             ddd hh:mm:ss                                                    
///             ddd hh:mm:ss.ffffff                                             
///             ddd hh:mm                                                       
///             ddd hh                                                          
///                hhh:mm:ss                                                    
///                hhh:mm:ss.ffffff                                             
///                   mmm:ss                                                    
///                   mmm:ss.ffffff                                             
///                    sssss                                                    
///                    sssss.ffffff                                             
///                         .ffffff                                             
///                                                                             
///@return  true = a valid delta time format; false = an invalid format.        
///                                                                             
//------------------------------------------------------------------------------
bool DeltaTime::isValid(const std::string& time)
{
    DeltaTime dt;
    return dt.fromString(time, true);

}

//------------------------------------------------------------------------------
///@brief   Set the value to the given delta time; return true if a valid time. 
///                                                                             
///@param time  A string holding a delta time in one of the following formats:  
///             ddd:hh:mm:ss                                                    
///             ddd:hh:mm:ss.ffffff                                             
///             ddd hh:mm:ss                                                    
///             ddd hh:mm:ss.ffffff                                             
///             ddd hh:mm                                                       
///             ddd hh                                                          
///                hhh:mm:ss                                                    
///                hhh:mm:ss.ffffff                                             
///                   mmm:ss                                                    
///                   mmm:ss.ffffff                                             
///                    sssss                                                    
///                    sssss.ffffff                                             
///                         .ffffff                                             
///                                                                             
///@param strict    A Boolean indicating that a strict check on the field       
///                 values should be enforced.                                  
///                                                                             
///@return  true = a valid delta time format; false = an invalid format.        
///                                                                             
//------------------------------------------------------------------------------
bool DeltaTime::fromString(const std::string& time, bool strict)
{
    //--------------------------------------------------------------------------
    // Declare the regular expression strings needed to create all
    // syntax options listed above when combined. Any syntax string
    // covering seconds may be used as is or combined with the Decimals one.
    // Decimals may also be used as is.
    //--------------------------------------------------------------------------

    std::string match_ddhhmmss( "(\\d+)[ :](\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    std::string match_ddhhmm(      "(\\d+) (\\d{1,2}):(\\d{1,2})");
    std::string match_ddhh(        "(\\d+) (\\d{1,2})");
    std::string match_hhmmss(                 "(\\d+):(\\d{1,2}):(\\d{1,2})");
    std::string match_mmss(                              "(\\d+):(\\d{1,2})");
    std::string match_ss(                                       "(\\d+)");                                
    std::string match_decimals("\\.(\\d*)");                                 
  
    //--------------------------------------------------------------------------
    // Using the strings declared above, declare regular expressions
    // covering all syntax options listed even further above.
    //--------------------------------------------------------------------------

    static const std::regex regex_ddhhmmss(  match_ddhhmmss);
    static const std::regex regex_ddhhmmssff(match_ddhhmmss + match_decimals);
    static const std::regex regex_ddhhmm(    match_ddhhmm);
    static const std::regex regex_ddhh(      match_ddhh);
    static const std::regex regex_hhmmss(    match_hhmmss);
    static const std::regex regex_hhmmssff(  match_hhmmss   + match_decimals);
    static const std::regex regex_mmss(      match_mmss);
    static const std::regex regex_mmssff(    match_mmss     + match_decimals);
    static const std::regex regex_ss(        match_ss);
    static const std::regex regex_ssff(      match_ss       + match_decimals);
    static const std::regex regex_ff(                         match_decimals);

    //--------------------------------------------------------------------------
    // Extract time data from the string using either of the regular expressions
    // from above.                                                              
    //--------------------------------------------------------------------------
    int days(0);
    int hours(0);
    int min(0);
    int sec(0);
    
    bool hoursMayNeedCheck(false);
    bool minMayNeedCheck(false);
    bool secMayNeedCheck(false);
    
    std::string fraction;

    std::smatch matches;

    if (std::regex_match(time, matches, regex_ddhhmmss)) {
        days        = lib::toUnsigned(matches[1],10);
        hours       = lib::toUnsigned(matches[2],10);
        min         = lib::toUnsigned(matches[3],10);
        sec         = lib::toUnsigned(matches[4],10);
        hoursMayNeedCheck = minMayNeedCheck = secMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_ddhhmmssff)) {
        days        = lib::toUnsigned(matches[1],10);
        hours       = lib::toUnsigned(matches[2],10);
        min         = lib::toUnsigned(matches[3],10);
        sec         = lib::toUnsigned(matches[4],10);
        fraction    = matches[5];
        hoursMayNeedCheck = minMayNeedCheck = secMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_ddhhmm)) {
        days        = lib::toUnsigned(matches[1],10);
        hours       = lib::toUnsigned(matches[2],10);
        min         = lib::toUnsigned(matches[3],10);
        hoursMayNeedCheck = minMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_ddhh)) {
        days        = lib::toUnsigned(matches[1],10);
        hours       = lib::toUnsigned(matches[2],10);
        hoursMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_hhmmss)) {
        hours       = lib::toUnsigned(matches[1],10);
        min         = lib::toUnsigned(matches[2],10);
        sec         = lib::toUnsigned(matches[3],10);
        minMayNeedCheck = secMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_hhmmssff)) {
        hours       = lib::toUnsigned(matches[1],10);
        min         = lib::toUnsigned(matches[2],10);
        sec         = lib::toUnsigned(matches[3],10);
        fraction    = matches[4];
        minMayNeedCheck = secMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_mmss)) {
        min         = lib::toUnsigned(matches[1],10);
        sec         = lib::toUnsigned(matches[2],10);
        secMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_mmssff)) {
        min         = lib::toUnsigned(matches[1],10);
        sec         = lib::toUnsigned(matches[2],10);
        fraction    = matches[3];
        secMayNeedCheck = true;
    } else if (std::regex_match(time, matches, regex_ss)) {
        sec         = lib::toUnsigned(matches[1],10);
    } else if (std::regex_match(time, matches, regex_ssff)) {
        sec         = lib::toUnsigned(matches[1],10);
        fraction    = matches[2];
    } else if (std::regex_match(time, matches, regex_ff)) {
        fraction    = matches[1];
    } else {
        return false;   // Syntax Error!
    }

    //--------------------------------------------------------------------------
    //  Make sure the values are in range if strict syntax checking is          
    //  requested.                                                              
    //--------------------------------------------------------------------------
    if (
        strict && (
            (hoursMayNeedCheck  && (hours   > 23))
         || (minMayNeedCheck    && (min     > 59))
         || (secMayNeedCheck    && (sec     > 59))
        )
    ) return false;
 
    //--------------------------------------------------------------------------
    //  Calculate and save total integer seconds                                
    //--------------------------------------------------------------------------
    tv_sec =
        days    *   lib::time::ds::secondsPerDay
      + hours   *   lib::time::ds::secondsPerHour
      + min     *   lib::time::ds::secondsPerMinute
      + sec;   

    //--------------------------------------------------------------------------
    //  Calculate and save fractions of seconds.                                
    //  Round if precision is higher than nano can handle.                      
    //  Ensure correct handling of leading zeroes.                              
    //--------------------------------------------------------------------------
    int64_t divisor = 1;
    for (size_t f=0; f < fraction.substr(0,9).size(); ++f) {
        divisor *= 10;
    }

    tv_nsec =
        lib::toUnsigned(fraction.substr(0,9),10) * (lib::si::ds::nano / divisor);
    if (fraction.size() > 9) {
        if (fraction[9] >= '5' && fraction[9] <= '9') ++tv_nsec;
    }

    //--------------------------------------------------------------------------
    //  Return True, everything is fine.                                        
    //--------------------------------------------------------------------------
    return true;
}   //  bool DeltaTime::fromString(const std::string& deltaTime)

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DeltaTime& DeltaTime::operator=(const DeltaTime& t)
{
    lib::time::work::DateDeltaTimeBase::operator=(t);
    return *this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DeltaTime& DeltaTime::operator=(const std::string& t)
{
    fromString(t);
    return *this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DeltaTime& DeltaTime::operator+=(const DeltaTime& that)
{
    tv_sec  += that.tv_sec;
    tv_nsec += that.tv_nsec;
    adjust();

    return *this;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DeltaTime& DeltaTime::operator-=(const DeltaTime& that)
{
    tv_sec  -= that.tv_sec;
    tv_nsec -= that.tv_nsec;
    adjust();

    return *this;
}

} // namespace work 
} // namespace time 
} // namespace lib  

