#ifndef LIB_THROW_H_FILE_GUARD
#define LIB_THROW_H_FILE_GUARD

#include "lib_compiler_info.h"

//------------------------------------------------------------------------------
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                                                                                                                                                  
///@macro THROW                                                                 
///                                                                             
///@brief Handle trailing "throw" syntax on routine signatures.                 
///                                                                             
///@par Why?                                                                    
///         Because the geniuses at Microsoft haven't implemented the "throw"   
///         portion of a method signature and when deriving from a virtual      
///         method that does properly identify the possible exceptions thrown,  
///         you have to follow suit.                                            
///                                                                             
///                                                                             
///@version 2013-10-03  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------

#ifdef IS_VISUAL_STUDIO
    #define THROW(x)
#else
    #define THROW(x) throw x
#endif

#endif // #ifndef LIB_THROW_H_FILE_GUARD
