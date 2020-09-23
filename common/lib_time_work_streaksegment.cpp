//------------------------------------------------------------------------------
///@file lib_time_work_streaksegment.cpp                                    
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
///                                                                             
///@brief   
///                                                                             
///     Class to hold data from time-streak-identifier-generated data file as
///     an analyzable object. 
///                                                                             
///@version 2017-08-28  YBS     Added error message
///
///@version 2017-01-26  YBS     Time data member initialization update
///                             Formatting cleanup
///
///@version 2016-10-27  YBS     Removed non-static data member initializers
///
///@version 2016-03-01  YBS     File creation                                   
//------------------------------------------------------------------------------
#include <sstream>
#include "lib_time_work_streaksegment.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///@brief StreakSegment constructor
//------------------------------------------------------------------------------
StreakSegment::StreakSegment()
    :
          m_Type("STREAK")
        , m_FirstOffset(0)
        , m_LastOffset(0)
        , m_PoorlyBehavedCount(0)
        , m_WellBehavedCount(0)
        , m_QualityValue(0)
        , m_CorrectionQualityValue(0)
        , m_CorrectionRequired(false)
        , m_SegmentStartOffset(0)
        , m_CorrectedIndication("UNCORRECTED")
        , m_FirstTime(0.0)
        , m_LastTime(0.0)
        , m_CorrectedSegmentStartTime(0.0)
        , m_CorrectedFirstTime(0.0)
        , m_CorrectedLastTime(0.0)
        , m_ErrorMessage("")
{
    m_StreakAnalysisConfig =
            boost::make_shared<lib::time::work::StreakAnalysisConfig>();
}


//------------------------------------------------------------------------------
///@brief StreakSegment constructor with DI StreakAnalysisConfig
//------------------------------------------------------------------------------
StreakSegment::StreakSegment(lib::time::work::ConstStreakAnalysisConfigPtr p)
    : StreakSegment()
{
    m_StreakAnalysisConfig = p;
}


//------------------------------------------------------------------------------
///@brief Constructs object from the input std::string str
//------------------------------------------------------------------------------
StreakSegment::StreakSegment
(
    lib::time::ds::CommonStreakParams& cp, const std::string& str
) : StreakSegment()
{
    fromString(cp, str);

    //-------------------------------------------------------------------------
    // Initialize the rest of the properites for now;
    // Normally, these values are updated externally.
    //-------------------------------------------------------------------------
    m_SegmentStartOffset = m_FirstOffset;
    m_CorrectedIndication = std::string("UNCORRECTED");
    m_CorrectedSegmentStartTime = m_FirstTime;
    m_CorrectedFirstTime = m_FirstTime;
    m_CorrectedLastTime = m_LastTime;

}


//------------------------------------------------------------------------------
///@brief Constructs object from the input std::string str
//------------------------------------------------------------------------------
StreakSegment::StreakSegment
(
    lib::time::ds::AnalyzedStreakParams& as, const std::string& str
) : StreakSegment()
{
    fromString(as, str);
}




//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
StreakSegment::~StreakSegment()
{

} // StreakSegment::~StreakSegment


//------------------------------------------------------------------------------
///@brief Streak quality-value
//------------------------------------------------------------------------------
double StreakSegment::qualityValue() const
{
    return m_QualityValue;
} // double StreakSegment::qualityValue() const //




//------------------------------------------------------------------------------
///@brief Calculate quality-value with well-behaved-count and duration
///
///@note  The streak quality can also be updated externally with respect
///       to other streaks in the collection.
//------------------------------------------------------------------------------
void StreakSegment::calculateQualityValue()
{
    if (m_WellBehavedCount == 0) {
        m_QualityValue = 0.0;
        m_CorrectionRequired = true;
        return;
    }

    m_QualityValue = static_cast<double>(m_WellBehavedCount) 
        / static_cast<double>(m_WellBehavedCount + m_PoorlyBehavedCount);

    m_QualityValue*= m_StreakAnalysisConfig->
                                    qualityMultiplierByDuration(duration());

} // void TimeStreakData::calculateQualityValue() //





