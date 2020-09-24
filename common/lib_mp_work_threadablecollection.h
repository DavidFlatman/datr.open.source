//------------------------------------------------------------------------------
///@file lib_mp_work_threadablecollection.h                                     
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_MP_THREADABLECOLLECTION_H_FILE_GUARD
#define LIB_MP_THREADABLECOLLECTION_H_FILE_GUARD

#include "lib_cast.h"                   // lib::cast_dynamic
#include "lib_ds_shared_ptr.h"
#include "lib_log_work_message.h"
#include "lib_mp_work_threadable.h"
#include "lib_msg_publisher.h"

#include <vector>


namespace lib {
namespace mp {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: ThreadableCollection                                             
///                                                                             
///@brief   Allow control (join, start) of multiple threads at one time.        
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Usage Note                                                              
///         The threadable collection can publish lib::log::ConstMessagePtr's   
///         that represent status / debug information.  This can safely be      
///         ignored.                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                             Changed typedef to using                        
///                                                                             
///@version 2017-07-12  DHF     Allow the person to push_back another           
///                             ThreadableCollection..                          
///                                                                             
///@version 2015-09-28  DHF     Made the start and join methods virtual.        
///                                                                             
///@version 2011-03-31  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class ThreadableCollection 
    : public std::vector<ThreadablePtr>
    , public lib::msg::Publisher<lib::log::work::Message>
{
    public:
        ThreadableCollection();
        virtual ~ThreadableCollection();

        //----------------------------------------------------------------------
        ///@brief   Add an object to the ThreadableCollection.                  
        ///                                                                     
        ///@param   x       A pointer to the object to be added to the          
        ///                 collection.  The pointer can be a raw pointer to a  
        ///                 class derived from Threadable or a boost::shared_ptr
        ///                 pointing to an object derived from Threadable.      
        ///                                                                     
        ///@note    push_back takes shared ownership of the object.  If a raw   
        ///         pointer is passed, ThreadableCollection owns the lifecycle  
        ///         of the object.                                              
        //----------------------------------------------------------------------
        template <typename TYPE>
        TYPE* push_back(TYPE* x)
        {
            ThreadablePtr tp(x);
            std::vector<ThreadablePtr>::push_back(tp);

            ThreadableCollection* p;
            lib::cast_dynamic(p, x);
            if (p != NULL)
            {
                for (iterator i = p->begin(); i != p->end(); ++i)
                {
                    std::vector<ThreadablePtr>::push_back(*i);
                }
            }
            return x;
        }

        template <typename TYPE>
        TYPE push_back(TYPE x)
        {
            ThreadablePtr tp(x);
            std::vector<ThreadablePtr>::push_back(tp);

            ThreadableCollection* p;
            lib::cast_dynamic(p, x.get());
            if (p != NULL)
            {
                for (iterator i = p->begin(); i != p->end(); ++i)
                {
                    std::vector<ThreadablePtr>::push_back(*i);
                }
            }
            return x;
            
        }

        bool isStarted() const;

        virtual void startAll(bool checkAllIsWell = true);
        virtual void joinAll();
        virtual void joinAllDebug();

        static const lib::log::ds::messageid_t  MSG_THREAD_COMPLETED;
        static const lib::log::ds::messageid_t  MSG_THREADS_STARTED;
        static const lib::log::ds::messageid_t  MSG_THREADS_COMPLETED;

    protected:

    private:
        //----------------------------------------------------------------------
        //  If you want these, you're doing something wrong.                    
        //----------------------------------------------------------------------
        ThreadableCollection(const ThreadableCollection& that);
        ThreadableCollection& operator=(const ThreadableCollection& that);

        bool m_IsStarted;



}; // class ThreadableCollection //
using ThreadableCollectionPtr = lib::ds::shared_ptr<ThreadableCollection>;

} // namespace work 
} // namespace mp
} // namespace lib

#endif // #ifndef LIB_MP_THREADABLECOLLECTION_H_FILE_GUARD

