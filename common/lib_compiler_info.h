//------------------------------------------------------------------------------
///@file lib_compiler_info.h                                                    
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_COMPILER_INFO_H_FILE_GUARD
#define LIB_COMPILER_INFO_H_FILE_GUARD

#include <stdint.h>

//------------------------------------------------------------------------------
///                                                                             
///@par Library: Info                                                           
///                                                                             
///@brief   Centralize compiler identification.                                 
///                                                                             
///         There are two different concepts that are easy to confuse that need 
///         to be kept straight:                                                
///             1.  The compiler, and                                           
///             2.  The operating system.                                       
///                                                                             
///         Yes.  I can hear you now: "well, duh."  Not so fast.  Is the        
///         issue that you're trying to #ifdef around a Visual Studio problem   
///         or a Microsoft Windows problem?  Now we're being more subtle.  Be   
///         clear about whether it is the operating system giving you grief or  
///         the compiler and use the appropriate symbols accordingly.           
///                                                                             
///@par Compiler Symbols                                                        
///         -   IS_CLANG_COMPILER                                               
///         -   IS_GCC_COMPILER                                                 
///         -   IS_INTEL_COMPILER                                               
///         -   IS_MINGW_COMPILER                                               
///         -   IS_VISUAL_STUDIO_COMPILER                                       
///                                                                             
///@par Operating System Symbols                                                
///         -   IS_LINUX                                                        
///         -   IS_UNIX_LIKE                                                    
///         -   IS_WINDOWS                                                      
///                                                                             
///@par Thread Safety:  object                                                  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2018-08-07  DHF     Added IS_WINDOWS_32BIT and IS_WINDOWS_64BIT.    
///                                                                             
///@version 2018-06-12  DHF/MNT Added IS_LITTLE_ENDIAN                          
///                                                                             
///@version 2015-03-25  DHF     Change "or" to "||" for Visual Studio.          
///                                                                             
///@version 2015-01-22  DHF     Reoganized to distinguish between compiler      
///                             versus operating system; update values based on 
///                             sourceforge.net/p/predef/wiki/OperatingSystems/ 
///                             and sourceforge.net/p/predef/wiki/Compilers/    
///                                                                             
///@version 2012-02-16  AGB/DHF File creation                                   
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Determine which compiler ...                                                
//------------------------------------------------------------------------------
#ifdef __CC_ARM
    #define IS_ARM_COMPILER     1
#endif

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
    #define IS_BORLAND_COMPILER 1

#endif

#ifdef __clang__ 
    #define IS_CLANG_COMPILER   1

#endif

#if defined(__DECCXX) || defined(__DECC) 
    #define IS_DEC_COMPILER     1

#endif

#ifdef __GNUC__ 
    #define IS_GCC_COMPILER     1

#endif

#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || _ECC 
    #define IS_INTEL_COMPILER   1

#endif

#ifdef __llvm__
    #define IS_LLVM_COMPILER    1

#endif

#ifdef _MSC_VER 
    #define IS_VISUAL_STUDIO    1

    #if _MSC_VER < 1900
        #define IS_VISUAL_STUDIO_PRE_2015
    #endif

#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
    #define IS_MINGW_COMPILER   1
    #define IS_GCC_COMPILER     1
#endif

#if defined(__SUNPRO_CC) || defined(__SUNPRO_C) 
    #define IS_SUN_COMPILER     1

#endif

//------------------------------------------------------------------------------
//  Determine the operating system                                              
//------------------------------------------------------------------------------
#ifdef _AIX
    #define IS_AIX  1
    #define IS_UNIX_LIKE    1

#endif

#ifdef __ANDROID__
    #define IS_ANDROID
    #define IS_UNIX_LIKE    1

#endif

#ifdef __CYGWIN__
    #define IS_WINDOWS

#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__)
    #define IS_BSD          1
    #define IS_UNIX_LIKE    1

#endif

#if defined(__gnu_linux__) || defined(__linux__)
    #define IS_LINUX        1
    #define IS_UNIX_LIKE    1

#endif

#if defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
    #define IS_BSD          1
    #define IS_APPLE        1
    #define IS_UNIX_LIKE    1

#endif

#ifdef EPLAN9
    #define IS_PLAN9        1

#endif

#if defined(__QNX__) || defined(__QNXNTO__)
    #define IS_QNX

#endif

#if defined(sun) || defined(__sun)
    #define IS_SOLARIS      1
    #define IS_UNIX_LIKE    1

#endif

#ifdef __unix__
    #define IS_UNIX         1
    #define IS_UNIX_LIKE    1

#endif

#if defined(VMS) || defined(__VMS)
    #define IS_VMS          1

#endif

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
    #define IS_WINDOWS      1

#endif

#if defined(IS_WINDOWS) && !defined(_WIN64)
    #define IS_WINDOWS_32BIT
#endif
#if defined(IS_WIDNOWS) && defined(_WIN64)
    #define IS_WINDWOS_64BIT
#endif




//------------------------------------------------------------------------------
//  Determine Endianness                                                        
//------------------------------------------------------------------------------
#define IS_LITTLE_ENDIAN    1

#if 0
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  OLD STUFF
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef _MSC_VER
#define IS_VISUAL_STUDIO 1
//#pragma message "is visual studio"

#elif _WIN32
#define IS_MINGW    1
//#pragma message "is MinGW"

#elif __INTEL_COMPILER
#define IS_LINUX    1
//#pragma message "is Linux"

#endif

#if __INTEL_COMPILER
#define IS_INTEL_COMPILER   1
//#pragma message "is Intel"

#endif

#endif      // #if 0

#endif // #ifndef LIB_COMPILER_INFO_H_FILE_GUARD