//------------------------------------------------------------------------------
///@brief Set the quality-value                                                                      
//------------------------------------------------------------------------------
void StreakSegment::setQualityValue(double value)
{
    m_QualityValue = value;
}



//------------------------------------------------------------------------------
///@brief Set the corrected quality-value 
///
///@note  Track the externally re-computed quailty-value seperately from the 
///       original/self-calculated quality-value.
//------------------------------------------------------------------------------
void StreakSegment::setCorrectionQualityValue(double value)
{
    m_CorrectionQualityValue = value;
}




//------------------------------------------------------------------------------
///@brief Corrected quality-value
//------------------------------------------------------------------------------
double StreakSegment::correctionQualityValue() const
{
    return m_CorrectionQualityValue;
}


//------------------------------------------------------------------------------
///@brief Duration of the streak
//------------------------------------------------------------------------------
lib::time::work::DeltaTime StreakSegment::duration() const
{
    return lib::time::work::DeltaTime(m_LastTime - m_FirstTime);
} // lib::time::work::DeltaTime StreakSegment::duration() const //


//------------------------------------------------------------------------------
///@brief Marked/unmarked for correction
//------------------------------------------------------------------------------
bool StreakSegment::markedForCorrection()const
{
    return m_CorrectionRequired;
} // bool StreakSegment::markedForCorrection()const //


//------------------------------------------------------------------------------
///@brief Mark/unmark the streak for correction
//------------------------------------------------------------------------------
void StreakSegment::setCorrectionRequired(bool c)
{
    m_CorrectionRequired = c;
    if (c) m_QualityValue = 0.0;

} // void StreakSegment::setCorrectionRequired(bool c) //


//------------------------------------------------------------------------------
///@brief First file-offset
//------------------------------------------------------------------------------
uint64_t StreakSegment::firstOffset() const
{
    return m_FirstOffset;
} // int64_t StreakSegment::firstOffset() const //


//------------------------------------------------------------------------------
///@brief Last file-offset
//------------------------------------------------------------------------------
uint64_t StreakSegment::lastOffset() const
{
    return m_LastOffset;
} // int64_t StreakSegment::lastOffset() const //


//------------------------------------------------------------------------------
///@brief Set first file-offset
//------------------------------------------------------------------------------
void StreakSegment::setFirstOffset(uint64_t offset)
{
    m_FirstOffset = offset;
}

//------------------------------------------------------------------------------
///@brief Set last file-offset
//------------------------------------------------------------------------------
void StreakSegment::setLastOffset(uint64_t offset)
{
    m_LastOffset = offset;
}



//------------------------------------------------------------------------------
///@brief Poorly-behaved count
//------------------------------------------------------------------------------
size_t StreakSegment::poorlyBehavedCount() const
{
    return m_PoorlyBehavedCount;
} // size_t StreakSegment::poorlyBehavedCount() const //


//------------------------------------------------------------------------------
///@brief Well-behaved count
//------------------------------------------------------------------------------
size_t StreakSegment::wellBehavedCount() const
{
    return m_WellBehavedCount;
} // size_t StreakSegment::wellBehavedCount() const //


//------------------------------------------------------------------------------
///@brief Set poorly-behaved count
//------------------------------------------------------------------------------
void StreakSegment::setPoorlyBehavedCount(size_t count)
{
    m_PoorlyBehavedCount = count;
}


//------------------------------------------------------------------------------
///@brief Set well-behaved count
//------------------------------------------------------------------------------
void StreakSegment::setWellBehavedCount(size_t count)
{
    m_WellBehavedCount = count;
}


//------------------------------------------------------------------------------
///@brief Total count (well-behaved + poorly-behaved counts)
//------------------------------------------------------------------------------
size_t StreakSegment::totalCount() const
{
    return m_WellBehavedCount + m_PoorlyBehavedCount;
} // size_t StreakSegment::totalCount() const //


//------------------------------------------------------------------------------
///@brief First-time
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime  StreakSegment::firstTime() const
{
    return m_FirstTime;
} // lib::time::work::InstrumentationTime  StreakSegment::firstTime() const //


//------------------------------------------------------------------------------
///@brief Last-time                                                                      
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime  StreakSegment::lastTime() const
{
    return m_LastTime;
} // lib::time::work::InstrumentationTime  StreakSegment::lastTime() const //


