//------------------------------------------------------------------------------
///@file lib_time_work_datetime.cpp                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@par Class: DateTime                                                         
///                                                                             
///@brief   Date Time class                                                     
///                                                                             
///@par Purpose:                                                                
///         **PLEASE**  Only evolve this class as needed; don't throw in the    
///         kitchen sink (unless of course you need the kitchen sink).          
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2019-06-07  DHF     Added "smoothed" functionality.                 
///                                                                             
///@version 2015-04-05  DHF     Allow ddd:hh:mm:ss.hhhh format in strict mode.  
///                                                                             
///@version 2015-03-17  DHF     Modified maximum time to 2500-12-31 because of  
///                             Visual Studio's limitations.                    
///                                                                             
///@version 2015-03-16  DHF     Modified minimum time to 1970-01-01 because of  
///                             Visual Studio's inability to handle dates before
///                             then.                                           
///                                                                             
///@version 2014-11-03  DHF     Changed the default value of maximum to have 0  
///                             nano-seconds.                                   
///                             Add toString(size_t precision);                 
///                                                                             
///@version 2014-10-02  DHF     File created from lib_datetime.cpp              
///                                                                             
///@version 2014-09-24  DHF     Added DeltaTime:                                
///                                 -   fromString                              
///                                 -   isValid                                 
///                                 -   operator=(const std::string&)           
///                                                                             
///@version 2014-02-11  DHF     Removed compiler warning;                       
///                             Changed -1 to std::string::npos                 
///                                                                             
///@version 2013-10-21  DHF     Added operator-(DateTime, DeltaTime);           
///                                                                             
///@version 2011-05-26  DHF     Modified the operator=(const std::string&) to   
///                             handle format yyyy-dd-mm hh:mm:ss.sssss         
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-11-16  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
#include "lib_time_work_datetime.h"
#include "lib_si_ds_prefixes.h"
#include "lib_string.h"
#include "lib_time_ds.h"
#include "lib_compiler_info.h"

#include <assert.h>
#include <boost/algorithm/string.hpp>       // boost::to_lower  
#include <boost/regex.hpp>
#include <ctype.h>              // isalpha, isdigit (may need change for MS)    
#include <stdio.h>
#include <stdint.h>
#include <string.h>             // memset   
#include <stdexcept>
#include <time.h>

#ifndef _MSC_VER
#include <sys/time.h>
#else
#include <windows.h>
#endif

// use the C++ definitions if possible, not the C macros. Especially do this if you're making a routine called min.
#ifdef min
#undef min
#undef max
#endif

#ifdef _WIN32

// The POSIX/C functions localtime() and gmtime() aren't guaranteed to be
// thread safe, while localtime_r() and gmtime_r() are.
// The Windows versions of localtime() and gmtime() are guaranteeds thread
// safe, but there isn't a defined localtime_r() and gmtime_r().
// These stubs work around the failings of the POSIX system by providing
// localtime_r() and gmtime_r() as shims around the Windows functions.

#ifdef localtime_r
#undef localtime_r
#endif

#ifdef gmtime_r
#undef gmtime_r
#endif

static struct tm* localtime_r(const time_t* tp, struct tm* result)
{
    memcpy(result, ::localtime(tp), sizeof(tm));
    return result;
}

static struct tm* gmtime_r(const time_t* tp, struct tm* result)
{
    memcpy(result, ::gmtime(tp), sizeof(tm));
    return result;
}

#if IS_VISUAL_STUDIO

// gettimeofday isn't native on Windows, it looks like.
// from https://social.msdn.microsoft.com/Forums/vstudio/en-us/430449b3-f6dd-4e18-84de-eebd26a8d668/gettimeofday
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

