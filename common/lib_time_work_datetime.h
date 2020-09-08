//------------------------------------------------------------------------------
///@file lib_time_work_datetime.h                                               
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_TIME_WORK_DATETIME_H
#define LIB_TIME_WORK_DATETIME_H

#include "lib_time_work_datedeltatimebase.h"
#include "lib_time_work_deltatime.h"

#include <iostream>
#include <stdint.h>
#include <string>
#include <time.h>
//#ifndef IS_VISUAL_STUDIO
//#include <pthread.h>        // mingw needs for timespec definition
//#endif

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
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
///@par Expected Usage:                                                         
///     @code                                                                   
///         lib::time::work::DateTime beg(lib::time::work::DateTime::minimum());
///         lib::time::work::DateTime end(lib::time::work::DateTime::maximum());
///                                                                             
///         if (!beg.fromString(commandLine["start-time"].string())) {          
///             //--------------------------------------------------------------
///             //  Handle poorly formed time string.                           
///             //--------------------------------------------------------------
///         }                                                                   
///                                                                             
///         if (!end.fromString(commandLine("stop-time"].string()))) {          
///             //--------------------------------------------------------------
///             //  Handle poorly formed time string.                           
///             //--------------------------------------------------------------
///         }                                                                   
///                                                                             
///         if (end < beg) {                                                    
///             //--------------------------------------------------------------
///             //  Handle stop time before start time.                         
///             //--------------------------------------------------------------
///         }                                                                   
///     @endcode                                                                
///                                                                             
///@par Expected Usage (Take II):                                               
///     @code                                                                   
///         std::cout                                                           
///             << "packet time is "                                            
///             << packet->m_AbsoluteTime.toString("%j %H:%D:%M.%%6f")          
///             << std::endl                                                    
///         ;                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-05-26  YBS     Added operator <<.                              
///                                                                             
///@version 2015-03-03  DHF     Make get_time public.                           
///                                                                             
///@version 2014-11-03  DHF     Added toString(size_t precision);               
///                                                                             
///@version 2012-04-17  DHF     Added static toString(...) method.              
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-11-16  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class DateTime : public lib::time::work::DateDeltaTimeBase
{
    public:
        DateTime() {}
        explicit DateTime(const std::string& str);
        explicit DateTime(double sec) { setSeconds(sec); }
        DateTime(const DateTime& t) { tv_sec = t.tv_sec; tv_nsec = t.tv_nsec;}
        virtual ~DateTime() {}

        DateTime& operator=(const DateTime& that);
        DateTime& operator=(const std::string& that);

        bool operator<(const DateTime& right) const;
        bool operator==(const DateTime& right) const;
        inline bool operator!=(const DateTime& r) {return !(*this == r);}
        inline bool operator<=(const DateTime& r) {return *this<r || *this==r;}

        operator double() const;

        static DateTime maximum();
        static DateTime minimum();
        static DateTime baseline();

        static DateTime now();

        enum TIME_LOCATION {
            GMT
          , LOCAL
        };

        virtual int yday(TIME_LOCATION location = GMT) const;
        virtual int mday(TIME_LOCATION location = GMT) const;
        virtual int month(TIME_LOCATION location = GMT) const;
        virtual int year(TIME_LOCATION location = GMT) const;

        virtual int hour() const;
        virtual int min() const;
        virtual int sec() const;
        virtual int millisec() const;
        virtual int microsec() const;
        virtual int nanosec() const;
        virtual double secondsSinceMidNight() const;

        bool set(
            int year
          , int yday
          , int hour
          , int min
          , int sec
          , int64_t nano_seconds
        );

        bool setYMD(
            int year
          , int month
          , int mday
          , int hour
          , int min
          , int sec
          , int64_t nano_seconds
        );

        std::string toString(
            const std::string& fmt="%F %H:%M:%S.%%6f"
          , TIME_LOCATION location = GMT
        ) const;
        std::string toString(
            TIME_LOCATION location
          , const std::string& fmt="%F %H:%M:%S.%%6f"
        ) const;

        std::string toString(
            int precision
          , TIME_LOCATION location = GMT
        ) const;
        std::string toString(
            TIME_LOCATION location
          , int precision
        ) const;

        static std::string toString(
            double seconds
          , const std::string& fmt="%F %H:%M:%S.%%6f"
        );

        tm* localtime(tm& t) const;
        tm* gmtime(tm& t) const;

        enum FORMAT {
            FMT_DDMMMYYYY
        };
        static bool isValid(const std::string& str);

        void truncateSecondsTo(uint64_t si_units);

        bool fromString(const std::string& str, bool strict = false);

        inline static bool isLeapYear(int year)
        { return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0); }

        //  0 = January; 11 = December  
        static int monthLength(uint32_t month);

        void get_time(TIME_LOCATION location, tm& t) const;

    protected:

    private:

}; // class DateTime //

} // namespace work //
} // namespace time //
} // namespace lib //

lib::time::work::DeltaTime operator+(
    const lib::time::work::DeltaTime& left
  , const lib::time::work::DeltaTime& right
);
lib::time::work::DateTime  operator+(
    const lib::time::work::DateTime&  left
  , const lib::time::work::DeltaTime& right
);
inline lib::time::work::DateTime  operator+(
    const lib::time::work::DeltaTime& left
  , const lib::time::work::DateTime&  right
) { return right + left; }

lib::time::work::DeltaTime operator-(
    const lib::time::work::DeltaTime& left
  , const lib::time::work::DeltaTime& right
);
lib::time::work::DeltaTime operator-(
    const lib::time::work::DateTime&  left
  , const lib::time::work::DateTime&  right
);
lib::time::work::DateTime  operator-(
    const lib::time::work::DateTime& left
  , const lib::time::work::DeltaTime& right
);

inline std::ostream& operator <<(
    std::ostream& out
  , const lib::time::work::DateTime& dateTime
)
{
    return out << dateTime.toString();
}

#endif // #ifndef LIB_DATETIME_H //
