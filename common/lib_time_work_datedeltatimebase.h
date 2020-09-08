//------------------------------------------------------------------------------
///@file lib_time_work_datedeltatimebase.h                                      
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_TIME_WORK_DATEDELTATIMEBASE_H_FILE_GUARD
#define LIB_TIME_WORK_DATEDELTATIMEBASE_H_FILE_GUARD

#include <stdint.h>
#include <time.h>
#ifndef IS_VISUAL_STUDIO
#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
#include <pthread.h>        // mingw needs for timespec definition
#endif
#endif

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: DateDeltaTimeBase                                                
///                                                                             
///@brief   Base class used by lib::DateTime and lib::DeltaTime.                
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///         Frankly, you're not expected to use this class.  Only DateTime and  
///         DeltaTime should be using it.                                       
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-08-02  DHF     Renamed seconds to inSeconds.                   
///                             Added inMicroSeconds()                          
///                             Added inMilliSeconds()                          
///                             Added inNanoSeconds()                           
///                                                                             
///@version 2014-10-01  DHF     Break out class from lib_datetime.h             
///                                                                             
//------------------------------------------------------------------------------
class DateDeltaTimeBase : public timespec
{
    public:
        DateDeltaTimeBase() : m_IsSmoothed(false) {tv_sec = tv_nsec = 0;}
        DateDeltaTimeBase(const DateDeltaTimeBase& that) = default;

        DateDeltaTimeBase& operator=(const DateDeltaTimeBase& that) = default;

        void setNanoSeconds(int64_t nano);
        void getNanoSeconds(int64_t& nano) const;
        void setSeconds(double seconds);

        void adjust();

        double inSeconds() const;
        double inMicroSeconds() const;
        double inMilliSeconds() const;
        double inNanoSeconds() const;

        bool isSmoothed() const;
        void setSmoothed(bool smoothed = true);

    private:
        bool    m_IsSmoothed;

}; // class DateDeltaTimeBase //

} // namespace work
} // namespace time
} // namespace lib

#endif // #ifndef LIB_TIME_WORK_DATEDELTATIMEBASE_H_FILE_GUARD