//------------------------------------------------------------------------------
///@brief Set first-time                                                                      
//------------------------------------------------------------------------------
void StreakSegment::setFirstTime(const lib::time::work::InstrumentationTime& time)
{
    m_FirstTime = time;

}


//------------------------------------------------------------------------------
///@brief Set last-time                                                                      
//------------------------------------------------------------------------------
void StreakSegment::setLastTime(const lib::time::work::InstrumentationTime& time)
{
    m_LastTime = time;
}



//------------------------------------------------------------------------------
///@brief Average delta-time within StreakSegment object
///
///@note It is computed at the time of the object creation
//------------------------------------------------------------------------------
lib::time::work::DeltaTime StreakSegment::averageDeltaTime() const
{
    return lib::time::work::DeltaTime
    (
        (m_LastTime.inSeconds() - m_FirstTime.inSeconds())
        / (m_PoorlyBehavedCount + m_WellBehavedCount - 1)
    );

} // lib::time::work::DeltaTime TimeStreakData::averageSampleTime() const //




//------------------------------------------------------------------------------
///@brief The file-offset of the start of the analyzed-streak 
//------------------------------------------------------------------------------
uint64_t StreakSegment::segmentStartOffset() const
{
    return m_SegmentStartOffset;
}


//------------------------------------------------------------------------------
///@brief set segment start-offset
//------------------------------------------------------------------------------
void StreakSegment::setSegmentStartOffset(uint64_t offset)
{
    m_SegmentStartOffset = offset;
}



//------------------------------------------------------------------------------
///@brief Corrected start-time of the analyzed-streak
//------------------------------------------------------------------------------
lib::time::work::InstrumentationTime StreakSegment
                                ::correctedSegmentStartTime() const
{
    return m_CorrectedSegmentStartTime;
}




//------------------------------------------------------------------------------
///@brief Set corrected start-time of the analyzed-streak
//------------------------------------------------------------------------------
void StreakSegment::setCorrectedSegmentStartTime
(
    const lib::time::work::InstrumentationTime& time
)
{
    m_CorrectedSegmentStartTime = time;
}


//------------------------------------------------------------------------------
///@bried Corrected Indication ("CORRECTED/UNCORRECTED")
///
///@note Analyzed streak-segment header element
//------------------------------------------------------------------------------
std::string StreakSegment::correctedIndication() const
{
    return m_CorrectedIndication;
}

        
void StreakSegment::setCorrectedIndication(const std::string& s)
{
    m_CorrectedIndication = s;
}




lib::time::work::InstrumentationTime StreakSegment::correctedFirstTime() const
{
    return m_CorrectedFirstTime;
}


lib::time::work::InstrumentationTime StreakSegment::correctedLastTime() const
{
    return m_CorrectedLastTime;
}


void StreakSegment::setCorrectedFirstTime
(
    const lib::time::work::InstrumentationTime& time
)
{
    m_CorrectedFirstTime = time;
}


void StreakSegment::setCorrectedLastTime
(
    const lib::time::work::InstrumentationTime& time
)
{
    m_CorrectedLastTime = time;
}




//------------------------------------------------------------------------------
///@brief toString from streak
//------------------------------------------------------------------------------
std::string StreakSegment::toString
(
    const lib::time::ds::CommonStreakParams& c
) const
{
  
    std::ostringstream out;


    if (c.m_Type == "NOISE") {
        out << c.m_Type
            << "," << c.m_FirstOffset 
            << "," << c.m_LastOffset 
            << "," << c.m_PoorlyBehavedCount
            ;
    } else {
        out << c.m_Type
            << "," << c.m_FirstOffset 
            << "," << c.m_LastOffset 
            << "," << c.m_PoorlyBehavedCount
            << "," << c.m_WellBehavedCount
            << "," << c.m_FirstValue
            << "," << c.m_LastValue
            ;
    }

    return out.str();
}