static int gettimeofday(struct timeval* tv, struct timezone* tz = NULL)
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if (NULL != tv)
    {
        GetSystemTimeAsFileTime(&ft);

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tmpres /= 10;  /*convert into microseconds*/
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (NULL != tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}

#endif // #if IS_VISUAL_STUDIO

#endif              // #ifdef _WIN32    

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DateTime::DateTime(const std::string& that)
{
    fromString(that);
//    *this = that;

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
DateTime& DateTime::operator=(const DateTime& that)
{
    DateDeltaTimeBase::operator=(that);

    return *this;
}

//------------------------------------------------------------------------------
///@brief   Assign the time value contained in the given string to this object. 
///@sa      fromString                                                          
//------------------------------------------------------------------------------
DateTime& DateTime::operator=(const std::string& that)
{
    fromString(that);
    return *this;

} // DateTime& DateTime::operator=(const std::string& that)

//------------------------------------------------------------------------------
///@brief   Convert the given string to a date and time.                        
///@par Date Formats                                                            
///         -   yyyy-mm-dd                                                      
///         -   yyyy/mm/dd                                                      
///         -   mm-dd-yyyy                                                      
///         -   mm/dd/yyyy                                                      
///         -   ddmmmyyyy                                                       
///         -   ddmmmyy                                                         
///         -   mmm dd, yyyy                                                    
///         -   mmm dd, yy                                                      
///                                                                             
///@par Time Formats                                                            
///         -   HH:MM:SS.FFFFFFF                                                
///         -   HH:MM:SS                                                        
///         -   HH:MM                                                           
///         -   HH                                                              
///         -                                                                   
///                                                                             
///@par Notes:                                                                  
///         -   Missing time components are assumed to be 0.                    
///         -   The separator between the date and time can be either a space or
///             a colon.                                                        
///                                                                             
///@param str       The std::string holding the time to be converted.           
///                                                                             
///@param strict    If strict is true then the values are examined to make sure 
///                 that they fall within expected ranges.                      
///                                                                             
///@return      true = The time was able to be extracted from the string.  If   
///             strict is true then bad values (e.g., 61 minutes) will be       
///             flagged with false.  If strict is not specified, 61 minutes is  
///             treated as 1 hour and 1 minute.                                 
///                                                                             
//------------------------------------------------------------------------------
bool DateTime::fromString(const std::string& str, bool strict)
{
    bool result = true;
    bool dddFormat = false;

    //--------------------------------------------------------------------------
    //  yyyy-mm-dd                                                              
    //  yyyy/mm/dd                                                              
    //--------------------------------------------------------------------------
    static const boost::regex d0("^\\s*(\\d{4})[/-](\\d{2})[/-](\\d{2})(.*)");

    //--------------------------------------------------------------------------
    //  mm-dd-yyyy                                                              
    //  mm/dd/yyyy                                                              
    //--------------------------------------------------------------------------
    static const boost::regex d1("^\\s*(\\d{2})[/-](\\d{2})[/-](\\d{4})(.*)");

    //--------------------------------------------------------------------------
    //  ddmmmyyyy                                                               
    //--------------------------------------------------------------------------
    static const boost::regex d2("^\\s*(\\d{2})(\\w{3})(\\d{2,})(.*)");

    //--------------------------------------------------------------------------
    //  mmm dd, yyyy                                                            
    //--------------------------------------------------------------------------
    static const boost::regex d4("^\\s*(\\w{3}) (\\d{1,2}), (\\d{2,})(.*)");

    //--------------------------------------------------------------------------
    //  ddd                                                                     
    //--------------------------------------------------------------------------
    static const boost::regex d5("^\\s*(\\d{1,3}):*(.*)");

    std::string m;
    int    month(0);
    int    day(0);
    int    year(0);
    std::string remainder;
    std::string yearStr;

    boost::smatch p;

    if (boost::regex_match(str, p, d0)) {
        year    = lib::toUnsigned(p[1], 10);
        month   = lib::toUnsigned(p[2], 10);
        day     = lib::toUnsigned(p[3], 10);
        remainder = p[4];
        yearStr = p[1];

    } else if (boost::regex_match(str, p, d1)) {
        month   = lib::toUnsigned(p[1], 10);
        day     = lib::toUnsigned(p[2], 10);
        year    = lib::toUnsigned(p[3], 10);
        remainder = p[4];
        yearStr = p[3];

    } else if (boost::regex_match(str,p,d2)) {
        day     = lib::toUnsigned(p[1], 10);
        m       = p[2];
        year    = lib::toUnsigned(p[3], 10);
        remainder = p[4];
        yearStr = p[3];
    } else if (boost::regex_match(str,p,d4)) {
        m       = p[1];
        day     = lib::toUnsigned(p[2], 10);
        year    = lib::toUnsigned(p[3], 10);
        remainder = p[4];
        yearStr = p[3];

    } else if (boost::regex_match(str, p, d5)) {
        day     = lib::toUnsigned(p[1], 10);
        year    = baseline().year();
        remainder = p[2];
        dddFormat = true;

    } else {
        result = false;
    }

    //--------------------------------------------------------------------------
    //  If the string specified a string month, translate it to a number.       
    //--------------------------------------------------------------------------
    if (m != "") {
        static const std::string months("janfebmaraprmayjunjulaugsepoctnovdec");
        size_t offset = months.find(boost::to_lower_copy(m));
        if (offset == std::string::npos) {
            result = false;
        } else {
            month = static_cast<int>(offset) / 3 + 1;  // 3 = number of characters, 1 = january   
        }
    }

    if (year < 70)   year += 2000;  //  30jan94 is same as 1994-01-30           
                                    //  30jan69 is same as 2069-01-30           
    if (year < 100)  year += 1900;

    if (result && strict && dddFormat) {
        result = (day > 0 && day < 366);

    } else if (result && strict) {
        result = (year >= minimum().year()) && (year <= maximum().year());
        result = result && month >= 1 && month <= 12;

        if (result && !yearStr.empty() && yearStr[0] == '0' && yearStr != "00") {
            result = false;
        }

        if (result && isLeapYear(year) && month == 2) {
            result = (day <= 29 && day > 0);
        } else if (result) {
            result = (day <= monthLength(month-1) && day > 0);
        }
    }

    if (result) {
        setYMD(year, month-1, day, 0, 0, 0, uint64_t(0));

        if (!remainder.empty()) {
            lib::time::work::DeltaTime delta;
            if (remainder[0] == ' ' || remainder[0] == ':') {
                result = delta.fromString(remainder.substr(1));
            } else {
                result = delta.fromString(remainder);
            }
            if (result) *this = *this + delta;
        }
    }

    setSmoothed(false);
    
    return result;
        
} // bool DateTime::fromString(const std::string& str)

//------------------------------------------------------------------------------
///@brief   Return the current system time as a DateTime object.                
//------------------------------------------------------------------------------
DateTime DateTime::now()
{
    DateTime result;

    #ifndef _WIN32

    clock_gettime(CLOCK_REALTIME, &result);

    #else 

    timeval t;
    gettimeofday(&t, NULL);
    result.tv_sec = t.tv_sec;
    result.tv_nsec = t.tv_usec * (lib::si::ds::nano / lib::si::ds::micro);

    #endif

    result.setSmoothed(false);

    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the maximum date/time.                                       
///@note    The system does not perform checks if the maximum is violated, but  
///         dates above the maximum are subject to breakage without notice.     
//------------------------------------------------------------------------------
DateTime DateTime::maximum()
{
    static const DateTime maxTime("2500-12-12 23:59:59.000000");
    return maxTime;
}

//------------------------------------------------------------------------------
///@brief   Return the minimum date/time.                                       
///@note    The system does not perform checks to ensure that date is not before
///         the minimum date, but dates before the minimum date are subject to  
///         breakage without notice.                                            
//------------------------------------------------------------------------------
DateTime DateTime::minimum()
{
//    static const DateTime min("1903-01-01 00:00:00.00000");  // first flight 1903-12-17

    //--------------------------------------------------------------------------
    //  Visual Studio has a limit to no dates before 1970.                      
    //--------------------------------------------------------------------------
    static const DateTime min("1970-01-01 00:00:00.00000");  
    return min;
}

//------------------------------------------------------------------------------
///@brief   The "baseline" time is the default time if all values are 0.        
///         By convention is this 1970-01-01 00:00:00.000000 (but may change)   
//------------------------------------------------------------------------------
DateTime DateTime::baseline()
{
    return DateTime(0);
}

//------------------------------------------------------------------------------
///@brief   Return true if the given time occurs after this time.               
//------------------------------------------------------------------------------
bool DateTime::operator<(const DateTime& right) const
{
    if (tv_sec < right.tv_sec) {
        return true;

    } else if (tv_sec == right.tv_sec) {
        return tv_nsec < right.tv_nsec;

    } else {
        return false;

    }
}

//------------------------------------------------------------------------------
///@brief   Return true if the given time is the same as this time.             
//------------------------------------------------------------------------------
bool DateTime::operator==(const DateTime& right) const
{
    return (tv_sec == right.tv_sec) && (tv_nsec == right.tv_nsec);
}

//------------------------------------------------------------------------------
///@brief   Return the day in year (January 1st = 0).                           
//------------------------------------------------------------------------------
int DateTime::yday(TIME_LOCATION location) const
{
    struct tm t;
    get_time(location, t);

    return t.tm_yday;
}

//------------------------------------------------------------------------------
///@brief   Return the day of the month.                                        
//------------------------------------------------------------------------------
int DateTime::mday(TIME_LOCATION location) const
{
    struct tm t;
    get_time(location, t);

    return t.tm_mday;
}

//------------------------------------------------------------------------------
///@brief   Return the month (0 = January, 1 = February, ...)                   
//------------------------------------------------------------------------------
int DateTime::month(TIME_LOCATION location) const
{
    struct tm t;
    get_time(location, t);

    return t.tm_mon;
}

//------------------------------------------------------------------------------
///@brief   Return the 4 digit year.                                            
//------------------------------------------------------------------------------
int DateTime::year(TIME_LOCATION location) const
{
    struct tm t;
    get_time(location, t);

    return t.tm_year + 1900;
}

//------------------------------------------------------------------------------
///@brief   Return the hour of the time.                                        
//------------------------------------------------------------------------------
int DateTime::hour() const
{
    return (tv_sec / ds::secondsPerHour) % ds::hoursPerDay;
}

//------------------------------------------------------------------------------
///@brief   Return the minutes of the time.                                     
//------------------------------------------------------------------------------
int DateTime::min() const
{
    return (tv_sec / ds::secondsPerMinute) % ds::minutesPerHour;
}

//------------------------------------------------------------------------------
///@brief   Return the seconds field of the time.                               
//------------------------------------------------------------------------------
int DateTime::sec() const
{
    return (tv_sec % ds::secondsPerMinute);
}

//------------------------------------------------------------------------------
///@brief   Return the milliseconds of the time.                                
//------------------------------------------------------------------------------
int DateTime::millisec() const
{
    return tv_nsec / (lib::si::ds::nano / lib::si::ds::milli);
}

//------------------------------------------------------------------------------
///@brief   Return the microseconds of the time.                                
//------------------------------------------------------------------------------
int DateTime::microsec() const
{
    return tv_nsec / (lib::si::ds::nano / lib::si::ds::micro);
}

//------------------------------------------------------------------------------
///@brief   Return the nanoseconds of the time.                                 
//------------------------------------------------------------------------------
int DateTime::nanosec() const
{
    return tv_nsec;
}

//------------------------------------------------------------------------------
///@brief   Return the seconds since midnight (as a double).                    
//------------------------------------------------------------------------------
double DateTime::secondsSinceMidNight() const
{
    return (tv_sec % ds::secondsPerDay) + tv_nsec / double(lib::si::ds::nano);
}

//------------------------------------------------------------------------------
///@brief   Convert the internal time representation to a local time tm struct. 
//------------------------------------------------------------------------------
tm* DateTime::localtime(tm& t) const
{
    time_t temp = tv_sec;
    localtime_r(&temp, &t);
    return &t;
}

//------------------------------------------------------------------------------
///@brief   Convert the internal time representation to a GMT time tm struct.   
//------------------------------------------------------------------------------
tm* DateTime::gmtime(tm& t) const
{
    time_t temp = tv_sec;
    gmtime_r(&temp, &t);
    return &t;
}

//------------------------------------------------------------------------------
///@brief   Convert the time to a string.                                       
///@param   format      What fields should be placed in the string.  The field  
///                     codes are identical to strftime (3 guesses as to why)   
///                     with the addition of %%f (double percent, ef) to include
///                     the fractions of seconds.                               
///                     E.g., "%F %H:%M:%S.%%6f" will produce a result similar  
///                     to "2011-01-13 16:29:42.654321".                        
///@param   location    Location indicates if local time or UTC time should be  
///                     used in the string conversion.                          
//------------------------------------------------------------------------------
std::string DateTime::toString(
    const std::string& format
  , TIME_LOCATION location
) const
{
    struct tm t;

    get_time(location, t);

    //--------------------------------------------------------------------------
    //  This piece of idiocy is brought to you by Microsoft -- the company that 
    //  doesn't believe in standards.  In this case C99.                        
    //--------------------------------------------------------------------------
    std::string mins(format);
    size_t f;
    while ((f = mins.find("%F")) != std::string::npos) {
        mins.replace(f, 2, "%Y-%m-%d");
    }

    char buffer[1024];
    strftime(buffer, sizeof(buffer), mins.c_str(), &t);

    //--------------------------------------------------------------------------
    //  There is a better way to do this ... I'm horribly lazy right now.       
    //--------------------------------------------------------------------------
    int size = 0;
    int state = 0;
    int start = 0;
    int percent_start = 0;
    int end = 0;

    for (int i=0; state != 3 && buffer[i]; ++i) {
        if (state == 0 && buffer[i] == '%') {
            state = 1;
            size = 0;
            percent_start = i;

        } else if (state == 1 && isdigit(buffer[i])) {
            state = 2;
            size = size * 10 + buffer[i] - '0';

        } else if (state == 1) {
            state = 0;
            --i;

        } else if (state == 2 && isdigit(buffer[i])) {
            size = size * 10 + buffer[i] - '0';
            
        } else if (state == 2 && buffer[i] == 'f') {
            start = percent_start;
            state = 3;
            end = i;

        } else if (state == 2) {
            state = 0;
            --i;
        }
    }

    std::string result(buffer);

    if (state == 3) {
        static const double multiple[] = {
            1. / 1000000000     // 0
          , 1. / 100000000      // 1
          , 1. / 10000000       // 2
          , 1. / 1000000        // 3
          , 1. / 100000         // 4
          , 1. / 10000          // 5
          , 1. / 1000           // 6
          , 1. / 100            // 7
          , 1. / 10             // 8
          , 1.                  // 9
          , 10.                 //10
          , 100.                //11
          , 1000.               //12
          , 10000.              //13
          , 100000.             //14
          , 1000000.            //15
          , 10000000.           //16
        };

        char format[20];
        sprintf(format, "%%0%dld", size);
        
        sprintf(buffer, format, int64_t(tv_nsec * multiple[size]));

        result = result.substr(0, start) 
            + buffer 
            + result.substr(end+1, result.size())
        ;
    }

    return result;

} // std::string DateTime::toString(const std::string& format) const

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string DateTime::toString(
    TIME_LOCATION location
  , const std::string& format
) const 
{
    return toString(format, location);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string DateTime::toString(
    TIME_LOCATION location
  , int precision
) const
{
    return toString(precision, location);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string DateTime::toString(
    int precision
  , TIME_LOCATION location
) const
{
    return toString(
        "%F %H:%M:%S.%%" + lib::format("%u"
      , precision) + "f", location
    );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string DateTime::toString(double seconds, const std::string& fmt)
{
    DateTime temp(seconds);
    return temp.toString(fmt);
}

//------------------------------------------------------------------------------
///@brief   Cast the DateTime object into a double.                             
//------------------------------------------------------------------------------
DateTime::operator double() const
{
    //--------------------------------------------------------------------------
    //  Evidently some compilers will treat:    1 + 2.5 as an integer while     
    //  other compilers treat the result as a double.                           
    //--------------------------------------------------------------------------
    return double(tv_sec) + double(tv_nsec) / double(lib::si::ds::nano);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DateTime::get_time(TIME_LOCATION location, tm& t) const
{
    switch(location) {
        case GMT:   gmtime(t);      break;
        case LOCAL: localtime(t);   break;
    }
}

//------------------------------------------------------------------------------
///@brief   Return true if the given string represents a valid date.            
///@warning The initial implementation only works for ddmmmyy[yy] format.  If   
///         you need another format, please add it.                             
//------------------------------------------------------------------------------
bool DateTime::isValid(const std::string& str)
{
    DateTime temp;
    return temp.fromString(str, true);
#if 0
    static const char* DDMMMYYYY_formats[] = 
    {
        "^(\\d{2})jan(\\d{4})$"
      , "^(\\d{2})feb(\\d{4})$"
      , "^(\\d{2})mar(\\d{4})$"
      , "^(\\d{2})apr(\\d{4})$"
      , "^(\\d{2})may(\\d{4})$"
      , "^(\\d{2})jun(\\d{4})$"
      , "^(\\d{2})jul(\\d{4})$"
      , "^(\\d{2})aug(\\d{4})$"
      , "^(\\d{2})sep(\\d{4})$"
      , "^(\\d{2})oct(\\d{4})$"
      , "^(\\d{2})nov(\\d{4})$"
      , "^(\\d{2})dec(\\d{4})$"
      , NULL
    };

    //               jan feb mar apr may jun jul aug sep oct nov dec
    int max_day[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool result = false;

    int day = 0;
    int month = -1;

    boost::smatch p;
    for (size_t f=0; DDMMMYYYY_formats[f] && !result; ++f) {
        boost::regex r(DDMMMYYYY_formats[f], boost::regex::icase);

        result = (boost::regex_match(str, p, r));

        if (result) {
            month = f;
            day  = lib::toUnsigned(p[1]);
            year = lib::toUnsigned(p[2]);
        }
    }

    if (result) {
        result = ((day > 0) && (day <= max_day[month]));
    }

    return result;

    #if REMOVED_BY_DHF_2014_09_24
    std::string::const_iterator s;

    std::string str_month;
    std::string str_date;
    std::string str_year;

    int month;
    int date;
    int year;

    bool result = false;

    enum STATE {
        STATE_YEAR
      , STATE_MONTH
      , STATE_DATE
      , STATE_HOUR
      , STATE_MINUTE
      , STATE_SECOND
      , STATE_FRACTION
      , STATE_ERROR
    } state;

    switch(format)
    {
        case FMT_DDMMMYYYY:
            state = STATE_DATE;

            for (s = str.begin(); s != str.end() && state != STATE_ERROR; ++s) {
                switch(state) {
                    case STATE_DATE:
                        if (isdigit(*s)) {
                            str_date += *s;
                        } else if (isalpha(*s)) {
                            state = STATE_MONTH;
                            str_month += char(*s | 0x20);
                        } else {
                            state = STATE_ERROR;
                        }
                        break;

                    case STATE_MONTH:
                        if (isalpha(*s)) {
                            str_month += char(*s | 0x20);
                        } else if (isdigit(*s)) {
                            str_year += *s;
                            state = STATE_YEAR;
                        } else {
                            state = STATE_ERROR;
                        }
                        break;

                    case STATE_YEAR:
                        if (isdigit(*s)) {
                            str_year += *s;
                        } else {
                            state = STATE_ERROR;
                        }
                        break;

                    default:
                        break;
                            
                } // switch(state)  

            } // for (...) 

            //------------------------------------------------------------------
            //  Check the month                                                 
            //------------------------------------------------------------------
            std::string months = "jan,feb,mar,apr,may,jun,"
                                 "jul,aug,sep,oct,nov,dec,";
            size_t pos = months.find(str_month + ",");

            if (pos == std::string::npos) {
                result = false;
                month = 0;
            } else {
                result = true;
                month = (pos / 4);
            }

            //------------------------------------------------------------------
            //  Check the year.                                                 
            //------------------------------------------------------------------
            year = toUnsigned(str_year);

            if (result) {
                result = str_year.length() == 4 
                       && (year >= minimum().year() && year < maximum().year());

            }

            //------------------------------------------------------------------
            //  Check the date range.                                           
            //------------------------------------------------------------------
            date = toUnsigned(str_date);

                                         // "jan,feb,mar,apr,may,jun,"          
                                        // "jul,aug,sep,oct,nov,dec,";          
            int leap_day = ((year % 4) == 0 && month == 1) ? 1 : 0;
            if (result) {
                int max = monthLength(month) + leap_day;

                result = (str_date.length() == 2 && date >= 1 && date <= max);
            }
                
            break;

    } // switch(format)

    return result;
    #endif
#endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DateTime::truncateSecondsTo(uint64_t si_units)
{
    assert(si_units != 0);
    tv_nsec = tv_nsec / (lib::si::ds::nano / si_units);
    tv_nsec = tv_nsec * (lib::si::ds::nano / si_units);
}

//------------------------------------------------------------------------------
///@brief   Set the value to the given year, yday, hour, min, second, and       
///         nano seconds.                                                       
///@param   year    The year of the date time;  since the calendar does not     
///                 include a year 0, a year 0 is treated as baseline().year()  
///                 (Probably 1970).                                            
///@param   yday    The day of the year where January 1st is 0.                 
///@param   hour    The hour of the day.                                        
///@param   min     The minute of the hour.                                     
///@param   sec     The second of the minute.                                   
///@param   nano_seconds    Nano seconds.                                       
//------------------------------------------------------------------------------
bool DateTime::set(
    int year
  , int yday
  , int hour
  , int min
  , int sec
  , int64_t nano_seconds
)
{
    if (year == 0) year = baseline().year();         /// @todo discuss

    //--------------------------------------------------------------------------
    //  Only want to count leap years that have had the leap year pass ...      
    //  1972 is the first leap year after epoch (1970).  So, (year - 1969)/4 is 
    //  the number of leap years that have already passed.  (In 1972, no leap   
    //  years have passed, in 1973 one has passed (1973-1969 == 4); 4 /4 == 1;  
    //                                                                          
    //  We don't really care about any dates except 1990-2090.                  
    //                                                                          
    //  NOTE:   1970 and after has a positive second value; before 1970 is      
    //          negative.                                                       
    //--------------------------------------------------------------------------
    if (year >= baseline().year()) {
        uint64_t leap_years = 0;
        for (int y=1972; y < year; y += 4) {
            if (isLeapYear(y)) ++leap_years;
        }

        uint64_t days = (year - baseline().year()) * 365 + leap_years + yday;

        tv_sec = days * ds::secondsPerDay 
               + hour * ds::secondsPerHour 
               + min  * ds::secondsPerMinute 
               + sec
        ;
        tv_nsec = nano_seconds;

    } else {
        //----------------------------------------------------------------------
        //  Before 1970 is represented by 'seconds before 1/1/1970';            
        //       -1 is 12/31/1969 23:59:59                                      
        //   -86400 is 12/30/1969 00:00:00                                      
        //                                                                      
        //  Nano seconds are always non-negative values.                        
        //----------------------------------------------------------------------
        int64_t leap_years = 0;
        for (int y=1968; y >= year; y -= 4) {
            if (isLeapYear(y)) ++leap_years;
        }

        int64_t days = (year - baseline().year()) * 365 - leap_years + yday;

        tv_sec  = days * ds::secondsPerDay
                + hour * ds::secondsPerHour
                + min  * ds::secondsPerMinute
                + sec
        ;
        tv_nsec = nano_seconds;

    }

    setSmoothed(false);

    return true;
}

//------------------------------------------------------------------------------
///@brief   Return the number of days within the given month (where 0 = January)
///@note    February is always 28 --- this routine does not account for leap    
///         years.                                                              
///@param   month   An integer representing the month to return.  0 = January,  
///                 11 = December.                                              
///@return          The number of days in the indicated month.                  
//------------------------------------------------------------------------------
int DateTime::monthLength(uint32_t month)
{
    if (month > 11) throw std::range_error("Month > 11");
    int month_len[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return month_len[month];
}

//------------------------------------------------------------------------------
///@brief   Change this object to the value indicated by the parameters.        
///                                                                             
///@sa      set                                                                 
///                                                                             
///@note    No range checks are made on the various values.  GIGO.              
///                                                                             
///@param   year    The year of the date time (0 = baseline().year()).          
///@param   month   The number of the month, January = 0, December = 11         
///@param   mday    The day of the month.                                       
///@param   hour    The hour of the day (0-23).                                 
///@param   min     The minute of the hour (0-59).                              
///@param   sec     The second of the minute (0-59).                            
///@param   nano_seconds    The nano second value.                              
//------------------------------------------------------------------------------
bool DateTime::setYMD(
    int year
  , int month
  , int mday
  , int hour
  , int min
  , int sec
  , int64_t nano_seconds
)
{
    //--------------------------------------------------------------------------
    //  Compute the day of year.                                                
    //--------------------------------------------------------------------------
    int yday = mday-1;
    for (int m=0; m < month; ++m) yday += monthLength(m);

    if (isLeapYear(year) && month > 1) ++yday;

    return set(year, yday, hour, min, sec, nano_seconds);
}

} // namespace work //
} // namespace time //
} // namespace lib //

//==============================================================================
//      Arithmetic operators belong outside the namespace.                      
//==============================================================================

//------------------------------------------------------------------------------
///@brief   A delta-time plus a delta-time is a delta-time.                     
//------------------------------------------------------------------------------
lib::time::work::DeltaTime operator+(
    const lib::time::work::DeltaTime& left
  , const lib::time::work::DeltaTime& right
)
{
    lib::time::work::DeltaTime result;

    result.tv_sec  = left.tv_sec  + right.tv_sec;
    result.tv_nsec = left.tv_nsec + right.tv_nsec;

    result.adjust();

    result.setSmoothed(left.isSmoothed() || right.isSmoothed());

    return result;
}

//------------------------------------------------------------------------------
///@brief   A date-time plus a delta-time is a date-time.                       
//------------------------------------------------------------------------------
lib::time::work::DateTime operator+(
    const lib::time::work::DateTime& left
  , const lib::time::work::DeltaTime& right
)
{
    lib::time::work::DateTime result;
    result.tv_sec = left.tv_sec + right.tv_sec;
    result.tv_nsec = left.tv_nsec + right.tv_nsec;

    result.adjust();
    
    result.setSmoothed(left.isSmoothed() || right.isSmoothed());

    return result;
}

//------------------------------------------------------------------------------
///@brief   A date-time minus a date-time is a delta-time.                      
//------------------------------------------------------------------------------
lib::time::work::DeltaTime operator-(
    const lib::time::work::DateTime& left
  , const lib::time::work::DateTime& right
)
{
    lib::time::work::DeltaTime result;

    result.tv_sec = left.tv_sec - right.tv_sec;
    result.tv_nsec = left.tv_nsec - right.tv_nsec;

    result.adjust();

    result.setSmoothed(left.isSmoothed() || right.isSmoothed());

    return result;
}

//------------------------------------------------------------------------------
///@brief   A date-time minus a delta-time is a date-time.                      
//------------------------------------------------------------------------------
lib::time::work::DateTime operator-(
    const lib::time::work::DateTime& left
  , const lib::time::work::DeltaTime& right
)
{
    lib::time::work::DateTime result;
    result.tv_sec = left.tv_sec - right.tv_sec;
    result.tv_nsec = left.tv_nsec - right.tv_nsec;

    result.adjust();

    result.setSmoothed(left.isSmoothed() || right.isSmoothed());

    return result;
}

//------------------------------------------------------------------------------
///@brief   A delta-time minus a delta-time is a delta-time.                    
//------------------------------------------------------------------------------
lib::time::work::DeltaTime operator-(
    const lib::time::work::DeltaTime& left
  , const lib::time::work::DeltaTime& right
)
{
    lib::time::work::DeltaTime result;

    result.tv_sec = left.tv_sec - right.tv_sec;
    result.tv_nsec = left.tv_nsec - right.tv_nsec;

    result.adjust();

    result.setSmoothed(left.isSmoothed() || right.isSmoothed());

    return result;
}

