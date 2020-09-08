//------------------------------------------------------------------------------
///@file lib_log_work.h                                                         
///@brief   Hold the non-class code for the lib::log facility.                  
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-03-03  DHF     Moved operator<< and >> to lib_log_ds.h         
///                                                                             
///@version 2012-09-21  DHF     Moved operator>>,operator<< here from lib_log_ds
///                                                                             
///@version 2011-05-18  DHF     File creation basd on old lib::Log class.       
///                                                                             
//------------------------------------------------------------------------------

#include "lib_log_ds.h"
#include <iostream>
#include <string>

namespace lib {
namespace log {
namespace work {

std::string toString(ds::level_t level, bool full = false);
ds::level_t fromString(const std::string& str);

} // namepsace ds   
} // namespace log  
} // namespace lib  
