//------------------------------------------------------------------------------
///@file lib_work_version.h                                                     
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced.                                   
//------------------------------------------------------------------------------

#ifndef LIB_VERSION_H
#define LIB_VERSION_H

#include <stdint.h>
#include <string>

namespace lib {
namespace work {

extern const uint32_t   VERSION_MAJOR;
extern const uint32_t   VERSION_MINOR;
extern const uint32_t   VERSION_DOT;
extern const uint32_t   VERSION_BUILD;
extern const uint32_t   VERSION_RELEASE_YEAR;
extern const char*      VERSION_RELEASE_MONTH;

std::string version();
std::string version_build();

} // namespace work //
} // namespace lib  //

#endif // #ifndef LIB_VERSION_H
