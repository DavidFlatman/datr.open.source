//------------------------------------------------------------------------------
///@file lib_mp_work_threadinfo.h                                               
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MP_THREADINFO_H_FILE_GUARD
#define LIB_MP_THREADINFO_H_FILE_GUARD

#include "lib_log_work_message.h"
#include "lib_work_namedobject.h"
#include "lib_time_work_datetime.h"
#include "lib_time_work_deltatime.h"
#include <thread>
#include <mutex>
#include <deque>

namespace lib {
namespace mp {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: ThreadInfo                                                       
///                                                                             
///@brief   Provide information about a thread.                                 
///                                                                             
///@par Thread Safety:  object                                                  
///                                                                             
///@version 2020-09-23  JRS     replace boost references with std references.   
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2010-04-07  DHF     File creation                                   
//------------------------------------------------------------------------------
class ThreadInfo : public lib::work::NamedObject
{
    public:
        ThreadInfo(const std::string& name = "");
        ThreadInfo(std::thread::native_handle_type handle, const std::string& name = "");
        virtual ~ThreadInfo();

        lib::time::work::DeltaTime cpuTime() const;
        lib::time::work::DeltaTime runTime() const;
        double cpuPercentCurrent() const;
        double cpuPercentTotal() const;
        const std::deque<double>& historyOfCPU() const;

        double updateHistory();

        void setRunning(bool is_running);
        bool isRunning() const;

        //----------------------------------------------------------------------
        //  Override the NamedObject methods to make them thread safe.          
        //----------------------------------------------------------------------
        void setName(const std::string& name);
        std::string name() const;

        void setHandle(std::thread::native_handle_type handle)
        {
            m_Handle = handle;
        }

        static const lib::log::ds::messageid_t  MSG_THREAD_INFO;
        lib::log::work::ConstMessagePtr stats() const;

    protected:

    private:
        std::thread::native_handle_type   m_Handle;
        lib::time::work::DateTime           m_StartTime;
        mutable lib::time::work::DeltaTime  m_PercentLastCPU;
        mutable lib::time::work::DateTime   m_PercentLastUpdateTime;
        std::deque<double>                  m_HistoryOfCPU;
        lib::time::work::DeltaTime          m_CPU;
        lib::time::work::DeltaTime          m_RunTime;
        bool                                m_IsRunning;

        mutable std::mutex                m_ThreadMutex;

        double cpuPercentCurrentNoLock() const;
        lib::time::work::DeltaTime runningCPUTime() const;
        lib::time::work::DeltaTime runningRunTime() const;

        //----------------------------------------------------------------------
        //  These would probably be painful to implement correctly.             
        //----------------------------------------------------------------------
        ThreadInfo(std::thread::native_handle_type handle);
        ThreadInfo& operator=(const ThreadInfo& that);
}; // class ThreadInfo //

} // namespace work 
} // namespace mp
} // namespace lib

#endif // #ifndef LIB_MP_THREADINFO_H_FILE_GUARD

