//------------------------------------------------------------------------------
///@file lib_time_work.cpp                                                      
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------
#include <fstream>
#include "lib_file_info.h"
#include "lib_time_work_streaksegment.h"

#include "lib_time_work.h"
#include "lib_time_work_range.h"
#include "lib_commandline.h"
#include "lib_time_work_instrumentationtime.h"
#include "lib_log_ds.h"
#include "lib_time_work_range.h"
#include <iostream>

#include "debug.h"

namespace lib {
namespace time {
namespace work {

//------------------------------------------------------------------------------
///@par     Function collection                                                 
///                                                                             
///@brief   This is NOT a class but rather a collection of c-style functions.   
///                                                                             
///@par Thread Safety:  reentrant                                               
///                                                                             
///@version 2017-03-28  KJS     Created new methods (timePrepareCommandLinePost 
///                             and timeParseCommandLinePost) in order to       
///                             complete the refactoring of the time argument   
///                                                                             
///@version 2017-02-08  DHF     Merged Brian's and Magnus' version.             
///                                                                             
///@version 2017-02-02  YBS     Changed to void createTranstionPointCollection  
///                                                                             
///@version 2017-01-26  YBS     Added createTranstionPointCollection            
///                                                                             
///@version 2017-01-06  MNT     - Elevated all log message levels to FATAL      
///                             - Updated log message related documentation     
///                                                                             
///@version 2015-08-06  MNT/DHF Corrected an InstrumentationTime/DeltaTime error
///                                                                             
///@version 2015-07-08  MNT     Replaced DateTime by InstrumentationTime        
///                                                                             
///@version 2015-06-04  MNT     Updated after Code Review 1:                    
///                               - Migrated documentation from .h              
///                               - Made code easier to read                    
///                                                                             
///@version 2014-11-30  MNT     Initial version                                 
///                                                                             
///@version 2014-08-27  MNT     File creation                                   
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///@brief   Initializes a lib::time::work::Range from a lib::CommandLine        
///                                                                             
///         The set method takes a lib::CommandLine, interpretates it,          
///         sets a lib::time::work::Range. The command line flags involved are: 
///                                                                             
///         start-time                                                          
///         stop-time                                                           
///         length-time                                                         
///                                                                             
///         Any combination may be used but if unlogical situations occur,      
///         or invalid command line flag syntaxes are involved, error messages  
///         are generated.                                                      
///                                                                             
///@param   range            The range to be initialized.                       
///@param   commandLine      The commandLine with initialization command        
///@param   errors           The errors detected during execution.              
///@return  boolean true if everything seems to be working fine.                
///@note    A range in CmmandLine is not a Range in this class' context.        
///         It is a period of time but it is not specified which time period,   
///         like delta time                                                     
///@brief   Error Messages:                                                     
///         START_TIME_IS_NOT_BEFORE_STOP_TIME                                  
///             The start time has to be earlier in time than the stop time.    
///         MSG_STOP_TIME_MINUS_START_TIME_IS_NOT_LENGTH                        
///             When all three time parameters are set, stop time - start time  
///             has to be equal to the length                                   
///         MSG_LENGTH_TIME_WITHOUT_START_OR_STOP_TIME                          
///             Length time may not be used without start or stop time          
///         MSG_SYNTAX_ERROR_IN_TIME_FIELD                                      
///             The syntax of a time field is incorrect                         
//------------------------------------------------------------------------------
bool set(lib::time::work::Range& range
       , const lib::CommandLine& commandLine
       , lib::log::Messages& errors
)
{
    //--------------------------------------------------------------------------
    // Declarations and initializations of the parameters needed. Note in       
    // particular the values of the SELECTORs. They are powers of two. The      
    // reason is that then any combination of parameters may be represented     
    // by adding their numbers together.                                        
    //--------------------------------------------------------------------------
    std::string startTime;
    std::string stopTimeToUseString;
    std::string rangeDurationToUseString;

    lib::time::work::InstrumentationTime    startTimeToUse;
    lib::time::work::InstrumentationTime    stopTimeToUse;
    lib::time::work::DeltaTime              rangeDurationToUse;

    bool syntaxIsWrong              = false;
    bool startStopOrderIsOk         = true;
    bool onlyLengthSpecified        = false;

    static const int NO_SELECTOR        (0x00);     //  0                       
    static const int START_SELECTOR     (0x01);     //  2^0                     
    static const int STOP_SELECTOR      (0x02);     //  2^1                     
    static const int LENGTH_SELECTOR    (0x04);     //  2^2                     

    int selector = NO_SELECTOR;

    std::string why;
    std::string why_separator = "";

    #define IS_VALID(x, TYPE)                                           \
        lib::time::work::TYPE::isValid(commandLine[x].string())

    #define CHECK_PARAMETER(x, SELECTOR, TYPE)                          \
        if (commandLine.isSpecified(x)) {                               \
            selector |= SELECTOR;                                       \
            if (!IS_VALID(x, TYPE)) {                                   \
                syntaxIsWrong = true;                                   \
                why += why_separator + "--" x " is invalid";            \
                why_separator = "; ";                                   \
            }                                                           \
        }

            
    CHECK_PARAMETER("start-time",   START_SELECTOR,  InstrumentationTime);
    CHECK_PARAMETER("stop-time",    STOP_SELECTOR,   InstrumentationTime);
    CHECK_PARAMETER("length-time",  LENGTH_SELECTOR, DeltaTime);

    if (syntaxIsWrong) {
        errors.push_back(
            lib::log::MessagePtr(
                new lib::log::Message(
                    lib::log::ds::MAIN
                  , lib::log::ds::FATAL
                  , MSG_SYNTAX_ERROR_IN_TIME_FIELD
                  , why
                )
            )
        );
        return false;
    }

    //--------------------------------------------------------------------------
    // Based upon the selector-value, cleanly find exactly how to:              
    // {                                                                        
    //      Get the values of the used command line parameters and use          
    //      these to initialize the range.                                      
    //                                                                          
    //      Save the return value from the initialization. This may indicate    
    //      incorrect logic order of filter startTime and stopTime. startTime   
    //      has to be before stopTime.                                          
    // }                                                                        
    //--------------------------------------------------------------------------
    switch(selector)
    {
        case NO_SELECTOR:
        //----------------------------------------------------------------------
        // In the NO_SELECTOR case, no relevant line flags were provided.       
        // The range will have it's default parameters. If this is good or      
        // bad is up to each application developer to decide and handle. This   
        // set method will return true, everything is fine.                     
        //----------------------------------------------------------------------
            break;

        case START_SELECTOR:
            startTimeToUse     = commandLine["start-time"].string();
            startStopOrderIsOk = range.setStartTime(startTimeToUse);
            break;

        case STOP_SELECTOR:
            stopTimeToUse      = commandLine["stop-time"].string();
            startStopOrderIsOk = range.setStopTime(stopTimeToUse);
            break;

        case STOP_SELECTOR + START_SELECTOR:
            stopTimeToUse      = commandLine["stop-time"].string();
            startTimeToUse     = commandLine["start-time"].string();
            startStopOrderIsOk = range.setRange(startTimeToUse, stopTimeToUse);
            break; 

        case LENGTH_SELECTOR:
            onlyLengthSpecified = true;
            break;

        case LENGTH_SELECTOR + START_SELECTOR:
            rangeDurationToUse = commandLine["length-time"].string();
            startTimeToUse     = commandLine["start-time"].string();
            stopTimeToUse      = startTimeToUse + rangeDurationToUse;
            startStopOrderIsOk = range.setRange(startTimeToUse, stopTimeToUse);
            break;

        case LENGTH_SELECTOR + STOP_SELECTOR:
            rangeDurationToUse = commandLine["length-time"].string();
            stopTimeToUse      = commandLine["stop-time"].string();
            startTimeToUse     = stopTimeToUse - rangeDurationToUse;
            startStopOrderIsOk = range.setRange(startTimeToUse, stopTimeToUse);
            break;

        case LENGTH_SELECTOR + STOP_SELECTOR + START_SELECTOR:
            rangeDurationToUse = commandLine["length-time"].string();
            stopTimeToUse      = commandLine["stop-time"].string();
            startTimeToUse     = commandLine["start-time"].string();

            //------------------------------------------------------------------
            // This is a special case where contradicting parameter values may  
            // appear.                                                          
            //                                                                  
            // if (no conflicting parameter values) { Set range as usual. }     
            // else { Genereate and set an error message and return false. }    
            //------------------------------------------------------------------
            if (stopTimeToUse - startTimeToUse == rangeDurationToUse) {
                startStopOrderIsOk
                  = range.setRange(startTimeToUse, stopTimeToUse);
            } else {
                errors.push_back(
                    lib::log::MessagePtr(
                        new lib::log::Message(
                            lib::log::ds::MAIN
                          , lib::log::ds::FATAL
                          , MSG_STOP_TIME_MINUS_START_TIME_IS_NOT_LENGTH
                          , "--start-time plus --length-time has to be --stop-time."
                        )
                    )
                );
                return false;
            }
            break;

        default:
            //------------------------------------------------------------------
            //  An error, this should not happen.                               
            //------------------------------------------------------------------
            break;
    }

    //--------------------------------------------------------------------------
    //  Exit with a false return value if only a range was specified on the     
    //  command line. No need for a range if we don't know it's start-time      
    //  or stop-time.                                                           
    //--------------------------------------------------------------------------
    if (onlyLengthSpecified) {
        errors.push_back(
            lib::log::MessagePtr(
                new lib::log::Message(
                    lib::log::ds::MAIN
                  , lib::log::ds::FATAL
                  , MSG_LENGTH_TIME_WITHOUT_START_OR_STOP_TIME
                  , "--length-time specified without --start-time or --stop-time."
                )
            )
        );
        return false;
    }

    //--------------------------------------------------------------------------
    //  Check the return value from the range-initialization we performed       
    //  above. If no problem was reported: execute "return true". On the other  
    //  hand: Generate and set an error message as well as execute "return      
    //  false" if a problem was reported.                                       
    //--------------------------------------------------------------------------
    if (!startStopOrderIsOk) {
        errors.push_back(
            lib::log::MessagePtr(
                new lib::log::Message(
                    lib::log::ds::MAIN
                  , lib::log::ds::FATAL
                  , MSG_START_TIME_IS_NOT_BEFORE_STOP_TIME
                  , "--start-time has to be before --stop-time."
                )
            )
        );
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
///@brief   
///     Parses the analyzed-time-streak data, extracts the elements, creates 
///     objects and populate the transition-point-collection.
///     The transition-point-collection normally is used to aid the
///     smoothing of time-values.
//------------------------------------------------------------------------------
void createTranstionPointCollection
(
      const std::string& filePath // path to the StreakAnalyzer output 
    , std::vector
      <
            std::pair
            <
              uint64_t
            , lib::time::work::InstrumentationTime
            > 
      >& transitionPoints
)
{
    if(!lib::file::Info::exists(filePath)){
        throw lib::log::work::Exception<std::invalid_argument>(
            lib::log::ds::LIB_TIME_WORK_SMOOTHINGSEGMENTCOLLECTION 
          , lib::log::ds::FATAL
          , lib::log::ds::FILE_OPEN_READ_FAILURE
          , "input analyzed-timestreak data file not found"
        );
    }


    std::ifstream inFile(filePath);

    lib::time::work::StreakSegment           s;
    lib::time::ds::AnalyzedStreakParams     as;

    //--------------------------------------------------------------------------
    // The valid analyzed streak data must exist. Parse the Streak data via
    // lib::time::work::StreekSegment.
    //--------------------------------------------------------------------------
    std::string line("");
    while (std::getline(inFile, line) && line!="") {
        if (!s.fromString(as, line)) {
            throw lib::log::work::Exception<std::invalid_argument>(
                  lib::log::ds::LIB_IRIG106_CH10_WORK_TIMESMOOTHER
                , lib::log::ds::FATAL
                , lib::log::ds::FILE_WRONG_TYPE
                , "invalid input string"
            );
        }

        //----------------------------------------------------------------------
        // Populate the transition-point collection with the extracted start-
        // file-offset and the corresponding start-time.
        //----------------------------------------------------------------------
        transitionPoints.push_back
        (
            std::make_pair
            (
                s.segmentStartOffset(), s.correctedSegmentStartTime()
            )
        );
    }

   
}

//------------------------------------------------------------------------------
///@brief Call this to allow the user to enter the parameters for the 
/// --start-time, --stop-time, and --length-time commands
//------------------------------------------------------------------------------
void timePrepareCommandLinePost(
    lib::CommandLine & command_line
  , bool required
)
{
    if(required)
    {
        command_line.setRequired(
              "start-time"
            , lib::CommandLine::DATE_TIME
            , "E.g.: 112:00:34:00.999999"
        );

        command_line.setRequired(
              "stop-time"
            , lib::CommandLine::DATE_TIME
            , "E.g.: 112:20:15:03.88"
        );

        command_line.setRequired(
              "length-time"
            , lib::CommandLine::DATE_TIME
            , "E.g.: 113:11:33:45:123456"
        );
    } else {

        command_line.setOptional(
              "start-time"
            , lib::CommandLine::DATE_TIME
            , "E.g.: 112:00:34:00.999999"
        );

        command_line.setOptional(
              "stop-time"
            , lib::CommandLine::DATE_TIME
            , "E.g.: 112:20:15:03.88"
        );

        command_line.setOptional(
              "length-time"
            , lib::CommandLine::DATE_TIME
            , "E.g.: 113:11:33:45:123456"
        );
    }
} // end timePrepareCommandLinePost

//------------------------------------------------------------------------------
/// @brief  Check that the start-time AND stop-time AND length-time parameters  
///         are properly formed and the busnames are correct.                   
///                                                                             
///         This should be called from a Main::parseCommandLinePost method.     
///                                                                             
///@param   time            A string holding either start-time, stop-time,      
///                         length-time; which command line parameter to check. 
///         messages        The error messages generated (if any)               
///         command_line    The command line to check                           
//------------------------------------------------------------------------------
bool timeParseCommandLinePost(
    lib::log::Messages& messages
  , const lib::CommandLine& command_line
)
{
    lib::time::work::Range range;
    return set(range, command_line, messages);

} //end timeParseCommandLinePost   

} // namespace work {
} // namespace time {
} // namespace lib {

