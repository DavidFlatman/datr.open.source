//------------------------------------------------------------------------------
///@file lib_log_ds.cpp                                                         
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Removed dependency on Boost.                    
///                                                                             
///@version 2014-03-03  DHF     Moved operator<< and >> inside lib::log::ds     
///                             namespace (clang had issues if it was outside). 
//------------------------------------------------------------------------------
#include "lib_log_ds.h"

#include <string>

namespace lib {
namespace log {
namespace ds {


//------------------------------------------------------------------------------
///@brief Output text values of the enumeration to the ostream                  
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, lib::log::ds::level_t e)
{
    switch (e)
    {
        case lib::log::ds::SILENT:        return out << "silent";
        case lib::log::ds::FATAL:         return out << "fatal"; 
        case lib::log::ds::CRITICAL:      return out << "critical";
        case lib::log::ds::WARNING:       return out << "warning";
        case lib::log::ds::INFORMATIONAL: return out << "informational";
        case lib::log::ds::VERBOSE:       return out << "verbose";
        case lib::log::ds::DEBUG_LEVEL_1: return out << "debug-level-1";
        default:                          return out << "undefined";
    }
}

//------------------------------------------------------------------------------
///@brief Translate text value to an enumeration value                          
///                                                                             
/// This operator is required to allow lib_configurationfile.h convert strings 
/// input by the user to enumeration values that lib_log_work_message can use.  Note
/// that the conversion is case insensitive.                                    
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& in, lib::log::ds::level_t& e)
{
    std::string x;
    in >> x;

    for (auto& c : x)
    {
        if (c >= 'A' && c <= 'Z') c = c + 32;
        if (c == '-') c = '_';
    }

    if      (x == "silent")         {e = lib::log::ds::SILENT;}
    else if (x == "fatal")          {e = lib::log::ds::FATAL;}
    else if (x == "critical")       {e = lib::log::ds::CRITICAL;}
    else if (x == "warning")        {e = lib::log::ds::WARNING;}
    else if (x == "informational")  {e = lib::log::ds::INFORMATIONAL;}
    else if (x == "verbose")        {e = lib::log::ds::VERBOSE;}
    else if (x == "debug_level_1")  {e = lib::log::ds::DEBUG_LEVEL_1;}
    else {
        e = lib::log::ds::UNDEFINED;
        in.setstate(std::ios::failbit);
    }

    return in;
}

} // namespace ds //
} // namespace log //
} // namespace lib //

                                                                                
