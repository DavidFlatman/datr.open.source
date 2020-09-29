//------------------------------------------------------------------------------
///                                                                             
///@file lib_time_work_instrumentationtime.cpp                                  
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@brief   Represent time from the instrumnents point of view.                 
///                                                                             
///         InstrumentationTime is not the same as a DateTime for at least two  
///         reasons:                                                            
///             -#  InstrumentationTime may, or may not, have a year associated 
///                 with it.                                                    
///             -#  DateTime has 1970-01-01 as 0.  InstrumentationTime has      
///                 date 86400 as January 1st and 0 means that the time is not  
///                 synchronized.                                               
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2019-06-07  DHF     Worked on smoothed logic.                       
///                                                                             
///@version 2016-06-06  YBS     Copy constructor the smoothed indicator         
///                                                                             
///@version 2016-05-17  YBS     Added smoothed member and functions             
///                                                                             
///@version 2015-09-01  MNT     Made fromString accept leading zeroes for days. 
///                                                                             
///@version 2015-08-12  MNT     Made toString use leading zeroes for days too.  
///                                                                             
///@version 2015-07-17  DHF     Fixed a bug in operator< and operator<=         
///                                 -   Iniitalized result                      
///                                 -   Changed < to <= on sec check.           
///                                                                             
///@version 2015-07-16  YBS     Fixed a bug in operator< and operator<=         
///                                                                             
///@version 2015-04-17  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_time_work_instrumentationtime.h"

#include "lib_irig106_work_time.h"
#include "lib_si_prefixes.h"
#include "lib_string.h"
#include "lib_time_ds.h"
#include "lib_time_work_deltatime.h"

#include <boost/algorithm/string.hpp>       // boost::to_lower  
#include <boost/regex.hpp>

#include "debug.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
InstrumentationTime::InstrumentationTime() 
{
} // InstrumentationTime::InstrumentationTime() //

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
InstrumentationTime::InstrumentationTime(const InstrumentationTime& that)
    : DateDeltaTimeBase(that)
{
} 

