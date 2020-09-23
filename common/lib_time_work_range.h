//------------------------------------------------------------------------------
///@file lib_time_work_range.h                                               
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------

#ifndef LIB_TIME_WORK_RANGE_H_FILE_GUARD
#define LIB_TIME_WORK_RANGE_H_FILE_GUARD

#include "lib_time_work_instrumentationtime.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///@par Class: Range                                                    
///                                                                             
///@brief   Defines a time range by specifying start and stop times.
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@version 2015-07-08  MNT     Replaced DateTime by InstrumentationTime        
///                                                                             
///@version 2014-09-11  MNT     Updated after Detailed Design Review 3
///                             - Reversed change log to reversed chronological.
///                             - Removed assignment operator. Use the default.
///                             - Renamed methods to isBefore and isAfter
///                             - Changed how contains handles simultaneous lack
///                               of both start_time and stop_time parameters.
///                             - Minor rephrasings.
///@version 2014-09-10  MNT     Updated after Detailed Design Review 2
///                             - Shortened the file header.
///                             - Removed error message defionitions.
///                             - Added copy constructor.
///                             - Added =, ==, and != operators.
///                             - Improved member function documentations.
///                             - Made comments more Doxygen compliant.
///                             - General cleanup
///@version 2014-08-27  MNT     File creation                                   
//------------------------------------------------------------------------------

class Range
{
    public:

        //----------------------------------------------------------------------
        /// Default Constructor
        //----------------------------------------------------------------------
        Range();

        //----------------------------------------------------------------------
        /// Copy Constructor
        //----------------------------------------------------------------------
        Range(const Range& that);

        //----------------------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------------------
        virtual ~Range() {};

        //----------------------------------------------------------------------
        /// Equal operator
        //----------------------------------------------------------------------
        bool operator==(const Range& right) const;
        
        //----------------------------------------------------------------------
        /// Non-Equal operator
        //----------------------------------------------------------------------
        inline bool operator!=(const Range& r) {return !(*this == r);}

        //----------------------------------------------------------------------
        ///@brief   Set the start InstrumentationTime value.
        ///
        ///@param   start       The InstrumentationTime value to set.
        ///
        ///@return true if new start time is before old stop time.
        //----------------------------------------------------------------------
        bool setStartTime(const lib::time::work::InstrumentationTime& start);

        //----------------------------------------------------------------------
        ///@brief   Set the stop InstrumentationTime value.
        ///
        ///@param   stop        The InstrumentationTime value to set.
        ///
        ///@return true if new stop time is after old start time.
        //----------------------------------------------------------------------
        bool setStopTime(const lib::time::work::InstrumentationTime& stop);

        //----------------------------------------------------------------------
        ///@brief   Set both the start and stop InstrumentationTime values.
        ///
        ///@param   start       The InstrumentationTime start value to set.
        ///@param   stop        The InstrumentationTime stop  value to set.
        ///
        ///@return boolean, false if new start time is after or equal to new
        ///        stop time.
        //----------------------------------------------------------------------
        bool setRange(
            const lib::time::work::InstrumentationTime& start
          , const lib::time::work::InstrumentationTime& stop
        );

        //----------------------------------------------------------------------
        ///@brief   Tests if startTime has been set.
        ///
        ///@return  boolean, true if startTime has been set.
        //----------------------------------------------------------------------
        bool isStartTimeSet() const;

        //----------------------------------------------------------------------
        ///@brief   Tests if stopTime has been set.
        ///
        ///@return  boolean, true if stopTime has been set.
        //----------------------------------------------------------------------
        bool isStopTimeSet() const;

        //----------------------------------------------------------------------
        ///@brief   Tests if both startTime and stopTime have been set.
        ///
        ///@return  boolean, true if both startTime and stopTime have been set.
        //----------------------------------------------------------------------
        bool isStartAndStopTimesSet() const;

        //----------------------------------------------------------------------
        ///@brief   Returns the start time, beginning of range.
        ///
        ///@return  startTime.
        //----------------------------------------------------------------------
        lib::time::work::InstrumentationTime  startTime() const;

        //----------------------------------------------------------------------
        ///@brief   Returns the stop time, end of range.
        ///
        ///@return  stopTime.
        //----------------------------------------------------------------------
        lib::time::work::InstrumentationTime  stopTime() const;

        //----------------------------------------------------------------------
        ///@brief   Check if the range contains or is inclusively bound by
        ///         the time.
        ///        
        ///@note    Return true if the time is before or equal to the stopTime
        ///         and the startTime has not been set.
        ///
        ///         Return true if the time is after or equal to the startTime
        ///         and the stopTime has not been set.
        ///
        ///         Return true if neither startTime nor stopTime have been set.      
        ///
        ///@param   time to compare with the time range.                         
        ///
        ///@return  true if the range contains the time.
        //----------------------------------------------------------------------
        bool contains(const lib::time::work::InstrumentationTime& time) const;

        //----------------------------------------------------------------------
        ///@brief   Check if the range is strictly before the time.
        ///
        ///@param   time to compare with the time range                         
        ///
        ///@note    Always returns false if no StopTime has been set.
        ///
        ///@return  true if the stopTime is earlier than the given time.
        //----------------------------------------------------------------------
        bool isBefore(const lib::time::work::InstrumentationTime& time) const;

        //----------------------------------------------------------------------
        ///@brief   Check if the range is strictly after the time
        ///
        ///@param   time to compare with the time range                         
        ///
        ///@note    Always returns false if no StartTime has been set.
        ///
        ///@return  true if the startTime is later than the given time.
        //----------------------------------------------------------------------
        bool isAfter(const lib::time::work::InstrumentationTime& time) const;

        //TQ:   I've updated this file + .cpp but is that correct? The 
        //      description of the method below lists years. I guess it's only
        //      the description here and in the .cpp file that needs to be
        //      updated to remove years?
        
        //----------------------------------------------------------------------
        ///@brief   Converts the range to std::String.
        ///
        ///@note    Default syntax:
        ///     [1990-01-01 12:00:00.999999, 1990-01-01 14:00:00.999999]
        ///         If a time value has not been specified, the [ or ] next
        ///         to it is replaced by < or > respectively.
        ///         E.g. if there is no stopTime:
        ///     [1990-01-01 12:00:00.999999, 9999-12-12 23:59:59.000000>
        ///
        ///@return  the String
        //----------------------------------------------------------------------
        std::string toString() const;
        
    protected:

    private:

        //----------------------------------------------------------------------
        ///     Stores the time indicating the start of the range
        //----------------------------------------------------------------------
        lib::time::work::InstrumentationTime    m_StartTime;
                
        //----------------------------------------------------------------------
        ///     Indicates if m_StartTime has been set       
        //----------------------------------------------------------------------
        bool                                    m_StartTimeIsSet;

        //----------------------------------------------------------------------
        ///     Stores the time indicating the stop/end of the range
        //----------------------------------------------------------------------
        lib::time::work::InstrumentationTime    m_StopTime;

        //----------------------------------------------------------------------
        ///     Indicates if m_StopTime has been set
        //----------------------------------------------------------------------
        bool                                    m_StopTimeIsSet;        

};

} // namespace work {
} // namespace time {
} // namespace lib {

#endif // #ifndef LIB_TIME_WORK_RANGE_H_FILE_GUARD