//------------------------------------------------------------------------------
///@brief toString from analyzed-streak
//------------------------------------------------------------------------------
std::string StreakSegment::toString
(
    const lib::time::ds::AnalyzedStreakParams& p
) const
{


    std::string fTime = p.m_Common.m_FirstValue.toString();
    std::string lTime = p.m_Common.m_LastValue.toString();

    std::ostringstream ow;

    if (p.m_Common.m_Type == "NOISE") {
        fTime = "";
        lTime = "";
        ow << "";
    } else {
        ow << p.m_Common.m_WellBehavedCount;
    }


    std::ostringstream out;
    out << p.m_Segment.m_StartOffset
        << "," << p.m_Segment.m_CorrectedStartTime
        << "," << p.m_CorrectedIndication
        << "," << p.m_Common.m_Type
        << "," << p.m_Common.m_FirstOffset
        << "," << p.m_Common.m_LastOffset
        << "," << p.m_Common.m_PoorlyBehavedCount
        << "," << ow.str() // p.m_Common.m_WellBehavedCount
        << "," << fTime
        << "," << lTime 
        << "," << p.m_QualityValue
        << "," << p.m_CorrectedFirstTime
        << "," << p.m_CorrectedLastTime
        ;

    return out.str();

}




//-----------------------------------------------------------------------------
///@note firstTime and lastTime are included in STREAK only.
//-----------------------------------------------------------------------------
std::string StreakSegment::toString() const
{
    std::ostringstream out;

    out << segmentStartOffset()
        << "," << correctedSegmentStartTime().toString()
        << "," << correctedIndication()
        << "," << type()
        << "," << firstOffset() 
        << "," << lastOffset() 
        << "," << poorlyBehavedCount()
        << "," << wellBehavedCount()
        << "," << outputStringTime()
        << "," << outputStringTime(false)
        << "," << qualityValue()
        << "," << correctedFirstTime().toString()
        << "," << correctedLastTime().toString()
        ;

    return out.str();
}





std::string StreakSegment::outputStringTime(bool first) const
{
    if (type() == "NOISE") {
        return "";
    } else {
        std::string ret = first ? firstTime().toString() 
                                    : lastTime().toString();
        return ret;
    }
}




//-----------------------------------------------------------------------------
///@brief type (STREAK/NOISE)
//-----------------------------------------------------------------------------
std::string StreakSegment::type() const
{
    return m_Type;
}


void StreakSegment::setType(const std::string& type)
{
    m_Type = type;
}


