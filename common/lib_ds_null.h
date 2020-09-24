//------------------------------------------------------------------------------
///@file lib_ds_null.h                                                          
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_DS_NULL_H_FILE_GUARD
#define LIB_DS_NULL_H_FILE_GUARD

namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Null                                                             
///                                                                             
///@brief   Useful as a default parameter to a template typename.               
///                                                                             
///@par Purpose:                                                                
///         The Null template doesn't do anything (as the name implies).  The   
///         original purpose was to create default parameters to the            
///         lib::msg::Subscriber template.                                      
///                                                                             
///@note    All of the NULL_DEFAULT_TYPE_n types are std::is_convertible to   
///         NULL_BASE.                                                          
///                                                                             
///@par Thread Safety:  object (go figure)                                      
///         The class doesn't do anything ... so it's not possible for multiple 
///         threads to step on each other's toes.                               
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2016-11-01  DHF     Moved from lib::msg:: namespace to lib::ds::    
///                                                                             
///@version 2015-08-30  DHF     Add virtual destructor to NULL_BASE to allow    
///                             dynamic_cast of derived objects.                
///                                                                             
///@version 2013-05-08  DHF     Added typedefs for NULL_DEFAULT_TYPE_0 - 7      
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-09-09  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

struct NULL_BASE { virtual ~NULL_BASE() { } };
#define UNDEF_CONCAT(a,b) a ## b

#define DEFINE_NULL(COUNT)                                                  \
    struct UNDEF_CONCAT(NULL_DEFAULT_TYPE_, COUNT) : public NULL_BASE {     \
        UNDEF_CONCAT(NULL_DEFAULT_TYPE_, COUNT)() { }                       \
        UNDEF_CONCAT(NULL_DEFAULT_TYPE_, COUNT)(const NULL_BASE& b) { }     \
    };

DEFINE_NULL(0);     // define NULL_DEFAULT_TYPE_0
DEFINE_NULL(1);     // define NULL_DEFAULT_TYPE_1
DEFINE_NULL(2);     // define NULL_DEFAULT_TYPE_2
DEFINE_NULL(3);     // define NULL_DEFAULT_TYPE_3
DEFINE_NULL(4);     // define NULL_DEFAULT_TYPE_4
DEFINE_NULL(5);     // define NULL_DEFAULT_TYPE_5
DEFINE_NULL(6);     // define NULL_DEFAULT_TYPE_6
DEFINE_NULL(7);     // define NULL_DEFAULT_TYPE_7
DEFINE_NULL(8);     // define NULL_DEFAULT_TYPE_8

#undef DEFINE_NULL
#undef UNDEF_CONCAT

}  // namespace ds //
}  // namespace lib //

#endif // #ifndef LIB_DS_NULL_H_FILE_GUARD