//------------------------------------------------------------------------------
///@note    The hot, lot, mot values are uint16_t ...                           
///@warning This assumes that HOT, LOT, MOT are Ch4 binary and not Ch4 BCD.     
//------------------------------------------------------------------------------
InstrumentationTime::InstrumentationTime(
    uint16_t hot
  , uint16_t lot
  , uint16_t mot
)
{
    fromCh4Binary(hot, lot, mot);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
InstrumentationTime::InstrumentationTime(double seconds)
{
    setSeconds(seconds);
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
InstrumentationTime::InstrumentationTime(const DateTime& dt)
{
    *this = dt;
} 

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
InstrumentationTime::InstrumentationTime(const std::string& str)
{
    fromString(str);
}

//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
InstrumentationTime::~InstrumentationTime()
{

}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
InstrumentationTime& InstrumentationTime::operator=(const DateTime& dt)
{
    //--------------------------------------------------------------------------
    //  Note:  yday()+1, the +1 comes from January 1st is 86400...              
    //--------------------------------------------------------------------------
    tv_sec = ((dt.yday()+1) * ds::secondsPerDay)
           + (dt.hour() * ds::secondsPerHour)
           + (dt.min()  * ds::secondsPerMinute)
           + (dt.sec())
    ;

    tv_nsec = dt.tv_nsec;

    setSmoothed(dt.isSmoothed());
    
    return *this;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
InstrumentationTime& InstrumentationTime::operator=(const std::string& that)
{
    fromString(that);
    setSmoothed(false);
    return *this;
} 

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
InstrumentationTime& InstrumentationTime::operator=(double seconds)
{
    setSeconds(seconds);
    setSmoothed(false);
    return *this;
}

//------------------------------------------------------------------------------
///@brief   Convert the instrumentation time to a string in the format          
///         jjj:hh:mm:ss.ffffff                                                 
///                                                                             
/// @param fractionLength   The number of digits after the decimal point that   
///                         should be placed into the string.  If 0 then the    
///                         decimal point is suppressed.                        
///                                                                             
//------------------------------------------------------------------------------
std::string InstrumentationTime::toString(uint32_t fractionLength) const
{
    InstrumentationTime temp(*this);
    if (tv_sec < 0) temp.tv_sec *= -1;

    std::string result(
        lib::format(
            "%03d:%02d:%02d:%02d"
          , temp.jday()
          , temp.hour()
          , temp.min()
          , temp.sec()
        )
    );

    switch(fractionLength) {
        case 0:                                                         break;
        case 1: result += lib::format(".%01d", tv_nsec / 100000000);    break;
        case 2: result += lib::format(".%02d", tv_nsec / 10000000);     break;
        case 3: result += lib::format(".%03d", tv_nsec / 1000000);      break;
        case 4: result += lib::format(".%04d", tv_nsec / 100000);       break;
        case 5: result += lib::format(".%05d", tv_nsec / 10000);        break;
        case 6: result += lib::format(".%06d", tv_nsec / 1000);         break;
        case 7: result += lib::format(".%07d", tv_nsec / 100);          break;
        case 8: result += lib::format(".%08d", tv_nsec / 10);           break;
        default:result += lib::format(".%09d", tv_nsec / 1);            break;
    }

    return (tv_sec < 0) ? "-" + result : result;

}

//------------------------------------------------------------------------------
///@brief   Cast the object as a string.                                        
///                                                                             
///@code                                                                        
///     InstrumentationTime i_time;                                             
///                                                                             
///     //----------------------------------------------------------------------
///     // The following two lines are equivalent:                              
///     //----------------------------------------------------------------------
///     std::cout << i_time.toString() << std::endl;                            
///     std::cout << (std::string) i_time << std::endl;                         
///                                                                             
//------------------------------------------------------------------------------
InstrumentationTime::operator std::string() const
{
    return toString();
}

//------------------------------------------------------------------------------
///@brief   Assign the value of this instrumentation time to the value          
///         represented in the given string.                                    
///                                                                             
///@note    The given string is expected to be in jjj:hh:mm:ss.ffffff           
///         format.  The initial colon (:) can be a space ( ).  If              
///         fields are missing, it is assumed that the most significant         
///         fields are missing first.  For example,                             
///         15:30:20 is interpreted as 0 days, 15 hours, 30 minutes, and        
///         20 seconds.  Days (jjj) may contain leading spaces.                 
///                                                                             
///@param   str     The string to convert to InstrumentationTime.               
///@param   strict  If true, the field values are checked to ensure that        
///                 they are within the proper range (0-366 for days,           
///                 0-23 for hours, 0-59 for minutes, 0-59 for seconds.)        
///@return  false if failed the strict'ness test; otherwise true.               
//------------------------------------------------------------------------------
bool InstrumentationTime::fromString(const std::string& str, bool strict)
{
    tv_sec = tv_nsec = 0;

    //--------------------------------------------------------------------------
    // Declare the regular expression strings needed to create all              
    // syntax options listed above when combined. Any syntax string             
    // covering seconds may be used as is or combined with the Decimals one.    
    // Decimals may also be used as is.                                         
    //--------------------------------------------------------------------------
    std::string match_dddhhmmss( "(\\ *\\d+)[ :](\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    std::string match_hhmmss( "(\\d{1,2}):(\\d{1,2}):(\\d{1,2})");
    std::string match_mmss( "(\\d{1,2}):(\\d{1,2})");
    std::string match_ss( "(\\d{1,2})");
    std::string match_decimals("\\.(\\d*)");                                 
  
    //--------------------------------------------------------------------------
    // Using the strings declared above, declare regular expressions            
    // covering all syntax options listed even further above.                   
    //--------------------------------------------------------------------------

    static const boost::regex regex_dddhhmmss(  match_dddhhmmss);
    static const boost::regex regex_dddhhmmssff(match_dddhhmmss+match_decimals);

    static const boost::regex regex_hhmmss(  match_hhmmss);
    static const boost::regex regex_hhmmssff(match_hhmmss + match_decimals);

    static const boost::regex regex_mmss(  match_mmss);
    static const boost::regex regex_mmssff(match_mmss + match_decimals);

    static const boost::regex regex_ss(  match_ss);
    static const boost::regex regex_ssff(match_ss + match_decimals);

    //--------------------------------------------------------------------------
    // Extract time data from the string using either of the regular expressions
    // from above.                                                              
    //--------------------------------------------------------------------------
    int days(0);
    int hours(0);
    int min(0);
    int sec(0);
    
    std::string fraction;

    boost::smatch matches;

    if (boost::regex_match(str, matches, regex_dddhhmmss)) {
        days        = lib::toUnsigned(matches[1],10);
        hours       = lib::toUnsigned(matches[2],10);
        min         = lib::toUnsigned(matches[3],10);
        sec         = lib::toUnsigned(matches[4],10);

    } else if (boost::regex_match(str, matches, regex_dddhhmmssff)) {
        days        = lib::toUnsigned(matches[1],10);
        hours       = lib::toUnsigned(matches[2],10);
        min         = lib::toUnsigned(matches[3],10);
        sec         = lib::toUnsigned(matches[4],10);
        fraction    = matches[5];

    } else if (boost::regex_match(str, matches, regex_hhmmss)) {
        hours       = lib::toUnsigned(matches[1],10);
        min         = lib::toUnsigned(matches[2],10);
        sec         = lib::toUnsigned(matches[3],10);

    } else if (boost::regex_match(str, matches, regex_hhmmssff)) {
        hours       = lib::toUnsigned(matches[1],10);
        min         = lib::toUnsigned(matches[2],10);
        sec         = lib::toUnsigned(matches[3],10);
        fraction    = matches[4];

    } else if (boost::regex_match(str, matches, regex_mmss)) {
        min         = lib::toUnsigned(matches[1],10);
        sec         = lib::toUnsigned(matches[2],10);

    } else if (boost::regex_match(str, matches, regex_mmssff)) {
        min         = lib::toUnsigned(matches[1],10);
        sec         = lib::toUnsigned(matches[2],10);
        fraction    = matches[3];

    } else if (boost::regex_match(str, matches, regex_ss)) {
        sec         = lib::toUnsigned(matches[1],10);

    } else if (boost::regex_match(str, matches, regex_ssff)) {
        sec         = lib::toUnsigned(matches[1],10);
        fraction    = matches[2];

    } else {

        return false;   // Syntax Error!
    }

    //--------------------------------------------------------------------------
    //  Make sure the values are in range if strict syntax checking is          
    //  requested.                                                              
    //--------------------------------------------------------------------------
    if (
        strict && ((days > 366) || (hours   > 23) || (min     > 59) || (sec     > 59))
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
        lib::toUnsigned(fraction.substr(0,9),10) * (lib::si::nano / divisor);
    if (fraction.size() > 9) {
        if (fraction[9] >= '5' && fraction[9] <= '9') ++tv_nsec;
    }

    setSmoothed(false);

    //--------------------------------------------------------------------------
    //  Return True, everything is fine.                                        
    //--------------------------------------------------------------------------
    return true;
}

//------------------------------------------------------------------------------
///@brief   Returns true if the given string holds a valid                      
///         InstrumentationTime in the form of jjj:hh:mm:ss.ffffff              
//------------------------------------------------------------------------------
bool InstrumentationTime::isValid(const std::string& str)
{
    InstrumentationTime temp;
    return temp.fromString(str, true);
}

//------------------------------------------------------------------------------
///@brief   Set the current value based on the given hot, lot, mot.             
///                                                                             
///@param hot   16-bit value representing the high order time.                  
///@param lot   16-bit value representing the low order time.                   
///@param mot   16-bit value representing the microsecond time.                 
//------------------------------------------------------------------------------
void InstrumentationTime::fromCh4Binary(
    uint16_t hot
  , uint16_t lot
  , uint16_t mot
)
{
    //--------------------------------------------------------------------------
    //  Magic numbers?                                                          
    //      The values here are taken from IRIG-106 4.7 (chapter 4, section 7), 
    //      which states in part:                                               
    //          The microsecond time word shall have a resolution of 1          
    //          microsecond; htat is, the least significant bit (LSB), bit 16,  
    //          has a value of 0.000001 second.                                 
    //                                                                          
    //          The low order time word shall have a resolution of 10           
    //          milliseconds; that is, the LSB, bit 16, of the low order time   
    //          word shall have a value of 0.01 second.                         
    //                                                                          
    //          The high order time word shall have a resolution of 655.36      
    //          seconds when a binary weighted; that is, the LSB, bit 16, has a 
    //          value of 655.36 seconds.                                        
    //--------------------------------------------------------------------------
    setSeconds(hot * 655.36 + lot * 0.01 + mot * 1e-6);
    setSmoothed(false);
}

//------------------------------------------------------------------------------
///@brief   Convert the current value to hot, lot, mot.                         
///                                                                             
///@warning If you use non 16-bit variables, it is very possible that you will  
///         get incorrect results on a Big Endian system.                       
///                                                                             
///@param   hot     A pointer to the 16-bit location to receive the HOT.        
///@param   lot     A pointer to the 16-bit location to receive the LOT.        
///@param   mot     A pointer to the 16-bit location to receive the MOT.        
//------------------------------------------------------------------------------
void InstrumentationTime::toCh4Binary (
    uint16_t* hot
  , uint16_t* lot
  , uint16_t* mot
) const
{
    //--------------------------------------------------------------------------
    //  Why + 0.000000001?  Due to rounding errors, if lot and mot are both 0   
    //  it is possible for the hot to be off by 1.                              
    //--------------------------------------------------------------------------
    double s = inSeconds() + 0.000000001;

    *hot = s /655.36;
    *lot = (s - *hot * 655.36)/0.01;
    *mot = (s - *hot * 655.36 - *lot * 0.01)/1e-6;
}

//------------------------------------------------------------------------------
///@brief   Return true if the instrumentation time has been synchronize with   
///         an outside source.  This is determined by checking if the Julian    
///         Day is within the range of 1 and 366 (inclusive).                   
///                                                                             
///         Note:  The 366 would be 365 except for leap year.                   
//------------------------------------------------------------------------------
bool InstrumentationTime::isSynced() const
{
    return jday() > 0 && jday() <= 366;
} 

//------------------------------------------------------------------------------
///@brief   The opposite of isSynced.                                           
//------------------------------------------------------------------------------
bool InstrumentationTime::isElapsed() const
{
    return !isSynced();
}

//------------------------------------------------------------------------------
///@brief   Return true if this value is the same as that value.                
///                                                                             
///@note    Whether or not a time has been smoothed is irrelevant to the        
///         equality check.                                                     
//------------------------------------------------------------------------------
bool InstrumentationTime::operator==(const InstrumentationTime& that) const
{
    return tv_sec == that.tv_sec && tv_nsec == that.tv_nsec;
}

//------------------------------------------------------------------------------
///@brief   Return true if this value is less than that value.                  
//------------------------------------------------------------------------------
bool InstrumentationTime::operator< (const InstrumentationTime& that) const
{
    bool result(false);
    if (tv_sec == that.tv_sec) {
        result = (tv_nsec < that.tv_nsec);
    } else {
        result = (tv_sec < that.tv_sec);
    }
    return result;
}

//------------------------------------------------------------------------------
///@brief   Return true if this value is less than or equal to that value.      
//------------------------------------------------------------------------------
bool InstrumentationTime::operator<=(const InstrumentationTime& that) const
{
    bool result(false);
    if (tv_sec == that.tv_sec) {
        result = (tv_nsec <= that.tv_nsec);
    } else {
        result = (tv_sec <= that.tv_sec);
    }
    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the Julian Day.  The jday() of January 1st is 1.             
///         If jday() is 0, that implies that the time is not synchronized.     
//------------------------------------------------------------------------------
uint32_t InstrumentationTime::jday() const
{
    return tv_sec / ds::secondsPerDay;
}

//------------------------------------------------------------------------------
///@brief   Return the hour portion of this value.                              
//------------------------------------------------------------------------------
uint32_t InstrumentationTime::hour() const
{
    return (tv_sec % ds::secondsPerDay) / ds::secondsPerHour;
}

//------------------------------------------------------------------------------
///@brief   Return the minute portion of this value.                            
//------------------------------------------------------------------------------
uint32_t InstrumentationTime::min() const
{
    return 
        ((tv_sec % ds::secondsPerDay) % ds::secondsPerHour) 
            / ds::secondsPerMinute
    ;
}

//------------------------------------------------------------------------------
///@brief   Return the seconds portion of this value.                           
//------------------------------------------------------------------------------
uint32_t InstrumentationTime::sec() const
{
    return tv_sec % ds::secondsPerMinute;
}

//------------------------------------------------------------------------------
///@brief   Return the number of milliseconds in the time.                      
//------------------------------------------------------------------------------
uint64_t InstrumentationTime::millisec() const
{
    uint64_t result = nanosec() / (lib::si::nano / (lib::si::milli * 10));

    if (result % 10 >= 5) result = (result / 10 + 1);
    else result = result / 10;

    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the number of microseconds in the time.                      
//------------------------------------------------------------------------------
uint64_t InstrumentationTime::microsec() const
{
    uint64_t result = nanosec() / (lib::si::nano / (lib::si::micro * 10));

    if (result % 10 >= 5) result = (result / 10 + 1);
    else result = result / 10;

    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the number of nanoseconds in the time.                       
//------------------------------------------------------------------------------
uint64_t InstrumentationTime::nanosec() const
{
    return tv_nsec;
}

//------------------------------------------------------------------------------
///@brief   Convert the instrumentation time to a DateTime.                     
///@warning The results depend on whether the InstrumentationTime has sync or   
///         not.                                                                
///\n\n                                                                         
///         If the time is synced then the given basisTime should be            
///         January 1st of the year that the instrumentation time was recorded  
///         in.                                                                 
///\n\n                                                                         
///         If the time is elapsed then the given basisTime should be when      
///         the elapsed time began; in other words, the actual DateTime for     
///         elapsed time 0.                                                     
///                                                                             
///@param basisTime A DateTime representing the basis for the instrumentation   
///                 time.  Either January 1st of the year if the time is        
///                 synchronized or time 0 if in elapsed mode.                  
//------------------------------------------------------------------------------
DateTime InstrumentationTime::toDateTime(const DateTime& basisTime) const
{
    DeltaTime delta;
    delta.tv_nsec = tv_nsec;

    if (isSynced()) {
        delta.tv_sec = tv_sec - ds::secondsPerDay;
    } else {
        delta.tv_sec = tv_sec;
    }
    
    DateTime result(basisTime + delta);
    result.setSmoothed(isSmoothed());

    return result;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
InstrumentationTime& InstrumentationTime::operator += (const DeltaTime& delta)
{
    setSmoothed(isSmoothed() || delta.isSmoothed());

    tv_sec += delta.tv_sec;
    tv_nsec += delta.tv_nsec;
    adjust();
    return *this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
InstrumentationTime& InstrumentationTime::operator -= (const DeltaTime& delta)
{
    setSmoothed(isSmoothed() || delta.isSmoothed());

    if (tv_sec >= delta.tv_sec) {
        tv_nsec -= delta.tv_nsec;
        tv_sec  -= delta.tv_sec;
        adjust();
    } else {
        tv_sec  = delta.tv_sec  - tv_sec;
        tv_nsec = delta.tv_nsec - tv_nsec;
        adjust();
        tv_sec *= -1;
    }
    return *this;
}

InstrumentationTime InstrumentationTime::minimum()
{
    return InstrumentationTime("000:00:00:00.000000");
}

InstrumentationTime InstrumentationTime::maximum()
{
    return InstrumentationTime("367:00:00:00.000000");
}


} // namespace work //
} // namespace time //
} // namespace lib //

//------------------------------------------------------------------------------
///@brief   Return the sum of the InstrumentationTime and the DeltaTime.        
///@param   iTime   The instrumentation time.                                   
///@param   dTime   The delta time to add to the instrumentation time.          
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime operator+(
    const lib::time::work::InstrumentationTime& iTime
  , const lib::time::work::DeltaTime& dTime
)
{
    lib::time::work::InstrumentationTime result(iTime);
    result += dTime;

    result.setSmoothed(iTime.isSmoothed() || dTime.isSmoothed());

    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the sum of the InstrumentationTime and the DeltaTime.        
///@param   iTime   The instrumentation time.                                   
///@param   dTime   The delta time to add to the instrumentation time.          
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime operator+(
    const lib::time::work::DeltaTime& dTime
  , const lib::time::work::InstrumentationTime& iTime
)
{
    lib::time::work::InstrumentationTime result(iTime);
    result += dTime;

    return result;
}

//------------------------------------------------------------------------------
///@brief   Subtract the delta time from the instrumentation time.              
///@param   iTime   The instrumentation time.                                   
///@param   dTime   The delta time to subtract from the instrumentation time.   
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime operator-(
    const lib::time::work::InstrumentationTime& iTime
  , const lib::time::work::DeltaTime& dTime
)
{
    lib::time::work::InstrumentationTime result(iTime);
    result -= dTime;

    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the difference between to InstrumentationTimes.              
//------------------------------------------------------------------------------
lib::time::work::DeltaTime operator-(
    const lib::time::work::InstrumentationTime& iTime1
  , const lib::time::work::InstrumentationTime& iTime2
)
{
    lib::time::work::DeltaTime result;
    if (iTime1.tv_sec >= iTime2.tv_sec) {
        result.tv_sec  = iTime1.tv_sec  - iTime2.tv_sec;
        result.tv_nsec = iTime1.tv_nsec - iTime2.tv_nsec;
        result.adjust();
    } else {
        result.tv_sec  = iTime2.tv_sec  - iTime1.tv_sec;
        result.tv_nsec = iTime2.tv_nsec - iTime1.tv_nsec;
        result.adjust();
        result.tv_sec *= -1;
    }

    result.setSmoothed(iTime1.isSmoothed() || iTime2.isSmoothed());

    return result;
}