//-----------------------------------------------------------------------------
///@brief Parsing for streak
//-----------------------------------------------------------------------------
bool StreakSegment::fromString
(
      lib::time::ds::CommonStreakParams& cp
    , const std::string& str
) 
{

    std::istringstream is(str);
    std::string token("");
    m_ErrorMessage = "";

    bool invalid_time_mark = false;

    std::vector<std::string> list;
    while (std::getline(is, token, ',')) {
        list.push_back(token);
    }
    if (list.empty()) return false;


    size_t inputToken = 0;
    std::string type(list[inputToken++]);

    //-------------------------------------------------------------------------
    // type check : must be streak or noise
    //-------------------------------------------------------------------------
    if (type != "STREAK" && type != "NOISE") {
        m_ErrorMessage = "Incorrect Type";
        return false;
    }
    if (type == "STREAK" && list.size() != 7) {
        m_ErrorMessage = "STREAK token error";
        return false;
    } else if (type == "NOISE" && list.size() != 4) {
        m_ErrorMessage = "NOISE token error";
        return false;
    }

    cp.m_Type = type;
    setType(type);
    //-------------------------------------------------------------------------
    // offset check
    //-------------------------------------------------------------------------
    std::stringstream   fOffsetStream(list[inputToken++]);
    std::stringstream   lOffsetStream(list[inputToken++]);


    fOffsetStream >> cp.m_FirstOffset;
    m_FirstOffset = cp.m_FirstOffset;
    lOffsetStream >> cp.m_LastOffset;
    m_LastOffset = cp.m_LastOffset;

    //-------------------------------------------------------------------------
    // Poorly-behaved count applies to both STREAK and NOISE
    //-------------------------------------------------------------------------
    std::stringstream   pbCountStream(list[inputToken++]);
    pbCountStream >> cp.m_PoorlyBehavedCount;
    m_PoorlyBehavedCount = cp.m_PoorlyBehavedCount;

    if (type == "NOISE") {
        //---------------------------------------------------------------------
        //if NOISE, add the fillers
        //---------------------------------------------------------------------
        cp.m_WellBehavedCount = m_WellBehavedCount = 0; //filler
        lib::time::work::InstrumentationTime    time(0);
        cp.m_FirstValue = m_FirstTime = time;     //filler
        cp.m_LastValue = m_LastTime = time;      //filler
        if (m_PoorlyBehavedCount< 1) {
            m_ErrorMessage = "NOISE : poorly-behaved-count is 0";
            return false;
        }
    } else {
        //---------------------------------------------------------------------
        // STREAK follows
        //---------------------------------------------------------------------
        std::stringstream   wbCountStream(list[inputToken++]);
        wbCountStream >> cp.m_WellBehavedCount;
        m_WellBehavedCount = cp.m_WellBehavedCount;

        if (m_WellBehavedCount < 1) { 
            m_ErrorMessage = "STREAK : well-behaved-count is 0";
            return 0;
        }

        invalid_time_mark = !cp.m_FirstValue.isValid(list[inputToken]);
        lib::time::work::InstrumentationTime    ftime(list[inputToken++]);
        cp.m_FirstValue = ftime;
        m_FirstTime = cp.m_FirstValue;

        invalid_time_mark = !cp.m_LastValue.isValid(list[inputToken]);
        lib::time::work::InstrumentationTime    ltime(list[inputToken++]);
        cp.m_LastValue = ltime;
        m_LastTime = cp.m_LastValue;

    }


    //-------------------------------------------------------------------------
    // Initialize the rest of the properites for now
    //-------------------------------------------------------------------------
    m_SegmentStartOffset = m_FirstOffset;
    m_CorrectedIndication = std::string("UNCORRECTED");
    m_CorrectedSegmentStartTime = m_FirstTime;
    m_CorrectedFirstTime = m_FirstTime;
    m_CorrectedLastTime = m_LastTime;

    //-------------------------------------------------------------------------
    // Calculate the initial quality-value
    //-------------------------------------------------------------------------
    calculateQualityValue();

    if (invalid_time_mark) {
        setCorrectionQualityValue(0);
    } else {
        setCorrectionQualityValue(qualityValue());
    }
    return true;
}




bool StreakSegment::isValid
(
      const lib::time::ds::CommonStreakParams& cp
    , const std::string& str
)
{
    lib::time::ds::CommonStreakParams data = cp; 
    lib::time::work::StreakSegment  s;
    return s.fromString(data, str);
}



bool StreakSegment::isValid
(
      const lib::time::ds::AnalyzedStreakParams& ap
    , const std::string& str
)
{
    lib::time::ds::AnalyzedStreakParams data = ap;
    lib::time::work::StreakSegment  s;
    return s.fromString(data, str);
}






