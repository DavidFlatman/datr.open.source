//------------------------------------------------------------------------------
///                                                                             
///@file dev_debug.cpp                                                          
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Various debugging routines for the developer.                       
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2017-03-20  DHF     Added Time class.                               
///                                                                             
///@version 2014-02-25  DHF     Added proper comment header.                    
///                                                                             
//------------------------------------------------------------------------------

#include "dev_debug.h"

#if IS_LINUX

#include <stdio.h>
#include <time.h>

#ifdef IS_LINUX

#include "lib_string.h"

#include <cxxabi.h>         // abi::__cxa_demangle          
#include <execinfo.h>       // backtrace, backtrace_symbols 
#include <signal.h>         // signal + SIGxxx              
#include <stdio.h>          // fopen, fprintf, fclose       
#include <stdlib.h>         // free                         

#include "debug.h"

#endif  // #ifdef IS_LINUX

namespace dev {
namespace debug {


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Time::Time(const std::string& name)
    : m_Paused(true)
    , m_DisplayOnExit(true)
    , m_DeltaCPU(0.0)
    , m_LastCPU(currentCPU())
    , m_DeltaWall(0.0)
    , m_LastWall(lib::time::work::DateTime::now())
    , m_Name(name)
{
} // Time::Time(const std::string& name, bool start) //


//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
Time::~Time()
{
    if (m_DisplayOnExit)
    {
        printf(
            "%-50s %10.5lf (cpu);  %10.5lf (wall)\n"
          , m_Name.c_str()
          , (double) deltaCPU()
          , (double) deltaWall()
        );
    }

} // Time::~Time


//------------------------------------------------------------------------------
///@brief   Return the current amount of CPU consumed as a delta-time.          
//------------------------------------------------------------------------------
lib::time::work::DeltaTime  Time::currentCPU()
{

    lib::time::work::DeltaTime result;

    #ifdef _WIN32
        FILETIME creationTime;
        FILETIME exitTime;

        union {
            FILETIME    filetime;
            uint64_t    hundredsOfNanos;
        } kernel, user;

        GetThreadTimes(
            m_Handle
          , &creationTime
          , &exitTime
          , &kernel.filetime
          , &user.filetime
        );

        //----------------------------------------------------------------------
        //  A uint64_t can hold over a 100 years worth of time this way ...     
        //  if a thread is running that long, I won't care.  Honest.            
        //----------------------------------------------------------------------
        uint64_t x = kernel.hundredsOfNanos + user.hundredsOfNanos;
//        result.m_TotalCPU.tv_nsec = (x * 100) % 1000000000;                   
//        result.m_TotalCPU.tv_sec = x / 10000000;                              
        result.tv_nsec = (x * 100) % 1000000000;
        result.tv_sec = x / 10000000;

    #else
        clockid_t cid;
        pthread_getcpuclockid(pthread_self(), &cid);
        clock_gettime(cid, &result);

    #endif

    return result;

}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Time::start()
{
    if (m_Paused)
    {
        m_LastCPU  = currentCPU();
        m_LastWall = lib::time::work::DateTime::now();

        m_Paused   = false;
    }
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Time::pause()
{
    if (!m_Paused)
    {
        m_DeltaCPU  += (currentCPU() - m_LastCPU);
        m_DeltaWall += (lib::time::work::DateTime::now() - m_LastWall);
        m_Paused = true;
    }
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Time::reset( bool start)
{
    pause();
    if (start) Time::start();
    m_DeltaCPU.tv_sec  = m_DeltaCPU.tv_nsec  = 0;
    m_DeltaWall.tv_sec = m_DeltaWall.tv_nsec = 0;
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
bool Time::isRunning() const
{
    return !m_Paused;
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
bool Time::isPaused() const
{
    return m_Paused;
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
lib::time::work::DeltaTime Time::deltaCPU () const
{
    lib::time::work::DeltaTime result (m_DeltaCPU);

    if (!m_Paused) result += (currentCPU() - m_LastCPU);

    return result;
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
lib::time::work::DeltaTime Time::deltaWall() const
{
    lib::time::work::DeltaTime result(m_DeltaWall);

    if (!m_Paused) result  += (lib::time::work::DateTime::now()- m_LastWall);

    return result;
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Time::setName(const std::string& name)
{
    m_Name = name;
}


//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Time::setDisplayOnExit(bool doe)
{
    m_DisplayOnExit = doe;
} 


#ifdef IS_LINUX

static int      g_Argc(0);
static char**   g_Argv(NULL);

//------------------------------------------------------------------------------
//  Function headers for our semi-private functions.                            
//------------------------------------------------------------------------------
std::string demangle(const std::string& symbol);
std::string calling_routine(int level = 0);
std::vector<std::string> call_stack();
std::string call_stack_line();

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string signame(int signum)
{
    switch (signum)
    {
        case SIGHUP:   return "Hangup (POSIX).";
        case SIGINT:   return "Interrupt (ANSI).";
        case SIGQUIT:  return "Quit (POSIX).";
        case SIGILL:   return "Illegal instruction (ANSI).";
        case SIGTRAP:  return "Trace trap (POSIX).";
        case SIGABRT:  return "Abort (ANSI).";
        case SIGBUS:   return "BUS error (4.2 BSD).";
        case SIGFPE:   return "Floating-point exception (ANSI).";
        case SIGKILL:  return "Kill, unblockable (POSIX).";
        case SIGUSR1:  return "User-defined signal 1 (POSIX).";
        case SIGSEGV:  return "Segmentation violation (ANSI).";
        case SIGUSR2:  return "User-defined signal 2 (POSIX).";
        case SIGPIPE:  return "Broken pipe (POSIX).";
        case SIGALRM:  return "Alarm clock (POSIX).";
        case SIGTERM:  return "Termination (ANSI).";
        case SIGSTKFLT:return "Stack fault.";
        case SIGCHLD:  return "Child status has changed (POSIX).";
        case SIGCONT:  return "Continue (POSIX).";
        case SIGSTOP:  return "Stop, unblockable (POSIX).";
        case SIGTSTP:  return "Keyboard stop (POSIX).";
        case SIGTTIN:  return "Background read from tty (POSIX).";
        case SIGTTOU:  return "Background write to tty (POSIX).";
        case SIGURG:   return "Urgent condition on socket (4.2 BSD).";
        case SIGXCPU:  return "CPU limit exceeded (4.2 BSD).";
        case SIGXFSZ:  return "File size limit exceeded (4.2 BSD).";
        case SIGVTALRM:return "Virtual alarm clock (4.2 BSD).";
        case SIGPROF:  return "Profiling alarm clock (4.2 BSD).";
        case SIGWINCH: return "Window size change (4.3 BSD, Sun).";
        case SIGIO:    return "I/O now possible (4.2 BSD).";
    }
    return "unknown signal";
    
}


//------------------------------------------------------------------------------
///@brief   Return the contents of the call stack (starting with what called    
///         call_stack.                                                         
//------------------------------------------------------------------------------
std::vector<std::string> call_stack()
{
    void* addresses[1024];
    const int addresses_count(sizeof(addresses) / sizeof(addresses[0]));

    int symbol_size = backtrace(addresses, addresses_count);
    char** symbols  = backtrace_symbols(addresses, symbol_size);

    //--------------------------------------------------------------------------
    //  Note that we start for (int i = 1) in order to skip over the name of    
    //  this routine (call_stack()).                                            
    //--------------------------------------------------------------------------
    std::vector<std::string> result;
    for (int i=1; i < symbol_size; ++i) {
        std::string deman(demangle(symbols[i]));
        if (deman != "") result.push_back(deman);
    }
    free(symbols);
    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the call_stack() in a manner that can be displayed as one    
///         line of text.                                                       
//------------------------------------------------------------------------------
std::string call_stack_line()
{
    std::vector<std::string> cs(call_stack());

    std::string result;
    std::string separator;

    int s = cs.size();

    while(--s) {
        result = result + separator + cs[s];
        separator = " => ";
    }
    return result;
}




//------------------------------------------------------------------------------
///@brief   Handle a signal (usually a catestrophic event).                     
//------------------------------------------------------------------------------
void signal_callback_handler(int signum)
{
    FILE* fp = fopen("crash-stack.txt", "w");
    fprintf(fp, "image: %s\n\n", g_Argv ? g_Argv[0] : "-unknown-");
    fprintf(fp, "error: %2d  (%s)\n\n", signum, signame(signum).c_str());

    std::vector<std::string> cs(call_stack());
    std::vector<std::string>::iterator c;
    for (c = cs.begin(); c != cs.end(); ++c) {
        fprintf(fp, "    %s\n", c->c_str());
    }

    fclose(fp);

    fprintf(stderr, "------------------------------------------------------\n");
    fprintf(stderr, "crash-stack.txt generated.  Use tools/crash to analyze\n");
    fprintf(stderr, "------------------------------------------------------\n");

    exit(signum);
}

//------------------------------------------------------------------------------
///@brief   Initialize the error handling.                                      
//------------------------------------------------------------------------------
void initialize(int argc, char** argv)
{
    g_Argc = argc;
    g_Argv = argv;

    signal(SIGINT,      signal_callback_handler);
    signal(SIGQUIT,     signal_callback_handler);
    signal(SIGILL,      signal_callback_handler);
    signal(SIGABRT,     signal_callback_handler);
    signal(SIGTRAP,     signal_callback_handler);
    signal(SIGFPE,      signal_callback_handler);
    signal(SIGKILL,     signal_callback_handler);
    signal(SIGSEGV,     signal_callback_handler);
    signal(SIGPIPE,     signal_callback_handler);
    signal(SIGALRM,     signal_callback_handler);
    signal(SIGTERM,     signal_callback_handler);
    signal(SIGSTKFLT,   signal_callback_handler);
    signal(SIGXFSZ,     signal_callback_handler);

}


//------------------------------------------------------------------------------
///@brief   Given a C++ symbol name in compiler mangled format, demangle it.    
//------------------------------------------------------------------------------
std::string demangle(const std::string& symbol)
{
    std::string sym(symbol);
    std::string bkt;

    if (sym.find("[") != std::string::npos) bkt = sym.substr(sym.find("["));
    if (sym.find("(") != std::string::npos) sym = sym.substr(sym.find("(")+1);
    if (sym.find(")") != std::string::npos) sym = sym.substr(0, sym.find(")"));
    if (sym.find("+") != std::string::npos) sym = sym.substr(0, sym.find("+"));

    char demangled[2048];
    int status;
    size_t length(sizeof(demangled));
    abi::__cxa_demangle(sym.c_str(), demangled, &length, &status);
    return lib::format("%-40s %s", status ? sym.c_str() : demangled, bkt.c_str());
    return status ? sym : std::string(demangled);
}


//------------------------------------------------------------------------------
///@brief   Return the [demangled] name of the calling routine.                 
///@param level     The number of routines back to return:                      
///                     -2  Returns "calling_routine(int)"                      
///                     -1  The name of the routine that called calling_routine.
///                      0  The name of the routine that called the routine that
///                         called calling_routine (this is what you want,      
///                         honest).                                            
///                      1  The name of the routine that called 0.              
///                      2  The name of the routine that called 1.              
///                      3  The name of the routine that called 2.              
///                     etc.                                                    
//------------------------------------------------------------------------------
std::string calling_routine(int level)
{
    void* addresses[100];
    const int addresses_count(sizeof(addresses) / sizeof(addresses[0]));

    std::string result;
    if (level+2 < addresses_count && level >= -2) {
        int symbol_size = backtrace(addresses, addresses_count);
        char** symbols = backtrace_symbols(addresses, symbol_size);

        result = demangle(symbols[level + 2]);

        free(symbols);
    }

    return result;
}

#else

//------------------------------------------------------------------------------
//  Windows version.                                                            
//------------------------------------------------------------------------------
void initialize(int argc, char** argv) { }

#endif  // #ifdef IS_LINUX

} // namespace debug    
} // namespace dev      



#endif
