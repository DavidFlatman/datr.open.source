//------------------------------------------------------------------------------
///@file lib_time_work_datedeltatimebase.cpp                                    
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Base class used by lib::DateTime and lib::DeltaTime.                
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-10-01  DHF     Break out class from lib_datetime.cpp           
///                                                                             
//------------------------------------------------------------------------------

#include "lib_time_work_datedeltatimebase.h"
#include "lib_si_ds_prefixes.h"

#include <math.h>               // floor, round                                 

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///@brief   Adjust the date/time that has a negative nsec.                      
//------------------------------------------------------------------------------
void DateDeltaTimeBase::adjust()
{

    while (int64_t(tv_nsec) < 0) {
        --tv_sec;
        tv_nsec += lib::si::ds::nano;
    }

    while (tv_nsec >= int64_t(lib::si::ds::nano)) {
        ++tv_sec;
        tv_nsec -= lib::si::ds::nano;
    }

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DateDeltaTimeBase::setNanoSeconds(int64_t nano)
{
    if (nano < 0) {
        nano *= (-1);

        tv_sec = (nano / lib::si::ds::nano) * (-1);
        tv_nsec = (nano % lib::si::ds::nano) * (-1);

        adjust();

    } else {
        tv_sec = nano / lib::si::ds::nano;
        tv_nsec = nano % lib::si::ds::nano;

    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DateDeltaTimeBase::getNanoSeconds(int64_t& nano) const
{
    nano = 0;
    if (tv_sec < 0) {
        int64_t temp_sec = tv_sec * (-1);
        int64_t temp_nsec = tv_nsec;
        if (tv_nsec < 0) temp_nsec = tv_nsec * (-1);

        nano = temp_sec * lib::si::ds::nano + temp_nsec;
        nano = nano * (-1);
    } else {
        nano = tv_sec * lib::si::ds::nano + tv_nsec;
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DateDeltaTimeBase::setSeconds(double seconds)
{
    if (seconds == 0) {
        tv_sec = tv_nsec = 0;

    } else if (seconds > 0) {
        tv_sec = floor(seconds);
        tv_nsec = round((seconds - tv_sec) * lib::si::ds::nano);

    } else {
        seconds *= -1;
        tv_sec = floor(seconds);
        tv_nsec = round((seconds - tv_nsec) * lib::si::ds::nano);

        tv_sec *= -1;
        tv_nsec *= -1;
    }
}

//------------------------------------------------------------------------------
///@brief   Return the value in seconds.                                        
//------------------------------------------------------------------------------
double DateDeltaTimeBase::inSeconds() const
{
    return double(tv_sec) + (double(tv_nsec) / double(lib::si::ds::nano));
}

//------------------------------------------------------------------------------
///@brief   Return the value in micro seconds.                                  
//------------------------------------------------------------------------------
double DateDeltaTimeBase::inMicroSeconds() const
{
    return inSeconds() * double(lib::si::ds::micro);
}

//------------------------------------------------------------------------------
///@brief   Return the value in milli seconds.                                  
//------------------------------------------------------------------------------
double DateDeltaTimeBase::inMilliSeconds() const
{
    return inSeconds() * lib::si::ds::milli;
}

//------------------------------------------------------------------------------
///@brief   Return the value in nano seconds.                                   
//------------------------------------------------------------------------------
double DateDeltaTimeBase::inNanoSeconds() const
{
    return inSeconds() * lib::si::ds::nano;
}

//------------------------------------------------------------------------------
///@brief   Return true if the time is considered smoothed.                     
//------------------------------------------------------------------------------
bool DateDeltaTimeBase::isSmoothed() const
{
    return m_IsSmoothed;
}

//------------------------------------------------------------------------------
///@brief   Set whether the time is smoothed or not.                            
//------------------------------------------------------------------------------
void DateDeltaTimeBase::setSmoothed(bool smoothed)
{
    m_IsSmoothed = smoothed;
}


} // namespace work //
} // namespace time //
} // namespace lib //

