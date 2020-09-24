//------------------------------------------------------------------------------
///                                                                             
///@file lib_mp_work_threadinfo.cpp                                             
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Provide information about a thread.                                 
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-09-23  JRS     replace boost references with std references.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-08-02  DHF     DeltaTime::seconds renamed inSeconds.           
///                                                                             
///@version 2014-10-02  DHF     lib::DateTime moved to lib::time::work::DateTime
///                                                                             
///@version 2014-04-01  DHF     Corrected spelling of sconds to seconds.        
///                                                                             
///@version 2010-04-07  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_mp_work_threadinfo.h"
#include "lib_log_work_messagefactory.h"
#include "lib_string.h"

#ifdef _WIN32
#include <Windows.h>
#endif


namespace lib {
namespace mp {
namespace work {

const lib::log::ds::messageid_t   ThreadInfo::MSG_THREAD_INFO            (0x01);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const size_t s_MaxHistorySize(100);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ThreadInfo::ThreadInfo(const std::string& name)
    : lib::work::NamedObject(name)
    , m_IsRunning(false)
{

} // ThreadInfo::ThreadInfo() //

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
ThreadInfo::ThreadInfo(
    std::thread::native_handle_type handle
  , const std::string& name
) : lib::work::NamedObject(name)
  , m_Handle(handle)
  , m_StartTime(lib::time::work::DateTime::now())
  , m_IsRunning(true)
{

} // ThreadInfo::ThreadInfo(std::thread::native_handle_type handle) //

//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
ThreadInfo::~ThreadInfo()
= default; // ThreadInfo::~ThreadInfo

//------------------------------------------------------------------------------
///@brief   Return the name of this thread.                                     
//------------------------------------------------------------------------------
std::string ThreadInfo::name() const
{

    std::lock_guard<std::mutex>   lock(m_ThreadMutex);
    return lib::work::NamedObject::name();

} // std::string ThreadInfo::name() const //

//------------------------------------------------------------------------------
///@brief   Change the name of the thread.                                      
//------------------------------------------------------------------------------
void ThreadInfo::setName(const std::string& name)
{

    std::lock_guard<std::mutex> lock(m_ThreadMutex);

    lib::work::NamedObject::setName(name);

}

//------------------------------------------------------------------------------
///@brief   Return the total amount of CPU that the thread has consumed.        
//------------------------------------------------------------------------------
lib::time::work::DeltaTime ThreadInfo::cpuTime() const
{

    return isRunning() ? runningCPUTime() : m_CPU;
} // lib::time::work::DeltaTime ThreadInfo::cpuTime() const //

//------------------------------------------------------------------------------
///@brief   Return the wall (aka real aka clock) time that the thread has been  
///         running.                                                            
///@note    When the thread stops, the wall runTime also stops.                 
//------------------------------------------------------------------------------
lib::time::work::DeltaTime ThreadInfo::runTime() const
{

    return isRunning() ? runningRunTime() : m_RunTime;
} // lib::time::work::DeltaTime ThreadInfo::runTime() const //

//------------------------------------------------------------------------------
///@brief   Return the percent of CPU time that the thread is consuming since   
///         the last time that the method was called.                           
//------------------------------------------------------------------------------
double ThreadInfo::cpuPercentCurrent() const
{

    std::lock_guard<std::mutex> lock(m_ThreadMutex);

    return cpuPercentCurrentNoLock();
} // double ThreadInfo::cpuPercentCurrent() const //

//------------------------------------------------------------------------------
///@brief   Return the percent of CPU time that the thread is consuming since   
///         the last time that the percent was gathered ... but don't lock the  
///         internals.                                                          
//------------------------------------------------------------------------------
double ThreadInfo::cpuPercentCurrentNoLock() const
{

    lib::time::work::DeltaTime currentCPU = cpuTime();
    lib::time::work::DateTime now(lib::time::work::DateTime::now());

    double deltaCPU = (double)(currentCPU - m_PercentLastCPU);
    m_PercentLastCPU = currentCPU;

    double deltaTime;
    if ((double) m_PercentLastUpdateTime == 0.0) {
        deltaTime = (double)(now - m_StartTime);
    } else {
        deltaTime = (double)(now - m_PercentLastUpdateTime);
    }
    m_PercentLastUpdateTime = now;

    double result = (deltaTime == 0) ? 0.0 : deltaCPU / deltaTime * 100;

    return result;

} // double ThreadInfo::cpuPercentCurrentNoLock() const

//------------------------------------------------------------------------------
///@brief   Return the percent of CPU that the thread has consumed since it was 
///         started.                                                            
///@note    After the thread stops, the cpuPercentTotal value will not change.  
//------------------------------------------------------------------------------
double ThreadInfo::cpuPercentTotal() const
{

    double cpu = (double) cpuTime();
    double run = (double) runTime();

    return (run == 0) ? 0 : cpu / run;
} // double ThreadInfo::cpuPercentTotal() const //

//------------------------------------------------------------------------------
///@brief   Return the [current] history of CPU utilization.                    
//------------------------------------------------------------------------------
const std::deque<double>& ThreadInfo::historyOfCPU() const
{

    std::lock_guard<std::mutex> lock(m_ThreadMutex);

    return m_HistoryOfCPU;
} // const std::deque<double>& ThreadInfo::historyOfCPU() const //

//------------------------------------------------------------------------------
///@brief   Update the CPU utilization history by pushing the current CPU       
///         percent onto the back of the queue; removing front if needed.       
//------------------------------------------------------------------------------
double ThreadInfo::updateHistory()
{

    std::lock_guard<std::mutex> lock(m_ThreadMutex);

    if (m_HistoryOfCPU.size() == s_MaxHistorySize) {
        m_HistoryOfCPU.pop_front();
    }

    double result = cpuPercentCurrentNoLock();
    m_HistoryOfCPU.push_back(result);

    return result;
} // double ThreadInfo::updateHistory() //

//------------------------------------------------------------------------------
///@brief   Return the amount of CPU time that the thread has consumed since it 
///         was created.                                                        
///@warning This method assumes that the thread is still running.               
//------------------------------------------------------------------------------
lib::time::work::DeltaTime ThreadInfo::runningCPUTime() const
{

    lib::time::work::DeltaTime result;

    //--------------------------------------------------------------------------
    //  Yes, we are "throwing away" the const off of this ... but we are        
    //  honoring teh const.                                                     
    //--------------------------------------------------------------------------
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

        int status = pthread_getcpuclockid(m_Handle, &cid);

        assert(status == 0);

        clock_gettime(cid, &result);

    #endif

    return result;

} // DeltaTime   ThreadInfo::cpuTime() const

//------------------------------------------------------------------------------
///@brief   Return the wall (aka real aka clock) time that the thread has been  
///         running.                                                            
//------------------------------------------------------------------------------
lib::time::work::DeltaTime ThreadInfo::runningRunTime() const
{

    return lib::time::work::DateTime::now() - m_StartTime;
}

//------------------------------------------------------------------------------
///@brief   Change the state of the thread.                                     
///@warning This method is called *after* the thread is really started and      
///         *before* it is really stopped.                                      
//------------------------------------------------------------------------------
void ThreadInfo::setRunning(bool state)
{

    std::lock_guard<std::mutex> lock(m_ThreadMutex);

    if (state) {
        m_StartTime = lib::time::work::DateTime::now();
    } else {
        m_CPU = runningCPUTime();
        m_RunTime = runningRunTime();
    }
    m_IsRunning = state;

}

//------------------------------------------------------------------------------
///@brief   Return true if the thread is still running -- mostly.               
///@warning There is a small period of time after the thread starts before the  
///         isRunning return true ... and a small bit of time between isRunning 
///         becomes false before the thread is truly done.                      
//------------------------------------------------------------------------------
bool ThreadInfo::isRunning() const
{

    return m_IsRunning;
}

lib::log::work::ConstMessagePtr ThreadInfo::stats() const
{
    lib::log::work::MessageFactory message(lib::log::ds::LIB_MP_THREADINFO);
    return message.informational(
        MSG_THREAD_INFO
      , format(
            "%13.8lf CPU seconds in %13.8lf wall seconds (thread %s)"
          , cpuTime().inSeconds()
          , runTime().inSeconds()
          , name().c_str()
        )
    );
}

} // namespace work //
} // namespace mp //
} // namespace lib //

