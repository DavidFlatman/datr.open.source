//------------------------------------------------------------------------------
///@file lib_mp_work_threadable.h                                               
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MP_THREADABLE_H
#define LIB_MP_THREADABLE_H

#include "lib_ds_shared_ptr.h"
#include "lib_mp_work_thread.h"
#include "lib_work_namedobject.h"

#include <string>
#include <stdint.h>

namespace lib {
namespace mp {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Threadable                                                       
///                                                                             
///@brief   Pure virtual class indicating that the class is threadable.         
///                                                                             
///@par Purpose:                                                                
///         This class should be used as a base class for all classes/objects   
///         that will be added to a ThreadableCollection.                       
///                                                                             
///@par Thread Safety:  object                                                  
///         But then again, this class doesn't do much.                         
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             class MyClass : public lib::mp::work::Threadable                
///             {                                                               
///                 public:                                                     
///                     MyClass()                                               
///                         : lib::mp::work::Threadable("MyClass")              
///                         , m_Initialized(false)                              
///                     { }                                                     
///                     void operator()()                                       
///                     {                                                       
///                         for (int i=0; i < m_Max; ++i) {                     
///                             computeDigitOfPi();                             
///                         }                                                   
///                     }                                                       
///                     void checkIfAllIsWell()                                 
///                     {                                                       
///                         if (!m_Initialized) {                               
///                             throw std::logic_error("not initialized");      
///                         }                                                   
///                         return true;                                        
///                     }                                                       
///                     void initialize(int max)                                
///                     {                                                       
///                         m_Max = max;                                        
///                         m_Initialized = true;                               
///                     }                                                       
///                 private:                                                    
///                     int m_Max;                                              
///                     bool m_Initialized;                                     
///             };                                                              
///         @endcode                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                             Changed typedef to using                        
///                                                                             
///@version 2013-10-08  DHF     Added beforeThreadStarts                        
///                             Added afterJoin                                 
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-10-27  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Threadable : virtual public lib::work::NamedObject
{
    public:
        Threadable() { }
        Threadable(const std::string& name) { setName(name);}
        virtual ~Threadable() {}

        virtual void operator()() = 0;

        void setThread(lib::ds::shared_ptr<Thread> t) {m_Thread = t;}
        ThreadPtr thread() { return m_Thread; }

        //----------------------------------------------------------------------
        ///@brief   Give the derived class an opportunity to indicate that      
        ///         there is a problem (most likely incomplete initialization). 
        ///                                                                     
        ///@note 1  It is envisioned that derived class will throw an           
        ///         exception if there is a problem (merely returning bool      
        ///         would provide insufficient information).  The types of      
        ///         exception will imply std::logic_error -- the user of the    
        ///         derived class did something wrong.                          
        ///                                                                     
        ///@note 2  It is expected that the program (or at least the chain of   
        ///         pub-subs) is going to die as a direct result of any         
        ///         exceptions thrown.                                          
        ///                                                                     
        ///@note 3  Note that the method is "const".  Calling this method is    
        ///         simply a status check and should not alter the non-mutable  
        ///         state of the object.  This method may be called multiple    
        ///         times.                                                      
        ///                                                                     
        ///@note 4  The difference between checkIfAllIsWell and                 
        ///         beforeThreadStarts is that it is envisioned that            
        ///         beforeThreadStarts will be explicitly called by             
        ///         ThreadableCollection, um, before the thread starts 
        ///         -- giving the object a last chance to do something in the   
        ///         main thread's context before going into the thread's        
        ///         context.                                                    
        ///\n\n                                                                 
        ///         The checkIfAllIsWell() method is effectively a status check 
        ///         allowing the derived object a chance to alter the process   
        ///         flow via exceptions.                                        
        ///                                                                     
        //----------------------------------------------------------------------
        virtual void checkIfAllIsWell() const { }

        //----------------------------------------------------------------------
        ///@brief   Give the derived class an opportunity to execute code just  
        ///         before the thread starts (ensure proper initialization?).   
        ///@note    If designer of the derived class wants beforeThreadStarts   
        ///         to throw an exception, they should override "allsWell"      
        ///         above.                                                      
        ///@return  true    All is good, go ahead and create the thread.        
        ///         false   Do not start a thread for this object.              
        //----------------------------------------------------------------------
        virtual bool beforeThreadStarts() { return true; }

        //----------------------------------------------------------------------
        ///@brief   Give the derived class an opportunity to execute code after 
        ///         its Thread has been joined (ThreadableCollection honots this
        ///         method).                                                    
        ///@note    This method will be called by ThreadableCollection after the
        ///         thread has been joined, but there may be a [lengthy] time   
        ///         gap between the thread terminating and the method being     
        ///         called.                                                     
        //----------------------------------------------------------------------
        virtual void afterJoin() { }

    private:
        Threadable& operator=(const Threadable& that);

        ThreadPtr  m_Thread;

}; // class Threadable //

using ThreadablePtr = lib::ds::shared_ptr<Threadable>;

} // namespace work //
} // namespace mp //
} // namespace lib //

#endif // #ifndef LIB_MP_THREADABLE_H //
