//------------------------------------------------------------------------------
///@file lib_time_work.h                                                        
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------
#ifndef LIB_TIME_WORK_H_FILE_GUARD
#define LIB_TIME_WORK_H_FILE_GUARD

#include "lib_time_work_range.h"
#include "lib_commandline.h"
#include "lib_log_messagefactory.h"
#include "lib_log_work_messagegenerator.h"

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
///@version 2020-02-03  KCG     Renamed ErrorGenerator to MessageGenerator      
///                                                                             
///@version 2020-01-07  KCG     Renamed MessageQueue to ErrorGenerator          
///                                                                             
///@version 2017-03-28  KJS     Created new methods (timePrepareCommandLinePost 
///                             and timeParseCommandLinePost) in order to       
///                             complete the refactoring of the time argument   
///                                                                             
///@version 2017-01-06  MNT     - Updated log message related documentation     
///                                                                             
///@version 2014-09-11  MNT     Updated after Detailed Design Review 3          
///                             - Reversed change log to reversed chronological.
///                             - Changed Thread Safety classification.         
///                             - Added an error code                           
///                             - Minor rephrasings.                            
///@version 2014-09-10  MNT     Updated after Detailed Design Review 2          
///                             - Removed "helpers" from the name of the file.  
///                             - Shortened the file header.                    
///                             - Gave the function set an errors parameter     
///                               via which errors will be relayed back to      
///                               the caller.                                   
///                             - Updated the function set's documentation.     
///@version 2014-08-27  MNT     File creation                                   
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///@Brief   Error code: The start time is later or equal to the stop time.      
//------------------------------------------------------------------------------
static const lib::log::ds::messageid_t
    MSG_START_TIME_IS_NOT_BEFORE_STOP_TIME(0);

//------------------------------------------------------------------------------
///@Brief   Error code: All three time parameters: start, stop, and length      
///                     have been specified, but stop - start != length.        
//------------------------------------------------------------------------------
static const lib::log::ds::messageid_t
    MSG_STOP_TIME_MINUS_START_TIME_IS_NOT_LENGTH(1);

//------------------------------------------------------------------------------
///@Brief   Error code: Syntax error in time field.                             
//------------------------------------------------------------------------------
static const lib::log::ds::messageid_t
    MSG_SYNTAX_ERROR_IN_TIME_FIELD(2);

//------------------------------------------------------------------------------
///@Brief   Error code: Length time specified without start or stop time.       
//------------------------------------------------------------------------------
static const lib::log::ds::messageid_t
    MSG_LENGTH_TIME_WITHOUT_START_OR_STOP_TIME(3);

//------------------------------------------------------------------------------
///@brief   Initializes a lib::time::work::Range from a lib::CommandLine        
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
bool set(
    lib::time::work::Range& range
  , const lib::CommandLine& commandLine
  , lib::log::Messages& errors
);

//------------------------------------------------------------------------------
///@brief   
///     Parses the analyzed-time-streak data, extracts the elements, creates 
///     objects and populate the transition-point-collection.
///     The transition-point-collection normally is used to aid the
///     smoothing of time-values.
//------------------------------------------------------------------------------
void createTranstionPointCollection (
      const std::string& path_to_file // path to the StreakAnalyzer output 
    , std::vector
      <
            std::pair
            <
              uint64_t
            , lib::time::work::InstrumentationTime
            > 
      >& transitionPoints
);

void timePrepareCommandLinePost(
    lib::CommandLine& command_line
  , bool required = false
);

bool timeParseCommandLinePost(
    lib::log::Messages& messages
  , const lib::CommandLine& command_line
);

} // namespace work {
} // namespace time {
} // namespace lib {

#endif // #ifndef LIB_TIME_WORK_H_FILE_GUARD
