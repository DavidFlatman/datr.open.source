//------------------------------------------------------------------------------
///@file lib_time_work_streaksegment.h                                          
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------

#ifndef LIB_TIME_WORK_STREAKSEGMENT_H_FILE_GUARD
#define LIB_TIME_WORK_STREAKSEGMENT_H_FILE_GUARD


#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "lib_compiler_info.h"
#include "lib_log_ds.h"
#include "lib_log_message.h"
//#include "lib_log_work_exception.h"
#include "lib_time_ds_commonstreakparams.h"
#include "lib_time_work_deltatime.h"
#include "lib_time_work_instrumentationtime.h"
#include "lib_time_work_streakanalysisconfig.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: StreakSegment                                                    
///                                                                             
///@brief                                                                       
///     Class to hold data from time-streak-identifier-generated data file as   
///     an analyzable object.                                                   
///                                                                             
///     StreakSegment object is initially created from the std::string-         
///     formatted time-streak data file as an analyzable object and             
///     is updated by StreakAnalyzer when the series of analysis operations     
///     were performed.                                                         
///                                                                             
///@note This class may appear to resemble lib::series::work::Streak, however,  
///      the two objects are created with different objectives as               
///      lib::series::work::Streak is tied to the Streak identification         
///      process.                                                               
///                                                                             
///@note Quality-values                                                         
///                                                                             
///     When the object is created, the quality of each time-streak is          
///     initialized accoring to the object properites.                          
///                                                                             
///     The quality-value multipliers, via timestreakanalysis                   
///     configuration file, are applied to the quality-values during the object 
///     initialization as well.                                                 
///                                                                             
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///                                                                             
///@version 2019-03-20  SEV     Added lib_compiler_info and compiler-specific   
///                             language at line 165                            
///                                                                             
///@version 2018-08-28  YBS     Added error message                             
///                                                                             
///@version 2017-01-26  YBS     Added comments regarding the time-streak and    
///                             the analyzed-time-streak string formats         
///                                                                             
///@version 2016-10-27  YBS     Removed non-static data member initializers     
///                                                                             
///@version 2016-09-20  YBS     Minor changes with respect to the internal      
///                             use of StreakAnalysisConfig, use of DI.         
///                             (StreakSegment object may be created via        
///                             StreakAnalyzer)                                 
///                                                                             
///@version 2016-09-14  YBS     Cleanup, removed scoped_ptr                     
///                                                                             
///@version 2016-03-31  YBS     Detailed design review #3 update                
///                              - Cleanups                                     
///                              - Added isQualityLessThan member function      
///                                (kept the inline version due to the          
///                                 current need by the client program)         
///                                                                             
///@version 2016-03-21  YBS     Detailed design review #2 update                
///                              - class name change, file name change          
///                              - cleanup                                      
///                                                                             
///@version 2016-03-16  YBS     Added == operator                               
///                                                                             
///@version 2016-03-02  YBS     Detailed design review #1 update                
///                              - Common streak paramter data structure usage  
///                                updated                                      
///                              - Removed unnecessary compare quality function 
///                              - Comment and minor cleanups                   
///                                                                             
///@version 2016-03-01  YBS     Detailed design review #1 update                
///                              - Added fromString using the common structures 
///                              - Used fromString in constructors to build     
///                                object                                       
///                              - cleanups                                     
///                                                                             
///@version 2016-02-16  YBS     Detailed design review #1 update                
///                              - Use common streak parameter structures       
///                              - Remove indiviual paramater based constructors
///                                                                             
///                                                                             
///@version 2015-11-02  YBS     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

class StreakSegment 
{
    public:
        StreakSegment();

        //----------------------------------------------------------------------
        ///@brief StreakSegment constructor with DI StreakAnalysisConfig        
        //----------------------------------------------------------------------
        explicit StreakSegment(lib::time::work::ConstStreakAnalysisConfigPtr p);

