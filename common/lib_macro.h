#ifndef lib_macro_h
#define lib_macro_h

//------------------------------------------------------------------------------
///@file    lib_macro.h                                                         
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                                                                     
///                                                                             
///@brief  Collection of useful (?) macros.                                     
///                                                                             
///@par  Library:  lib_macro.h                                                  
///                                                                             
///@par  Purpose:                                                               
///     The lib_macro.h library contains a useful (?) collection of C/C++       
///     macros.  It was started because too many files started having very      
///     similar looking macros.                                                 
///                                                                             
///@par   Implementation Note:                                                  
///     By their nature, being interpretted by the C/C++ precompiler,           
///     macros do not respect namespaces or class namespaces.  Therefore,       
///     though the library is generic (and should be in the lib namespace)      
///     it technically cannot be.                                               
///                                                                             
///                                                                             
///@version 2005-12-13  DHF     Added back in "CRITICAL_SECTION" definitions    
///                             making them Windows and Linux compatible.       
///                                                                             
///@version 2005-12-06  DHF     File created based on itis_macro.h; removed     
///                             Windows specific macros.                        
///                                                                             
///@version 2003-03-12  DHF     Added VARIANT_DEREF and VARIANT_TO_STRING.      
///                                                                             
///@version 2001-06-14  DHF     Added CRITICAL_INITIALIZE                       
///                                                                             
///@version 2000-10-04  DHF     Added CONCAT4                                   
///                                                                             
///@version 2000-10-01  DHF     Original file creation.                         
///                                                                             
//------------------------------------------------------------------------------

#include <string.h>     // memcpy

//------------------------------------------------------------------------------
//  Setup for differences between Windows and Linux.                            
//------------------------------------------------------------------------------
#ifndef _WIN32
#ifndef PTHREADS
#define PTHREADS
#endif // #ifndef PTHREADS
#endif // #ifndef _WIN32

#ifdef PTHREADS
//    #include <pthread.h>                    // should compile with g++ -pthread
    #define CRITICAL_SECTION pthread_mutex_t
    #define LeaveCriticalSection pthread_mutex_unlock
    #define DeleteCriticalSection pthread_mutex_destroy
    #define EnterCriticalSection pthread_mutex_lock
#endif

#ifndef PTHREADS
//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_INITIALIZE                                            
///\par  Purpose:                                                               
///     The CRITICAL_INITIALIZE macro initializes a critical section.           
///\par  Dependencies: ..                                                       
///     Under windows, windows.h                                                
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
    #define CRITICAL_INITIALIZE(x) \
        ::InitializeCriticalSection(&CONCAT(x,CriticalSection))
#else
//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_INITIALIZE                                            
///\par  Purpose:                                                               
///     The CRITICAL_INITIALIZE macro initializes a critical section.           
///\par  Dependencies: ..                                                       
///     Under windows, windows.h                                                
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
    #define CRITICAL_INITIALIZE(x)                                          \
        {                                                                   \
            pthread_mutex_t temp = PTHREAD_MUTEX_INITIALIZER;               \
            memcpy(&CONCAT(x,CriticalSection), &temp, sizeof(x));           \
        }
#endif

//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_DELETE                                                
/// \par Purpose:                                                               
///     The CRITICAL_DELETE macro releases the critical section allocated       
///     memory.                                                                 
/// \par Dependencies: ..                                                       
///     Under windows.h                                                         
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
#define CRITICAL_DELETE(x) \
        ::DeleteCriticalSection(&CONCAT(x,CriticalSection))

//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_ENTER                                                 
///\par  Purpose:                                                               
///     The CRITICAL_ENTER macro indicates that a critical section is about     
///     to be entered.  It sets up a CRITICAL_SECTION variable and traps        
///     exceptions to release the critical section.  It must be used in         
///     conjunction with CRITICAL_LEAVE (below).                                
///\par  Assumption:  xCriticalSection                                          
///     Is is assumed that there will be a CRITICAL_SECTION variable with       
///     a trailing CriticalSection on the name defined.                         
///\par  Design Note:  Why a Macro                                              
///     The CRITICAL_ENTER macro is a code fragment and not a complete          
///     routine -- therefore, it cannot be an inline function.                  
///\par  Dependencies: ..                                                       
///     Under Windows, windows.h                                                
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
#define CRITICAL_ENTER(x)                                                                                                       \
{                                                                                                                                                       \
        bool CONCAT(x,CRITICAL_SECTION_FLAG) = false;                                                   \
        try {                                                                                                                                   \
                CRITICAL_RESUME(x)

//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_LEAVE                                                 
///\par  Purpose:                                                               
///     The CRITICAL_LEAVE macro releases the critical section that was         
///     started by CRITICAL_ENTER and terminates the extra error handling       
///     to ensure that the ::LeaveCriticalSection routine is called.            
///\par  Assumption:  (1)  xCriticalSection                                     
///     Is is assumed that there will be a CRITICAL_SECTION variable with       
///     a trailing CriticalSection on the name defined.                         
///\par  Constraint:  (1) Don't use return                                      
///     While exceptions being thrown will be caught ... do not use a           
///     return without first suspending (or leaving) the critical section.      
///\par  Design Note:  Why a Macro                                              
///     The CRITICAL_LEAVE macro is a code fragment and not a complete          
///     routine -- therefore, it cannot be an inline function.                  
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//----------------------------------------------------------------------------------
#define CRITICAL_LEAVE(x)                                                   \
        } catch(...) {                                                      \
                CRITICAL_SUSPEND(x)                                         \
                throw;                                                      \
        }                                                                   \
        CRITICAL_SUSPEND(x)                                                 \
}

