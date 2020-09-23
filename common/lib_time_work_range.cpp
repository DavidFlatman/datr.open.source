//------------------------------------------------------------------------------
///@file lib_time_work.cpp                                               
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------

#include "lib_time_work_range.h"
#include <iostream>
#include "lib_log_message.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///@par     Class:Range
///                                                                            
///@brief   Implementation of lib_time_work_range.h
///                                                                             
///@par Thread Safety:  class                                              
///                                                                             
///@version 2015-07-08  MNT     Replaced DateTime by InstrumentationTime
///@version 2014-11-??  MNT     Initial version
///@version 2014-08-27  MNT     File creation                                   
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Default Constructor that sets startTime and stopTime to DataTime's
/// min and max values respectively.
///
/// The flags shall initially indicate that no startTime or stopTime has been
/// set. They are therefore set to false;
//------------------------------------------------------------------------------
Range::Range()
{
    m_StartTime = lib::time::work::InstrumentationTime::minimum();
    m_StopTime  = lib::time::work::InstrumentationTime::maximum();
    m_StartTimeIsSet = false;
    m_StopTimeIsSet = false;
}

//------------------------------------------------------------------------------
/// @brief Copy Constructor. Copies all class internal parameters.
///
/// @param that Reference to object to copy class internal parameters from.
//------------------------------------------------------------------------------
Range::Range(const Range& that)
{   m_StartTime         =   that.m_StartTime;
    m_StartTimeIsSet    =   that.m_StartTimeIsSet;
    m_StopTime          =   that.m_StopTime;
    m_StopTimeIsSet     =   that.m_StopTimeIsSet;
}

//------------------------------------------------------------------------------
/// @brief  Equal operator. Compares all class internal parameters.
///
/// @param that Reference to object to compare class internal parameters to.
//------------------------------------------------------------------------------
bool Range::operator==(const Range& right) const
{
    return(m_StartTime         ==   right.m_StartTime)
       && (m_StartTimeIsSet    ==   right.m_StartTimeIsSet)
       && (m_StopTime          ==   right.m_StopTime)
       && (m_StopTimeIsSet     ==   right.m_StopTimeIsSet);
}

