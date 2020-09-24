//------------------------------------------------------------------------------
///@file lib_log_work.cpp                                                       
///@brief   Hold the non-class code for the lib::log facility.                  
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2011-05-18  DHF     File creation basd on old lib::Log class.       
///                                                                             
//------------------------------------------------------------------------------

#include "lib_log_work.h"

#include <assert.h>
#include <string>

namespace lib {
namespace log {
namespace work {

//------------------------------------------------------------------------------
///@brief   Convert the level_t enumerated type to a string.                    
//------------------------------------------------------------------------------
std::string toString(ds::level_t level, bool full)
{
    std::string result;
    switch(level)
    {
        case ds::SILENT:        result = "SILENT "; break;
        case ds::FATAL:         result = "FATAL  "; break;
        case ds::CRITICAL:      result = "CRITICAL "; break;
        case ds::WARNING:       result = "WARNING"; break;
        case ds::INFORMATIONAL: result = "INFO   "; break;
        case ds::VERBOSE:       result = "VERBOSE"; break;
        case ds::DEBUG_LEVEL_1: result = "DEBUG  "; break;
        default:                result = "?      "; break;

    }

    return full ? result : result.substr(0,1);
}

//------------------------------------------------------------------------------
///@brief   Convert the string to a level_t enumerated type.                    
//------------------------------------------------------------------------------
ds::level_t fromString(const std::string& str)
{
    assert(!str.empty());

    switch(str[0]) 
    {
        case'S':  return ds::SILENT;
        case'F':  return ds::FATAL;
        case'C':  return ds::CRITICAL;
        case'W':  return ds::WARNING;
        case'I':  return ds::INFORMATIONAL;
        case'V':  return ds::VERBOSE;
        case'D':  return ds::DEBUG_LEVEL_1;
        default:  return ds::UNDEFINED;
    }  

    return ds::INFORMATIONAL;

}

} // namepsace work 
} // namespace log  
} // namespace lib  