//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_LEAVE_NOTHROW                                         
///\par  Purpose:                                                               
///     The CRITICAL_LEAVE_NOTHROW macro operates the same as the macro         
///     CRITICAL_LEAVE except that it will supress the exception.               
///     \see CRITICAL_LEAVE                                                     
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
#define CRITICAL_LEAVE_NOTHROW(x)                                                                                       \
        } catch(...) {                                                                                                                  \
        }                                                                                                                                               \
        CRITICAL_SUSPEND(x)                                                                                                             \
}

//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_SUSPEND                                               
///\par  Purpose:                                                               
///     The CRITICAL_SUSPEND macro is used to temporarily suspend a             
///     critical section hold on an object.  It is used in conjunction with     
///     CRITICAL_ENTER, CRITICAL_LEAVE, and CRITICAL_RESUME.                    
///\par  Assumption:  (1)  xCriticalSection                                     
///     Is is assumed that there will be a CRITICAL_SECTION variable with       
///     a trailing CriticalSection on the name defined.                         
///\par  Design Note:  Why a Macro                                              
///     The CRITICAL_SUSPEND macro is a code fragment and not a complete        
///     routine -- therefore, it cannot be an inline function.                  
///\par  Dependencies: ..                                                       
///             windows.h                                                       
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
#define CRITICAL_SUSPEND(x)                                                                                                     \
        if (CONCAT(x,CRITICAL_SECTION_FLAG)) {                                                                  \
                ::LeaveCriticalSection(&CONCAT(x,CriticalSection));                                     \
                CONCAT(x,CRITICAL_SECTION_FLAG) = false;                                                        \
        }

//------------------------------------------------------------------------------
///\par  Macro:  CRITICAL_RESUME                                                
///\par  Purpose:                                                               
///     The CRITICAL_RESUME macro is used to resume a critical section          
///     after having been suspended by the CRITICAL_SUSPEND above.  It          
///     works in conjunction with CRITICAL_ENTER, CRITICAL_LEAVE, and           
///     CRITICAL_SUSPEND.                                                       
///\par  Assumption:  (1)  xCriticalSection                                     
///     Is is assumed that there will be a CRITICAL_SECTION variable with       
///     a trailing CriticalSection on the name defined.                         
///\par  Design Note:  Why a Macro                                              
///     The CRITICAL_RESUME macro is a code fragment and not a complete         
///     routine -- therefore, it cannot be an inline function.                  
///\par  Dependencies: ..                                                       
///     Under Windows, windows.h                                                
///@deprecated  Use lib::mp::thread::ProtectedObject instread                   
//------------------------------------------------------------------------------
#define CRITICAL_RESUME(x)                                                                                                      \
        if (!CONCAT(x,CRITICAL_SECTION_FLAG)) {                                                                 \
                ::EnterCriticalSection(&CONCAT(x,CriticalSection));                                     \
                CONCAT(x,CRITICAL_SECTION_FLAG) = true;                                                         \
        }

#ifndef QUOTE
//------------------------------------------------------------------------------
///\par   Macro:  QUOTE                                                         
///\par   Purpose:                                                              
///     The QUOTE macro has the C preprocessor place quotation marks around     
///     the given text.                                                         
///\param     a       The text to be quoted.                                    
///\par   Example:  (1) simple case                                             
///     Code:                                                                   
///         std::cout << QUOTE(Hello World) << std::endl;                       
///     Output Results:                                                         
///         Hello World                                                         
///\par   Example:  (2) more complex case                                       
///     Code:                                                                   
///         std::cout << QUOTE(CONCAT(Hello,World)) << std::endl;               
///     Output Results:                                                         
///         CONCAT(Hello, World)                                                
///     Interpretation:                                                         
///         The QUOTE macro is executed before the CONCAT macro is seen;        
///         and the CONCAT macro is never seen because it is contained in       
///         quotation marks as far as the C/C++ precompiler is concerned.       
///                                                                             
///\par   Design Note:  Why a Macro                                             
///     The heart of the purpose of the QUOTE macro is to manipulate            
///     the C preprocessor -- and hence it must be a macro versus an inline     
///     function.                                                               
//------------------------------------------------------------------------------
#define QUOTE(a)    #a
#endif

