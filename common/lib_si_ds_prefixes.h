//------------------------------------------------------------------------------
///@par Classification:  UNCLASSFIIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced.                                   
//------------------------------------------------------------------------------
#ifndef LIB_SI_PREFIXES_H
#define LIB_SI_PREFIXES_H

#include <stdint.h>

namespace lib {
namespace si {
namespace ds {

extern uint64_t    deci;                        //  pow(10, -1)                 
extern uint64_t    centi;                       //  pow(10, -2)                 
extern uint64_t    milli;                       //  pow(10, -3)                 
extern uint64_t    micro;                       //  pow(10, -6)                 
extern uint64_t    nano;                        //  pow(10, -9)                 
extern uint64_t    pico;                        //  pos(10,-12)                 

extern uint64_t deca;
extern uint64_t hecto;
extern uint64_t kilo;
extern uint64_t mega;
extern uint64_t giga;
extern uint64_t tera;
extern uint64_t peta;
extern uint64_t exa;

} // namespace ds 
} // namespace si   
} // namespace lib  

#endif // #ifndef LIB_SI_PREFIXES_H