//-----------------------------------------------------------------------------
///@brief Parsing for analyzed-streak
//-----------------------------------------------------------------------------
bool StreakSegment::fromString
(
    lib::time::ds::AnalyzedStreakParams& as, const std::string& str
) 
{

    std::istringstream is(str);
    std::string token("");

    std::vector<std::string> list;
    while (std::getline(is, token, ',')) {
        list.push_back(token);
    }
    if (list.size() < 13 || list.size() > 13) return false;

    size_t inputToken = 0;
    //-------------------------------------------------------------------------
    // Parsing the segment information
    //-------------------------------------------------------------------------
    std::stringstream   startSegmentOffsetStream(list[inputToken++]);
    startSegmentOffsetStream >> as.m_Segment.m_StartOffset;
    m_SegmentStartOffset =  as.m_Segment.m_StartOffset;
    
    std::stringstream   startSegmentTimeStream(list[inputToken]);
    if (!as.m_Segment.m_CorrectedStartTime.isValid(list[inputToken]))return false;
    lib::time::work::InstrumentationTime    segStartTime(list[inputToken++]);
    as.m_Segment.m_CorrectedStartTime = segStartTime;
    m_CorrectedSegmentStartTime = as.m_Segment.m_CorrectedStartTime;

    std::string correctedIndication(list[inputToken++]);
    as.m_CorrectedIndication = correctedIndication;
    m_CorrectedIndication = as.m_CorrectedIndication;

    std::string     tempStr("");
    //--------------------------------------------------------------------------
    // Parses for the common streak params
    //--------------------------------------------------------------------------
    int adjust = list[3] == "NOISE" ? 6 : 3;
    for (auto it = list.cbegin() + 3; it < list.cend() - adjust; it++) {
        tempStr += (*it);
        if (it != list.end() - 4) tempStr += ",";
        inputToken++;
    }
    if (adjust == 6) inputToken+=3;

    //--------------------------------------------------------------------------
    // Not to overwrite the analyzed-streak properties, do with the temp object
    //--------------------------------------------------------------------------
    lib::time::ds::CommonStreakParams cpTemp = as.m_Common;
    lib::time::work::StreakSegment    sTemp;
    if(!sTemp.fromString(cpTemp, tempStr)) return false;

    as.m_Common.m_Type = sTemp.type();
    m_Type = as.m_Common.m_Type;

    as.m_Common.m_FirstOffset = sTemp.firstOffset();
    m_FirstOffset = as.m_Common.m_FirstOffset;

    as.m_Common.m_LastOffset = sTemp.lastOffset();
    m_LastOffset = as.m_Common.m_LastOffset;

    as.m_Common.m_PoorlyBehavedCount = sTemp.poorlyBehavedCount();
    m_PoorlyBehavedCount = as.m_Common.m_PoorlyBehavedCount;

    as.m_Common.m_WellBehavedCount = sTemp.wellBehavedCount();
    m_WellBehavedCount = as.m_Common.m_WellBehavedCount;

    as.m_Common.m_FirstValue = sTemp.firstTime();
    m_FirstTime = as.m_Common.m_FirstValue;

    as.m_Common.m_LastValue = sTemp.lastTime();
    m_LastTime = as.m_Common.m_LastValue;



    //--------------------------------------------------------------------------
    // Last 3 fields in the analyzed time-streak
    //--------------------------------------------------------------------------
    std::stringstream   qualityValueStream(list[inputToken++]);
    qualityValueStream >> as.m_QualityValue;
    m_QualityValue = as.m_QualityValue;

    if (!as.m_CorrectedFirstTime.isValid(list[inputToken])) return false;
    lib::time::work::InstrumentationTime correctedFirstTime(list[inputToken++]);
    as.m_CorrectedFirstTime = correctedFirstTime;
    m_CorrectedFirstTime = as.m_CorrectedFirstTime;

    if (!as.m_CorrectedLastTime.isValid(list[inputToken])) return false;
    lib::time::work::InstrumentationTime    correctedLastTime(list[inputToken]);
    as.m_CorrectedLastTime = correctedLastTime;
    m_CorrectedLastTime = as.m_CorrectedLastTime;

    return true;
}

bool StreakSegment::isQualityEqual(const StreakSegment& that) const 
{
    return qualityValue() == that.qualityValue();
}

bool StreakSegment::isQualityLessThan(const StreakSegment& that) const 
{
    return qualityValue() < that.qualityValue();
}

bool StreakSegment::operator==(const StreakSegment& that) const
{
    return 
    (
           m_Type == that.m_Type 
        && m_FirstOffset == that.m_FirstOffset
        && m_LastOffset == that.m_LastOffset
        && m_PoorlyBehavedCount == that.m_PoorlyBehavedCount
        && m_WellBehavedCount == that.m_WellBehavedCount
        && m_FirstTime == that.m_FirstTime
        && m_LastTime == that.m_LastTime
        && m_QualityValue == that.m_QualityValue
        && m_SegmentStartOffset == that.m_SegmentStartOffset
        && m_CorrectedSegmentStartTime 
                               == that.m_CorrectedSegmentStartTime
        && m_CorrectedIndication == that.m_CorrectedIndication
        && m_CorrectedFirstTime == that.m_CorrectedFirstTime
        && m_CorrectedLastTime == that.m_CorrectedLastTime
  );

}

std::string StreakSegment::errorMessage() const 
{
    return m_ErrorMessage;
}


} // namespace work //
} // namespace time //
} // namespace lib //