#ifndef QUOTE2
//------------------------------------------------------------------------------
///\par   Macro:  QUOTE2                                                        
///                                                                             
///\par   Purpose:                                                              
///     The QUOTE2 macro has the C preprocessor place quotation marks           
///     around the given text after other macros have been processed.           
///                                                                             
///\param     a       The text to be quoted.                                    
///                                                                             
///\par   Example:  (1) simple case                                             
///     Code:                                                                   
///         std::cout << QUOTE2(Hello World) << std::endl;                      
///     Output Results:                                                         
///         Hello World                                                         
///                                                                             
///\par   Example:  (2) complex case                                            
///     Code:                                                                   
///         std::cout << QUOTE2(CONCAT(Hello,World)) << std::endl;              
///     Output Results:                                                         
///         HelloWorld                                                          
///     Interpretation:                                                         
///         The QUOTE2 macro is executed after the CONCAT macro is executed.    
///                                                                             
///\par   Design Note:  Why a Macro                                             
///     The heart of the purpose of the QUOTE2 macro is to play manipulate      
///     the C preprocessor -- and hence it must be a macro versus an inline     
///     function.                                                               
///                                                                             
//------------------------------------------------------------------------------
#define QUOTE2(a)   QUOTE(a)
#endif

#ifndef LOGIC_ERROR2
//------------------------------------------------------------------------------
///\par  Macro:  LOGIC_ERROR2                                                   
///\par  Purpose:                                                               
///     The LOGIC_ERROR2 macro should not be called directly.  It works in      
///     conjunction with LOGIC_ERROR and is used to translate the line          
///     number into a character string in order to implicity concatenate        
///     the values.                                                             
//------------------------------------------------------------------------------
#define LOGIC_ERROR2(file, line, err)                                       \
    throw std::logic_error(file " (" QUOTE(line) "): " err)
#endif

#ifndef LOGIC_ERROR
//------------------------------------------------------------------------------
///\par  Macro: LOGIC_ERROR                                                     
///\par  Purpose:                                                               
///     The LOGIC_ERROR macro throws a std::logic_error exception and           
///     includes the file and line number prepended to the given message.       
///\param   err     The message to be contained within the logic error's        
///                 what() value.                                               
///\warning  Character Array Literal Expected.                                  
///     The parameter to LOGIC_ERROR should be a "character string" literal     
///     due to implict character string concatentation.                         
//------------------------------------------------------------------------------
#define LOGIC_ERROR(err)    LOGIC_ERROR2(__FILE__, __LINE__, err)
#endif

#ifndef CONCAT
//------------------------------------------------------------------------------
///\par   Macro:  CONCAT                                                        
///\par   Purpose:                                                              
///     The CONCAT macro has the C preprocessor concatentate two elements       
///     together, sending the results to the compiler.  For example, if         
///     CONCAT(get_,Field) is used, the compiler sees get_Field.                
///\param   a   The element to be concatenated on the left of the other.        
///\param   b   The element to be concatenated on the right of the other.       
//------------------------------------------------------------------------------
#define CONCAT(a,b)     a ## b
#endif

#ifndef CONCAT3
//------------------------------------------------------------------------------
///\par   Macro:  CONCAT3                                                       
///\par   Purpose:                                                              
///     The CONCAT3 macro has the C preprocessor concatentate three             
///     elements together, sending the results to the compiler.  For            
///     example, if CONCAT3(get_,Field,Error) is used, the compiler sees        
///     get_FieldError.                                                         
///\param   a   The element to be concatenated on the left of the others.       
///\param   b   The element to be concatenated in the middle of the others.     
///\param   c   The element to be concatenated on the right of the others.      
//------------------------------------------------------------------------------
#define CONCAT3(a,b,c)  a ## b ## c
#endif

#ifndef CONCAT4
//------------------------------------------------------------------------------
///\par   Macro:  CONCAT4                                                       
///\par   Purpose:                                                              
///     The CONCAT4 macro has the C preprocessor concatentate four              
///     elements together, sending the results to the compiler.  For            
///     example, if CONCAT4(class,::get_,Field,Error) is used, the compiler     
///     sees class::get_FieldError.                                             
///\param   a   The element to be concatenated on the left of the others.       
///\param   b   The element to the right of a and left of c and d.              
///\param   c   The element to the right of a and b, left of d.                 
///\param   d   The element to be concatenated on the right of the others.      
//------------------------------------------------------------------------------
#define CONCAT4(a,b,c,d)    a ## b ## c ## d
#endif

#ifndef CONCAT5
//------------------------------------------------------------------------------
///\par   Macro:  CONCAT5                                                       
///\par   Purpose:                                                              
///     The CONCAT5 macro has the C preprocessor concatentate five              
///     elements together, sending the results to the compiler.  For            
///     example, if CONCAT5(class,::get_,Field,Error) is used, the compiler     
///     sees class::get_FieldError.                                             
///\param   a   The element to be concatenated on the left of the others.       
///\param   b   The element to the right of a and left of c, d, and e.          
///\param   c   The element to the right of a and b, left of d and e.           
///\param   d   The element to the right of a, b, and c, left of e.             
///\param   e   The element to be concatenated on the right of the others.      
//------------------------------------------------------------------------------
#define CONCAT5(a,b,c,d,e)  a ## b ## c ## d ## e
#endif

#endif // #ifndef lib_macro_h //

