//------------------------------------------------------------------------------
///@file lib_mp_work_thread.cpp                                                 
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@par Thread Safety:  unknown none class object                               
///                                                                             
///@version 2020-09-23  JRS     switched from boost::thread to std::thread      
///                                                                             
///@version 2020-09-21  JRS     Replaced boost::mutexed_singleton               
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-11-20  DHF     File creation                                   
//------------------------------------------------------------------------------

#include "lib_mp_work_thread.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace lib {
namespace mp {
namespace work {

// static data members from RegisteredThreads
std::mutex RegisteredThreads::m_mutex;
RegisteredThreads* RegisteredThreads::m_instance = nullptr;


//------------------------------------------------------------------------------
///@brief   Return the native handle of the calling thread.                     
//------------------------------------------------------------------------------
std::thread::native_handle_type Thread::self()
{
    #ifdef _WIN32
//    return boost::thread::native_handle_type(GetCurrentThreadId());           
    HANDLE result;
    DuplicateHandle(
        GetCurrentProcess()
      , GetCurrentThread()
      , GetCurrentProcess()
      , &result
      , 0
      , TRUE
      , DUPLICATE_SAME_ACCESS
    );
    return result;
//    return GetCurrentThread()                                                 
    #else
    return pthread_self();
    #endif
}

} // namespace work //
} // namespace mp //
} // namespace lib //

