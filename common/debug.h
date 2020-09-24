//------------------------------------------------------------------------------
///@file debug.h                                                                
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief Holds some useful^W primitive debugging tools.                        
///                                                                             
///@par Expected Usage                                                          
///     @code                                                                   
///         #include "debug.h"                                                  
///                                                                             
///         int main(int argc, char* argv)                                      
///         {                                                                   
///             DEBUG("before declaration of x = mightBeCrashSource()");        
///             int x = mightBeCrashSource();                                   
///             DEBUG("after  declaration of x = mightBeCrashSource()");        
///                                                                             
///             DEBUGVAR(x);        // display in decimal.                      
///             DEBUGVARHEX(x);     // display in hex.                          
///         }                                                                   
///     @endcode                                                                
///\n\n                                                                         
///     The results would look like:                                            
///         @code                                                               
///             foo.cpp: main (5): before declartion of x = mightBeCrashSource()
///             foo.cpp: main (7): after declartion of x = mightBeCrashSource() 
///             foo.cpp: main (9): x = '42'                                     
///             foo.cpp: main (10): x = 0x2a                                    
///         @endcode                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced.                                   
///                                                                             
///@version 2015-03-17  DHF     Added header comments.                          
///                                                                             
///@version 2015-03-17  AGB     Added DEBUGVARHEX.                              
///                                                                             
///@version 199?-??-??  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <stdio.h>

#define DEBUG(x) std::cout << __FILE__ << ": " << __FUNCTION__ << " (" << __LINE__ << "): " << x << std::endl
//#define DEBUG(x) std::cerr << __FUNCTION__ << " (" << __LINE__ << "): " << x << std::endl
#define DEBUGVAR(x) DEBUG(#x << " = '" << x << "'")
#define DEBUGVARHEX(x)  DEBUG(#x << " = 0x" << std::hex << x << std::dec)
#define DEBUGVARS(x) std::cout << #x << " = '" << x << "';  "
#define DEBUGVARS_FIRST(x) {std::cout << __FUNCTION__ << " (" << __LINE__ << "): "; DEBUGVARS(x); }
#define DEBUGVARS_LAST(x) {DEBUGVARS(x); std::cout << std::endl;}

template <typename TYPE>
std::string HEX(TYPE x, int base = 16) {

    const char digits[] = "0123456789ABCDEF";
    const char* prefix[] = {
        ""
      , ""
      , ""
      , ""
      , ""
      , ""
      , ""
      , ""
      , "0"
      , ""
      , ""
      , ""
      , ""
      , ""
      , ""
      , ""
      , "0x"
    };

    std::string result;

    if (x == 0) {
        return "0x00";
    }

    std::string sign = "";
    if (x < 0) {
        sign = "-";
        x = 0 - x;
    }

    while (x > 0) {
        result = digits[x % base] + result;
        x /= base;
    }

    if (result.length() < 2) result = "0" + result;

    return sign + prefix[base] + result;
}

#define OCT(x) HEX(x, 8)

#define S(x) (x).toStdString()

void debug_hexdump(const void* memory, size_t bytes);
void debug_hexdump(const std::string& str);
void debug_diff(const std::string& left, const std::string& right);

#endif
