//------------------------------------------------------------------------------
///@file lib_mp_work_thread.h                                                   
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MP_THREAD_H
#define LIB_MP_THREAD_H

#include <boost/thread.hpp>
#include <set>
#include <string>
#include <mutex>

#include "lib_ds_shared_ptr.h"
#include "lib_mp_work_threadinfo.h"

namespace lib {
namespace mp {
namespace work {

//------------------------------------------------------------------------------
///@brief   Provide access to the registered threads.
///@note    Extremely simplified and stripped-down singleton implementation.
///         Use RegisteredThreads::lock as a scoped lock.
/// 
///         e.g.
///         {
///             RegisteredThreads::lock lock;
///             RegisteredThreads::instance()->do_something(...);
///         }
///@note    Requires manual destruction via RegisteredThreads::destroy()
///@note    instance() is not thread-locked! a separate lock instance is
///         required to control the usage of the singleton to allow multiple
///         operations on the singleton without multiple lock requirements.
///         This behavior is different than using boost::mutexed_singleton in
///         that using boost::mutexed_singleton::instance automatically does
///         a lock at every reference.
///@version 2020-09-21  JRS     Replaced boost::mutexed_singleton because it
///         wasn't compiling under all platforms
///
//------------------------------------------------------------------------------
class RegisteredThreads
    : public std::set<ThreadInfo*>
{
    public:
        /// @brief get a scoped lock on the singleton
        struct lock
        {
            std::lock_guard<std::mutex> m_lock;
            lock()
                : m_lock(RegisteredThreads::m_mutex)
            {
            }
        };

        /// @brief get access to the single instance in the system
        /// @note is thread safe, but it's wise to check the return value.
        /// @not requires C++ 11 magic statics and uses lambdas.
        /// @return single instance in the system
        static RegisteredThreads* instance()
        {
            static bool static_init = []()->bool {
                m_instance = new RegisteredThreads;
                return true;
            }();
            return m_instance;
        }

        /// @brief destroy the single instance in the system
        /// @note once it's dead, it's dead and won't come back to life.
        static void destroy()
        {
            lock llock;
            delete m_instance;
            m_instance = nullptr;
        }

        RegisteredThreads() = default;

    private:
        // make the class noncopyable by deleting the assign and copy
        RegisteredThreads(const RegisteredThreads&) = delete;
        RegisteredThreads& operator=(const RegisteredThreads&) = delete;

        ~RegisteredThreads()
        {
            // the process was responsible for cleaning up.
        }

        static std::mutex m_mutex;
        static RegisteredThreads* m_instance;
};


//------------------------------------------------------------------------------
///                                                                             
///@par Class: Thread                                                           
///                                                                             
///@brief                                                                       
///                                                                             
///@par Purpose:                                                                
///                                                                             
///@par Thread Safety:  object                                                  
///                                                                             
///@par Why Thread Factory?                                                     
///         We had several goals for threading:                                 
///             -#  reusable threads (this goal is a back burner at this point) 
///             -#  running threads                                             
///             -#  thread info (name, status, cpu, ...)                        
///             -#  thread collections (startAll, joinAll, killAll, ...)        
///\n\n                                                                         
///         We seriously considered the boost threading model: instant          
///         portablity.  We probably would have pushed forward with it if       
///         they had made their destructor virtual ... or maybe even their      
///         start_thread virtual.  But no.                                      
///\n\n                                                                         
///         We seriously considered QThread (Qt framework), but we didn't want  
///         to force the Qt framework on all target platforms.                  
///                                                                             
///@par Philosophy of Operations                                                
///         There are [at least] two major philosophies for thread classes.     
///             -#  The boost approach is to have the constructor to kick off   
///                 the thread and the class/object being threaded is agnostic  
///                 to the fact the thread exists.                              
///             -#  The Qt framework approach, the class to run in a thread     
///                 derives from QThread and implements the pure virtual        
///                 run() method that acts as the thread main.  The class to    
///                 be threaded if fully cognizant of the fact that it is       
///                 running in a thread.                                        
///                                                                             
///         Both approaches have their advantages.                              
///                                                                             
///@par Extending This Class                                                    
///         If you need additional constructors for this class, see             
///         boost/thread/detail/thread.hpp for the form that they should        
///         probably take.  None of our classes (at this time) require          
///         parameters passed to the thread main.                               
///                                                                             
///@par Sending Parameters to Thread Main                                       
///         Because of the way the underlying Boost threads pass parameters to  
///         the start function, types passed must have a copy constructor -- and
///         that copy constructor is going to be called @e ten (10) times!      
///         @code                                                               
///             class MyType : public std::string {                             
///                 public:                                                     
///                     MyType(const std::string& s) : std::string(s) {}        
///                     MyType(const MyType& mt) : std::string(mt) {}           
///                     ~MyType() {}                                            
///             };                                                              
///                                                                             
///             void wrongWayToDoIt(const MyType& mt) {                         
///                 std::cout << "my thread main: " << mt << std::endl;         
///             }                                                               
///                                                                             
///             int main(int argc, char* argv[]) {                              
///                 MyType mt("some string");                                   
///                 Thread t("thread-name", wrongWayToDoIt, mt);                
///                 t.join();                                                   
///             }                                                               
///         @endcode                                                            
///\n\n                                                                         
///         Note that even though the main routine for the thread is expecting  
///         a <i>const MyType& mt</i>, the copy constructor is still called 10  
///         times.                                                              
///\n\n                                                                         
///         It is better to pass via pointer or const pointer.                  
///         @code                                                               
///             class MyType : public std::string {                             
///                 public:                                                     
///                     MyType(const std::string& s) : std::string(s) {}        
///                     MyType(const MyType& mt) : std::string(mt) {}           
///                     ~MyType() {}                                            
///             };                                                              
///                                                                             
///             void rightWayToDoIt(const MyType* mt) {                         
///                 std::cout << "my thread main: " << *mt << std::endl;        
///             }                                                               
///                                                                             
///             int main(int argc, char* argv[]) {                              
///                 MyType mt("some string");                                   
///                 Thread t("thread-name", rightWayToDoIt, &mt);               
///                 t.join();                                                   
///             }                                                               
///         @endcode                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Changed typedef to using                        
///                             Replaced #include <boost/shared_ptr.hpp>        
///                                                                             
///@version 2010-06-24  DHF         Made to work with Windows -- evidently, in  
///                                 their quest for godhood, Microsoft defines  
///                                 THIS as a macro.                            
///@version 2010-04-08  DHF         Abstract out the "thread information" into  
///                                 its own class.                              
///                                 Made template parameter names look more     
///                                 like boost.                                 
///                                                                             
///@version 2010-01-26  DHF         Changed outer namespace from tools to lib.  
///                                                                             
///@version 2009-11-20  DHF         File creation                               
//------------------------------------------------------------------------------
class Thread;

class Thread 
    : public ThreadInfo
    , public boost::thread
{
    public:
        //----------------------------------------------------------------------
        ///@brief   Create (and start) a thread executing function f.           
        ///@param   name    The name of the thread; used to identify the thread 
        ///                 when doing performance forensics.                   
        ///@param   f       The function to be executed.                        
        //----------------------------------------------------------------------
        template<typename F>
        Thread(const std::string& name, F* f)
            : ThreadInfo(name)
            , boost::thread(trackThread<F>, this, f)
        {
        }

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        template<typename F, typename A1>
        Thread(const std::string& name, F* f, A1 a1)
            : ThreadInfo(name)
            , boost::thread(trackThread<F, A1>, this, f, a1)
        {
        }

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        template<typename F, typename A1, typename A2>
        Thread(const std::string& name, F* f, A1 a1, A2 a2)
            : ThreadInfo(name)
            , boost::thread(trackThread<F, A1, A2>, this, f, a1, a2)
        {
        }

        //----------------------------------------------------------------------
        ///@brief   Free up resources; remove thread from registered threads.   
        //----------------------------------------------------------------------
        virtual ~Thread()
        {
            RegisteredThreads::lock lock;
            RegisteredThreads::instance()->erase(this);
        }

        //======================================================================
        //  static methods                                                      
        //======================================================================
        static boost::thread::native_handle_type self();

    protected:

    private:
        //----------------------------------------------------------------------
        ///@brief   Handle the thread tracking overhead for when the thread     
        ///         starts.                                                     
        //----------------------------------------------------------------------
        inline static void initialize(Thread* thread) 
        {
            thread->setHandle(self());
            thread->setRunning(true);

            RegisteredThreads::lock lock;
            RegisteredThreads::instance()->insert(thread);
        }

        //----------------------------------------------------------------------
        ///@brief   Handle the thread tracking overhead when the thread         
        ///         terminates.                                                 
        //----------------------------------------------------------------------
        inline static void finalize(Thread* thread)
        {
            thread->setRunning(false);
        }

        //----------------------------------------------------------------------
        ///@brief   Thread "main" for registering the thread when it first      
        ///         starts and unregistering it when the object is destroyed.   
        //----------------------------------------------------------------------
        template<typename F> 
        static void trackThread(Thread* thread, F* f)
        {
            initialize(thread);
            try {
                (*f)();
            } catch(...) {
                finalize(thread);
                throw;
            }
            finalize(thread);
        }

        template<typename F, typename A1> 
        static void trackThread(Thread* thread, F* f, A1 a1)
        {
            initialize(thread);
            try {
                (*f)(a1);
            } catch(...) {
                finalize(thread);
                throw;
            }
            finalize(thread);
        } 

        template<typename F, typename A1, typename A2> 
        static void trackThread(Thread* thread, F* f, A1 a1, A2 a2)
        {
            initialize(thread);
            try {
                (*f)(a1, a2);
            } catch(...) {
                finalize(thread);
                throw;
            }
            finalize(thread);
        } 

}; // class Thread //                                                           
using ThreadPtr = lib::ds::shared_ptr<Thread>;

} // namespace work //                                                          
} // namespace mp //                                                            
} // namespace lib //                                                           

#endif // #ifndef LIB_MP_THREAD_H //                                            

