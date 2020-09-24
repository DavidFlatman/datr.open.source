//------------------------------------------------------------------------------
///@file lib_string.h                                                           
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///@brief A collection of functions that operate on std::string                 
//------------------------------------------------------------------------------
#ifndef LIB_STRING_H
#define LIB_STRING_H

#define __STDC_FORMAT_MACROS
#include <cstdlib>
#include <inttypes.h>           // PRId64               
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "lib_compiler_info.h"  // get the compiler type discriminators

namespace lib {

//------------------------------------------------------------------------------
///@par Not Another String Class                                                
///         This is NOT yet another string class.  It does allow a number of C  
///         string functions to work on std::string (and maybe just a little    
///         more).                                                              
///@par Notes                                                                   
///         1.  needs to work with std::string                                  
///         2.  don't create our own string class                               
///         3.  implementation:                                                 
///             -   if stl can do, use stl                                      
///             -   if boost can do, use boost                                  
///             -   roll your own                                               
///     4.  DATR lib coding standards; wrapper around stl, boost                
///             -   camelCaseFunctions                                          
///             -   first argument is always the string being operated on       
///                 passed as const reference                                   
///             -   prefer result returned (e.g., toLower(myString) == "abc")   
///             -   namespace lib { ... }                                       
///                                                                             
///     A.  Functions                                                           
///             - int64_t toInteger(const std::string&, int base=0);            
///                 -   leading +/-                                             
///                 -   if (base==0) 0xABCD implies base 16, else base 10       
///             - std::string format(const std::string& fmt, ...);              
///             - conversion                                                    
///                 -   std::string toUpper(const std::string&);                
///                 -   std::string toLower(const std::string&);                
///                 -   std::string trimLeft (" a    b  c ") == "a    b  c "    
///                 -   std::string trimRight(" a    b  c ") == " a    b  c"    
///                 -   std::string trim     (" a    b  c ") == "a    b  c"     
///                 -   std::string compress (" a    b  c ", bool trim = true) == "a b c"
///                 -   std::string collapse (" a    b  c ", bool trim = true) == "abc"
///                 -   std::string right(const std::string&, size_t x);        
///                 -   std::string left (const std::string&, size_t x);        
///             - eat-quote                                                     
///                 -   ?                                                       
///             - is                                                            
///                 -   isAllAlpha(const std::string&, bool allowSpace=false, bool allowPunc=false);
///                 -   isAllDigit(const std::string&);                         
///                 -   isAllAlphaNumeric(const std::string&);                  
///                 -   hasDigit  (const std::string&);    // true if at least 1 digit
///                 -   isBinary  (const std::string&);                         
///                 -   isOctal   (const std::string&);                         
///                 -   isHex     (const std::string&);                         
///                 -   isNumber  (const std::string&);                         
///                       -   scientific notation allowed                       
///                           ([+-]{0,1}\d+[Ee][+-]{0,1}\d+)                    
///                       -   integer                                           
///                       -   float                                             
///                                                                             
///                 -   beginsWith(const std::string&, const std::string) // replaces strncmp
///                 -   static bool InstrumentTime::isValid(const std::string&);
///                       -   "9:2:1" is "09:02:01"                             
///                                                                             
///             - std::vector<std::string> split(const std::string&, const std::string& delimiter, const std::string& quote="");
///                                                                             
///@par >Imprelementation Note:  Definition of Whitespace                       
///         The isspace function identifies any of the following characters:    
///         space, formfeed (\f), newline (\n), carriage return (\r), tab (\t), 
///         and vertical tab (\v).  This collection of functions uses this same 
///         defintion when use of the isspace function is not practical.        
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2017-05-31  MNT         Renamed                                     
///                                   - setFixedCharArray() toFixedCharArray()  
///                                   - getFixedCharArray() fromFixedCharArray()
///                                                                             
///@version 2017-03-28  MNT         Added setFixedCharArray & getFixedCharArray 
///                                                                             
///@version 2015-07-08  MNT         Replaced DateTime by InstrumentationTime    
///                                                                             
///@version 2012-04-04  AGB v1.00   Merged Andy's string manipulation stuff     
///                                 with Dave's                                 
//------------------------------------------------------------------------------
///@todo Research the need for spanExcluding and eatQuotes                      
///@todo Clean up all the commented-out stuff                                   

//------------------------------------------------------------------------------
//  Booleans                                                                    
//------------------------------------------------------------------------------
bool hasDigit(const std::string& str);

bool isAllAlpha(const std::string& str, bool allowspace=false, bool allowpunct=false);
bool isBinary(const std::string& str);
bool isHex(const std::string& str);
bool isNumber(const std::string& str, bool allowcomma=false);
bool isOctal(const std::string& str);

//----------------------------------------------------------------------------- 
//  Visual Studio uses a loophole in the standard to be stupid about passing a  
//  reference to va_start.                                                      
//----------------------------------------------------------------------------- 
#if IS_VISUAL_STUDIO
std::string format(const std::string format, ...);
#else
std::string format(const std::string& format, ...);
#endif

//------------------------------------------------------------------------------
//  Numerics                                                                    
//------------------------------------------------------------------------------
int64_t toSigned(const std::string&, int base=0);
uint64_t toUnsigned(const std::string&, int base=0);

inline std::string toString(int8_t val)   { return format("%" PRIi8, val);  }
inline std::string toString(int16_t val)  { return format("%" PRIi16, val); }
inline std::string toString(int32_t val)  { return format("%" PRIi32, val); }
inline std::string toString(int64_t val)  { return format("%" PRIi64, val); }

inline std::string toString(uint8_t val)  { return format("%" PRIu8, val); }
inline std::string toString(uint16_t val) { return format("%" PRIu16, val); }
inline std::string toString(uint32_t val) { return format("%" PRIu32, val); }
inline std::string toString(uint64_t val) { return format("%" PRIu64, val); }

inline std::string toString(double val)   { return format("%f", val); }

//------------------------------------------------------------------------------
std::string comma(const std::string& str, const std::string& delim=",", int count=3);
inline std::string comma(
    const char* str, const std::string& delim=",", int count=3
) { return comma(std::string(str), delim, count); }

template <typename TYPE>
std::string comma(TYPE val, const std::string& delim=",", int count=3)
    { return comma(toString(val), delim, count); }

#if 0
inline std::string comma(int8_t val, const std::string& delim=",",  int count=3)
    { return comma(format("%" PRIi8, val), delim, count); }
inline std::string comma(int16_t val, const std::string& delim=",", int count=3)
    { return comma(format("%" PRIi16, val), delim, count); }
inline std::string comma(int32_t val, const std::string& delim=",", int count=3)
    { return comma(format("%" PRIi32, val), delim, count); }
inline std::string comma(int64_t val, const std::string& delim=",", int count=3)
    { return comma(format("%" PRIi64, val), delim, count); }

inline std::string comma(uint8_t val, const std::string& delim=",", int count=3)
    { return comma(format("%" PRIu8, val), delim, count); }
inline std::string comma(uint16_t val, const std::string& delim=",",int count=3)
    { return comma(format("%" PRIu16, val), delim, count); }
inline std::string comma(uint32_t val, const std::string& delim=",",int count=3)
    { return comma(format("%" PRIu32, val), delim, count); }
inline std::string comma(uint64_t val, const std::string& delim=",",int count=3)
    { return comma(format("%" PRIu64, val), delim, count); }

inline std::string comma(double val, const std::string& delim=",", int count=3)
    { return comma(format("%f", val), delim, count); }
#endif

//------------------------------------------------------------------------------
//  Subsets                                                                     
//------------------------------------------------------------------------------
#if 0
//  beginsWith is deprecated in favor of boost::algorithm::starts_with          
int beginsWith(const std::string& a, const std::string& b, int32_t n = -1);
#endif

std::string left(const std::string& str, int ct);
std::string right(const std::string& str, int ct);

//------------------------------------------------------------------------------
//  Transformations                                                             
//------------------------------------------------------------------------------
std::string collapse(const std::string& str, bool trm=true);
std::string compress(const std::string& str, bool trm=true);

//------------------------------------------------------------------------------
//  XML encoding                                                                
//------------------------------------------------------------------------------
std::string xmlEncode(const std::string& str);
std::string xmlDecode(const std::string& str);

bool isAllDigit(const std::string& str);
bool isAlphaNumeric(const std::string& str);

double toDouble(const std::string& str, double errorValue = 0);

std::string trim(const std::string& str, const std::string& white = " \t\n\v\r\f");
std::string trimLeft(const std::string& str, const std::string& white = " \t\n\v\r\f");
std::string trimRight(const std::string& str, const std::string& white = " \t\n\v\r\f");

std::string toLower(const std::string& str);
std::string toUpper(const std::string& str);

void replace_all(std::string& str, const std::string& search,
    const std::string& format, int max_replace = 100000);

//------------------------------------------------------------------------------
//  Fixed size char array manipulation.                                         
//------------------------------------------------------------------------------
bool toFixedCharArray(
    char*               destination
  , size_t              destination_max_size
  , const std::string&  source
  , char                pad = ' '
);

std::string fromFixedCharArray(
    const char* source
  , size_t      source_max_size
  , char        trim_trailing = ' '
);

} // namespace lib

#endif // #ifndef LIB_STRING_H

