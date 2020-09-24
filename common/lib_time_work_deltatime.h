//------------------------------------------------------------------------------
///@file lib_time_work_deltatime.h                                              
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_TIME_WORK_DELTATIME_H_FILE_GUARD
#define LIB_TIME_WORK_DELTATIME_H_FILE_GUARD

#include "lib_time_work_datedeltatimebase.h"
#include <string>

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: DeltaTime                                                        
///                                                                             
///@brief   Class representing the different between two times.                 
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///         lib::time::work::DeltaTime  delta("15:00");                         
///         stopTime = startTime + delta;                                       
///     @endcode                                                                
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2017-03-20  DHF     Added operator+=, operator-=                    
///                                                                             
///@version 2014-10-08  MNT     Added parameter strict to fromString            
///                                                                             
///@version 2014-10-01  DHF     Break out class from lib_datetime.h             
///                                                                             
///@version 2014-09-24  DHF     Added DeltaTime:                                
///                                 -   fromString                              
///                                 -   isValid                                 
///                                 -   operator=(const std::string&)           
///                                                                             
//------------------------------------------------------------------------------
class DeltaTime : public lib::time::work::DateDeltaTimeBase
{
    public:
        DeltaTime() = default;
        DeltaTime(const DeltaTime& dt) = default;
        explicit DeltaTime(const std::string& time);
        explicit DeltaTime(double sec) { setSeconds(sec); }
        operator double() const;

        std::string toString(bool stopAtSeconds = false) const;

        static bool isValid(const std::string& time);
        bool fromString(const std::string& time, bool strict = false);
        DeltaTime& operator=(const DeltaTime& t);
        DeltaTime& operator=(const std::string& t);

        bool operator==(const DeltaTime& that) const;
        bool operator< (const DeltaTime& that) const;
        bool operator<=(const DeltaTime& that) const;

        DeltaTime& operator+=(const DeltaTime& that);
        DeltaTime& operator-=(const DeltaTime& that);
};


} // namespace work
} // namespace time
} // namespace lib

#endif // #ifndef LIB_TIME_WORK_DELTATIME_H_FILE_GUARD

