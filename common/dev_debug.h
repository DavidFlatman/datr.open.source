//------------------------------------------------------------------------------
///@file dev_debug.h                                                            
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Various debugging routines for the developer.                       
///                                                                             
///@warning In order for some of these routines to work, the code should be     
///         compiled with:                                                      
///             % g++ -rdynamic -g3 -O0                                         
///         This has the effect of:                                             
///             -   -rdynamic   Turns on name tracing for getting the calling   
///                             routine names.                                  
///             -   -g3         Adds extended debugger information.             
///             -   -O0         Turns optimizations off.                        
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2017-10-05  AGB     Modified the code in the following way.  Old:   
///                             namespace debug {                               
///                             #if IS_LINUX                                    
///                             } // namespace debug                            
///                             #endif                                          
///                                                                             
///                             New:                                            
///                             namespace debug {                               
///                             #if IS_LINUX                                    
///                             #endif                                          
///                             } // namespace debug                            
///                                                                             
///@version 2017-03-20  DHF     Added Time class.                               
///                                                                             
///@version 2014-02-25  DHF     Added proper comment header.                    
///                                                                             
//------------------------------------------------------------------------------

#ifndef DEV_DEBUG_H_FILE_GUARD
#define DEV_DEBUG_H_FILE_GUARD


#include "lib_compiler_info.h"
#include "lib_time_work_datetime.h"
#include "lib_time_work_deltatime.h"

#include <string>

namespace dev {
namespace debug {

#if IS_LINUX

void initialize(int argc, char** argv);

#else

inline void initialize(int argc, char** argv) { }

#endif

#if IS_LINUX

//------------------------------------------------------------------------------
///@note    The initial state is paused.                                        
//------------------------------------------------------------------------------
class Time
{
    public:
        Time(const std::string& name = "anonymous");
        Time(const Time& that) = default;
        virtual ~Time();

        Time& operator=(const Time& that) = default;

        void start();
        void pause();
        void reset( bool start = false);

        bool isRunning() const;
        bool isPaused()  const;

        lib::time::work::DeltaTime deltaCPU () const;
        lib::time::work::DeltaTime deltaWall() const;

        void setName(const std::string& name);
        void setDisplayOnExit(bool doe = true);

        static lib::time::work::DeltaTime  currentCPU();
    private:


    private:
        bool                            m_Paused;
        bool                            m_DisplayOnExit;

        lib::time::work::DeltaTime      m_DeltaCPU;
        lib::time::work::DeltaTime      m_LastCPU;

        lib::time::work::DeltaTime      m_DeltaWall;
        lib::time::work::DateTime       m_LastWall;

        std::string                     m_Name;

};

//------------------------------------------------------------------------------
///@par Expected Usage:                                                         
///         @code                                                               
///             void myNonThreadableFunction()                                  
///             {                                                               
///                 static dev::debug::Time my_timer("my-timer: " __FUNCTION__);
///                 dev::debug::TimeScope(my_timer);                            
///                 //                                                          
///                 //  ... some code that may have a performance problem ...   
///                 //                                                          
///             }                                                               
///         @endcode                                                            
///                                                                             
///         In the above code:                                                  
///             -   The dev::debug::Time is declared static so that it records  
///                 all the times that the myNonThreadableFunction is called.   
///             -   The dev::debug::TimeScope is not static so that when it     
///                 goes out of scope it stops recording the time.              
///             -   When the application terminates, the static variable goes   
///                 out of scope and displays the results.                      
//------------------------------------------------------------------------------
class TimeScope
{
    public:
        TimeScope(Time& timer) : m_Timer(timer) { m_Timer.start(); }
        ~TimeScope()                            { m_Timer.pause(); }

    private:
        Time&   m_Timer;
};

//------------------------------------------------------------------------------
//  AGB - Visual Studio is much happier with the #endif appearing before the }
//  that ends the namespace scope.
//------------------------------------------------------------------------------
#endif

} // namespace debug    
} // namespace dev      

#ifndef CONCAT
#define CONCAT(a, b)    a ## b
#endif

//------------------------------------------------------------------------------
//  Why?!?                                                                      
//                                                                              
//  The DEBUG_TIMER_MSG is what the developer wants to use and we want to       
//  add the line number (__LINE__) to both the text message and to variable     
//  names.                                                                      
//                                                                              
//  The DEBUG_TIMER_MSG2 has line as __LINE__ and not the number, so we turn    
//  around and call DEBUG_TIMER_MSG3 to translate the __LINE__ into the         
//  line number passed in by DEUB_PROFILE_TIME.                                 
//------------------------------------------------------------------------------
#define DEBUG_TIMER_MSG3(x, line)                                             \
    static dev::debug::Time CONCAT(named_cpu, line)(std::string(x));          \
    dev::debug::TimeScope CONCAT(scope_timer, line)(CONCAT(named_cpu, line))

#define DEBUG_TIMER_MSG2(x, line)                                             \
    DEBUG_TIMER_MSG3(x, line)

//------------------------------------------------------------------------------
///@par Expected Usage:                                                         
///         @code                                                               
///             void myNonThreadSafeFunction()                                  
///             {                                                               
///                 DEBUG_TIMER_MSG(__FUNCTION__);                              
///                                                                             
///                 // .. code ..                                               
///                                                                             
///                 {                                                           
///                     DEBUG_TIMER_MSG(__FUNCTION__);                          
///                     while (...) {                                           
///                     }                                                       
///                 }                                                           
///                                                                             
///                 DEBUG_TIMER_MSG(__FUNCTION__);                              
///                 // ... wrap up code ..                                      
///             }                                                               
///         @endcode                                                            
///                                                                             
///         In the above,                                                       
///             -   the first DEBUG_TIMER_MSG profiles the entire function.     
///             -   the second DEBUG_TIMER_MSG profiles the while loop.         
///             -   the third DEBUG_TIMER_MSG profiles the wrap up code.        
//------------------------------------------------------------------------------
#define DEBUG_TIMER_MSG(x)                                                  \
    DEBUG_TIMER_MSG2(x, __LINE__)

#define DEBUG_TIMER3(file, line)                                            \
    DEBUG_TIMER_MSG(file " " #line)

#define DEBUG_TIMER2(file, line)                                            \
    DEBUG_TIMER3(file, line)

#define DEBUG_TIMER                                                         \
    DEBUG_TIMER2(__FILE__, __LINE__)

//------------------------------------------------------------------------------
//  AGB - This #endif is outside the scope of the namespace statements, which
//  upsets Visual Studio.
//------------------------------------------------------------------------------
//#endif

#endif // #ifndef DEV_DEBUG_H_FILE_GUARD