        //----------------------------------------------------------------------
        ///@brief Constructs object from the input std::string str              
        ///                                                                     
        ///@param cp: common data structure with time-streak data,              
        ///           TimeStreakIdentifier output.                              
        ///@param str: std::string that consists of time-streak data            
        ///                                                                     
        /// The expected string format is the string format of the              
        /// TimeStreakIdentifier output. For example,                           
        /// "STREAK,10664,98558816,0,41,000:00:06:29.000000,000:00:07:09.000000"
        ///                                                                     
        ///                                                                     
        ///@note CommonStreakParams will be populated with the parsed str prior 
        ///      to constructing StreakSegment object; the analysis related     
        ///      data members such as corrected-times are populated with the    
        ///      initialized values.                                            
        //----------------------------------------------------------------------
        StreakSegment
        (
            lib::time::ds::CommonStreakParams& cp, const std::string& str
        );

        //----------------------------------------------------------------------
        ///@brief Constructs object from the input std::string str              
        ///                                                                     
        ///@param cp: common data structure with analyzed-time-streak data      
        ///@param str: std::string that consists of time-streak data            
        ///                                                                     
        /// The string format is the StreakAnalyzer output string-format.       
        /// . For example,                                                      
        /// "100,003:01:32:00.100000,CORRECTED,"                                
        /// "STREAK,100,119,3,17,003:01:30:00.100000,003:01:30:25.001000,"      
        /// "0.5,003:01:32:00.100000,003:01:32:25.001000"                       
        ///                                                                     
        //----------------------------------------------------------------------
        StreakSegment
        (
            lib::time::ds::AnalyzedStreakParams& as, const std::string& str
        );

        virtual ~StreakSegment();

        StreakSegment(const StreakSegment& that) = default;
        StreakSegment& operator=(const StreakSegment& that) = default;

        #ifndef IS_VISUAL_STUDIO
        StreakSegment(StreakSegment&& that) = delete;
        StreakSegment& operator=(StreakSegment&& that) = delete;
        #endif // !IS_VISUAL_STUDIO
                                            

        bool fromString
        (
              lib::time::ds::CommonStreakParams& cp
            , const std::string& str
        );

        bool fromString
        (
              lib::time::ds::AnalyzedStreakParams& cp
            , const std::string& str
        );


        static bool isValid
        (
              const lib::time::ds::CommonStreakParams& cp
            , const std::string& str
        );

        static bool isValid
        (
              const lib::time::ds::AnalyzedStreakParams& cp
            , const std::string& str
        );

        //----------------------------------------------------------------------
        ///@brief toString for analyzed-time-streak                             
        //----------------------------------------------------------------------
        std::string toString() const;

        std::string toString(const lib::time::ds::CommonStreakParams& c) const;
        std::string toString(const lib::time::ds::AnalyzedStreakParams& c) const;

        bool operator==(const StreakSegment& that) const;

        //----------------------------------------------------------------------
        ///@brief Initially calculated the quality-value of the time-streak     
        //----------------------------------------------------------------------
        double qualityValue() const;
        void setQualityValue(double value);

        //----------------------------------------------------------------------
        ///@brief Re-calculated the quality-value of time-streak                
        //----------------------------------------------------------------------
        double correctionQualityValue() const;
        void setCorrectionQualityValue(double value);

        lib::time::work::DeltaTime duration() const;

        //----------------------------------------------------------------------
        ///@brief Is the time-streak need correction?                           
        //----------------------------------------------------------------------
        bool markedForCorrection() const;

        //----------------------------------------------------------------------
        ///@brief Set to true/false if the time-streak needs/does not need      
        ///       correction                                                    
        //----------------------------------------------------------------------
        void setCorrectionRequired(bool c);


        uint64_t firstOffset() const;
        uint64_t lastOffset() const;
        size_t  poorlyBehavedCount() const;
        size_t  wellBehavedCount() const;
        size_t  totalCount() const;

        void setType(const std::string& type);
        std::string type() const;

