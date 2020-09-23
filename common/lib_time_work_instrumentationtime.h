//------------------------------------------------------------------------------
///@file lib_time_work_instrumentationtime.h                                    
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------

#ifndef LIB_TIME_WORK_INSTRUMENTATIONTIME_H_FILE_GUARD
#define LIB_TIME_WORK_INSTRUMENTATIONTIME_H_FILE_GUARD

#include "lib_time_work_datetime.h"
#include "lib_time_work_datedeltatimebase.h"
#include "lib_time_work_deltatime.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <string>

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: InstrumentationTime                                              
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
///@par Thread Safety:  class                                                   
///                                                                             
///@par Smoothed / isSmoothed                                                   
///         @code                                                               
///             InstrumentationTime it("123:12:34:56.1234");                    
///             assert(it.isSmoothed() == false);                               
///                                                                             
///             it.setSmoothed(true);                                           
///             assert(it.isSmoothed() == true);                                
///                                                                             
///             InstrumentationTime it2(it);                                    
///             assert(it.isSmoothed() == true);                                
///                                                                             
///             DeltaTime dt(it2 - it);                                         
///             assert(dt.isSmoothed() == true);                                
///                                                                             
///         @endcode                                                            
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///         lib::time::work::InstrumentationTime i_time(hot, lot, mot);         
///                                                                             
///         if (i_time.isSynced()) {                                            
///             //  The time is synchronized.                                   
///             lib::time::work::DateTime basis_time("2015-01-01");             
///             std::cout << i_time.toDateTime(basis_time).toString();          
///         } else {                                                            
///             //  The time is elapsed                                         
///             lib::time::work::DateTime basis_time("2015-03-05 12:30:01");    
///             std::cout << i_time.toDateTime(basis_time).toString();          
///         }                                                                   
///                                                                             
///         std::cout << "time is:  " << i_time.toString() << std::endl;        
///                                                                             
///     @endcode                                                                
///                                                                             
///@version 2016-05-17  YBS     Added smoothed data member and functions        
///                                                                             
///@version 2015-07-15  YBS     Added operator<<                                
///                                                                             
///@version 2015-07-07  DHF     Added operator!=                                
///                                                                             
///@version 2015-04-17  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class InstrumentationTime : public lib::time::work::DateDeltaTimeBase
{
    public:
        InstrumentationTime();
        InstrumentationTime(const InstrumentationTime& that);
        InstrumentationTime(uint16_t hot, uint16_t lot, uint16_t mot);

        explicit InstrumentationTime(double seconds);
        explicit InstrumentationTime(const DateTime& dt);
        explicit InstrumentationTime(const std::string& str);

        virtual ~InstrumentationTime();
    
        InstrumentationTime& operator=(const InstrumentationTime& that) = default;
        InstrumentationTime& operator=(const DateTime& that);
        InstrumentationTime& operator=(const std::string& that);
        InstrumentationTime& operator=(double seconds);

        DateTime toDateTime(const DateTime& basisTime) const;
        
        operator std::string() const;

        std::string toString(uint32_t fractionLength = 6) const;

        bool fromString(const std::string& str, bool strict = false);

        static bool isValid(const std::string& str);

        void fromCh4BCD   (uint16_t  hot, uint16_t  lot, uint16_t  mot);
        void fromCh4Binary(uint16_t  hot, uint16_t  lot, uint16_t  mot);

        void toCh4BCD   (uint16_t* hot, uint16_t* lot, uint16_t* mot) const;
        void toCh4Binary(uint16_t* hot, uint16_t* lot, uint16_t* mot) const;

        bool isSynced()  const;
        bool isElapsed() const;

        bool operator==(const InstrumentationTime& that) const;
        bool operator< (const InstrumentationTime& that) const;
        bool operator<=(const InstrumentationTime& that) const;
        inline bool operator> (const InstrumentationTime& that) const { return !(*this <= that); }
        inline bool operator>=(const InstrumentationTime& that) const { return !(*this <  that); }
        inline bool operator!=(const InstrumentationTime& that) const
        {
            return !(*this == that);
        }

        uint32_t jday() const;
        uint32_t hour() const;
        uint32_t min() const;
        uint32_t sec() const;
        uint64_t millisec() const;
        uint64_t microsec() const;
        uint64_t nanosec() const;

        InstrumentationTime& operator += (const DeltaTime& delta);
        InstrumentationTime& operator -= (const DeltaTime& delta);

        static InstrumentationTime maximum();
        static InstrumentationTime minimum();

    
    protected:

    private:

}; // class InstrumentationTime //

typedef boost::shared_ptr<InstrumentationTime> InstrumentationTimePtr;
typedef boost::shared_ptr<const InstrumentationTime> ConstInstrumentationTimePtr;

} // namespace work
} // namespace time
} // namespace lib

lib::time::work::InstrumentationTime operator+(
    const lib::time::work::InstrumentationTime& iTime
  , const lib::time::work::DeltaTime& dTime
);

lib::time::work::InstrumentationTime operator+(
    const lib::time::work::DeltaTime& dTime
  , const lib::time::work::InstrumentationTime& iTime
);

lib::time::work::InstrumentationTime operator-(
    const lib::time::work::InstrumentationTime& iTime
  , const lib::time::work::DeltaTime& dTime
);

lib::time::work::DeltaTime operator-(
    const lib::time::work::InstrumentationTime& iTime1
  , const lib::time::work::InstrumentationTime& iTime2
);

inline std::ostream&operator <<
(
    std::ostream& out, const lib::time::work::InstrumentationTime& time
) {
    return out << time.toString();
}

#endif // #ifndef LIB_TIME_WORK_INSTRUMENTATIONTIME_H_FILE_GUARD