//------------------------------------------------------------------------------
/// @brief setStartTime Tries to set startTime to the one given as a parameter.
/// However, if the stopTime has been set and it's time value is earlier
/// or equal to this new startTime, the setting fails. A failed setting
/// results in a false being returned and a succeeded setting results in
/// a true being returned. Also, if the setting succeeds, a parameter,
/// m_StartTimeIsSet, is set to true to make it possible for setStopTime
/// to perform similar logic from its point of view.
///
/// @param start    The new InstrumentationTime to set.
//------------------------------------------------------------------------------
bool Range::setStartTime(const lib::time::work::InstrumentationTime& start)
{
    if(!m_StopTimeIsSet
      || start < m_StopTime)
    {
         m_StartTime = start;
        m_StartTimeIsSet = true;
        return true;
    } else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/// @brief setStopTime tries to set stopTime to the one given as a parameter.
/// However, if the startTime has been set and it's time value is later
/// or equal to this new stopTime, the setting fails. A failed setting
/// results in a false being returned and a succeeded setting results in
/// a true being returned. Also, if the setting succeeds, a parameter,
/// m_StopTimeIsSet, is set to true to make it possible for setStartTime
/// to perform similar logic from its point of view.
///
/// @param stop     The new InstrumentationTime to set.
//------------------------------------------------------------------------------
bool Range::setStopTime(const lib::time::work::InstrumentationTime& stop)
{
    if(!m_StartTimeIsSet
      || stop > m_StartTime)
    {
        m_StopTime = stop;
        m_StopTimeIsSet = true;
        return true;
    } else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/// @brief setRange tries to set startTime and stopTime to the ones given as
/// parameters. However, if the startTime parameter is anything but earlier
/// in time than the stopTime parameter, the setting fails and a false is
/// returned. Otherwise, both parameters are set and both flags indicating
/// that the parameters have been set are set to true, and finally a true is
/// being returned.
///
/// @param start    The new InstrumentationTime to set.
/// @param stop     The new InstrumentationTime to set.
//------------------------------------------------------------------------------
bool Range::setRange(const lib::time::work::InstrumentationTime& start
                    , const lib::time::work::InstrumentationTime& stop)
{
    if(start < stop)
    {
        m_StartTime = start;
        m_StartTimeIsSet = true;
        m_StopTime = stop;
        m_StopTimeIsSet = true;
        return true;
    } else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/// @brief Return the boolean value of the flag indicating whether startTime
/// has been set.
//------------------------------------------------------------------------------
bool Range::isStartTimeSet() const
{
    return m_StartTimeIsSet;
}

//------------------------------------------------------------------------------
/// @brief Return the boolean value of the flag indicating whether stopTime
/// has been set.
//------------------------------------------------------------------------------
bool Range::isStopTimeSet() const
{
    return m_StopTimeIsSet;
}

//------------------------------------------------------------------------------
/// @brief Return the result of performing a logic AND on the boolean variables
/// indicating whether startTime and stopTime have been set.
/// That answers the question: Has both startTime and stopTime been set?
//------------------------------------------------------------------------------
bool Range::isStartAndStopTimesSet() const
{
    return m_StartTimeIsSet && m_StopTimeIsSet;
}

//------------------------------------------------------------------------------
/// @brief Return the value of the startTime variable.
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime  Range::startTime() const
{
    return m_StartTime;
}

//------------------------------------------------------------------------------
/// @brief Return the value of the stopTime variable.
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime  Range::stopTime() const
{
    return m_StopTime;
}

//------------------------------------------------------------------------------
/// @brief Return a boolean answering the question:
/// Does the range contain/span the time in the provided parameter?
//
/// The logic relies on isBefore(...) and isAfter(...) and works as follows:
//
/// ((NOT before) AND (NOT after))
/// This is a perfect example of something that also could have been written:
/// NOT(before OR after)
//
/// Either way, this method returns true in the edge cases too, where the
/// parameter is equal to either of the range's edge times.
///
/// @param time The time which is to be compared to the Range's outer bounds.
//------------------------------------------------------------------------------
bool Range::contains(const lib::time::work::InstrumentationTime& time) const
{
    return (!this->isBefore(time) && !this->isAfter(time));
}

//------------------------------------------------------------------------------
/// @brief Return a boolean answering the question:
/// Is the entire range earlier, before, in time than the provided parameter?
/// The logic is:
//
/// If stopTime has been set, return the boolean result of "stopTime is
/// earlier than the provided time". Otherwise, return false.
///
/// @param time The time which is to be compared to the Range's stopTime.
//------------------------------------------------------------------------------
bool Range::isBefore(const lib::time::work::InstrumentationTime& time) const
{
    return (m_StopTimeIsSet) ? m_StopTime < time : false;
}

//------------------------------------------------------------------------------
/// @brief Return a boolean answering the question:
/// Is the entire range later, after, in time than the provided parameter?
/// The logic is:
//
/// If startTime has been set, return the boolean result of "startTime is
/// later than the provided time". Otherwise, return false.
///
/// @param time The time which is to be compared to the Range's startTime.
//------------------------------------------------------------------------------
bool Range::isAfter(const lib::time::work::InstrumentationTime& time) const
{
    return  (m_StartTimeIsSet) ? time < m_StartTime : false;
}

//------------------------------------------------------------------------------
/// @brief Converts the range to a human readable format, a std::string,
/// and returns it.
//
/// Following the format for closed intervals at
//
///     https:en.wikipedia.org/wiki/Interval_(mathematics)
//
/// the dates are bounded by "[" and "]" and separated by ", ".
//
/// However, we also have to cater to the cases when only one or no
/// parameters have been set. The default values are used and the
/// enclosed brackets "[" and "]" are replaced by "<" if the startTime
/// has not been set and ">" if the stopTime has not been set.
//
/// e.g.
//
/// Range is completely uninitialized
///     "<1903-01-01 00:00:00.000000, 9999-12-12 23:59:59.000000>"
//
/// Only start time is set.
///     "[2012-01-01 00:00:00.000000, 9999-12-12 23:59:59.000000>"
//
/// Only stop time is set.
///     "<1903-01-01 00:00:00.000000, 2012-01-01 00:00:00.000000]"
//
/// Range is completely initialized
///     "[2002-01-01 01:02:03.123456, 2012-05-06 12:13:14.111222]"
//------------------------------------------------------------------------------
std::string Range::toString() const
{    
    std::string out;
 
    out = (m_StartTimeIsSet) ? "[" : "<";
    out +=  m_StartTime.toString();
    out += ", ";
    out +=  m_StopTime.toString();
    out += (m_StopTimeIsSet) ? "]" : ">";
    return out;    
}
  
} // namespace work {
} // namespace time {
} // namespace lib {