        lib::time::work::InstrumentationTime    firstTime() const;
        lib::time::work::InstrumentationTime    lastTime() const;

        //----------------------------------------------------------------------
        ///@brief Average delta-time within StreakSegment object                
        ///@note It is computed at the time of the object creation              
        //----------------------------------------------------------------------
        lib::time::work::DeltaTime averageDeltaTime() const;

        void setFirstOffset(uint64_t offset);
        void setLastOffset(uint64_t offset);

        void setPoorlyBehavedCount(size_t count);
        void setWellBehavedCount(size_t count);
        void setFirstTime(const lib::time::work::InstrumentationTime& time);
        void setLastTime(const lib::time::work::InstrumentationTime& time);

        //----------------------------------------------------------------------
        ///@note segment-start-offset, corrected-segment-time, corrected--------
        ///      indication are analyzed-time-streak properties                 
        //----------------------------------------------------------------------
        uint64_t segmentStartOffset() const;
        lib::time::work::InstrumentationTime correctedSegmentStartTime() const;

        //----------------------------------------------------------------------
        ///@brief std::string format indicator (CORRECTED or UNCORRECTED)       
        //----------------------------------------------------------------------
        std::string correctedIndication() const;
        void setCorrectedIndication(const std::string& indicator);

        void setSegmentStartOffset(uint64_t offset);

        //----------------------------------------------------------------------
        ///@brief Sets the corrected start-time at the very first offset of     
        ///       the time-streak.                                              
        //----------------------------------------------------------------------
        void setCorrectedSegmentStartTime
        (
            const lib::time::work::InstrumentationTime& time
        );

        //----------------------------------------------------------------------
        ///@brief Keeps track of the corrected times separately from the        
        ///       original time-streak times.                                   
        //----------------------------------------------------------------------
        lib::time::work::InstrumentationTime    correctedFirstTime() const;
        lib::time::work::InstrumentationTime    correctedLastTime() const;

        void setCorrectedFirstTime
        (
            const lib::time::work::InstrumentationTime& time
        );
        void setCorrectedLastTime
        (
            const lib::time::work::InstrumentationTime& time
        );

        std::string errorMessage() const;


        bool isQualityEqual(const StreakSegment& that) const;
        bool isQualityLessThan(const StreakSegment& that) const;

        static const lib::log::ds::messageid_t MSG_INPUT_ERROR;

    protected:

    private:
        void calculateQualityValue();
        std::string outputStringTime(bool first=true) const;

        std::string  m_Type;
        uint64_t     m_FirstOffset;
        uint64_t     m_LastOffset;
        size_t       m_PoorlyBehavedCount;
        size_t       m_WellBehavedCount;
        double       m_QualityValue;
        double       m_CorrectionQualityValue;
        bool         m_CorrectionRequired;
        uint64_t     m_SegmentStartOffset;
        std::string  m_CorrectedIndication;

        lib::time::work::InstrumentationTime  m_FirstTime;
        lib::time::work::InstrumentationTime  m_LastTime;
        lib::time::work::InstrumentationTime  m_CorrectedSegmentStartTime;
        lib::time::work::InstrumentationTime  m_CorrectedFirstTime;
        lib::time::work::InstrumentationTime  m_CorrectedLastTime;

        lib::time::work::ConstStreakAnalysisConfigPtr m_StreakAnalysisConfig;

        std::string m_ErrorMessage;



}; // class StreakSegment //                                                    

typedef boost::shared_ptr<StreakSegment> StreakSegmentPtr;
typedef boost::shared_ptr<const StreakSegment> ConstStreakSegmentPtr;

} // namespace work                                                             
} // namespace time                                                             
} // namespace lib                                                              



inline std::ostream&operator << 
(
      std::ostream& out
    , const lib::time::work::StreakSegment& streak
) {
    return out << streak.toString();
}



#endif // #ifndef LIB_TIME_WORK_STREAKSEGMENT_H_FILE_GUARD                      

