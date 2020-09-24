//------------------------------------------------------------------------------
///@file lib_work_conversionlabbase.h                                           
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_WORK_CONVERSIONLABBASE_H_FILE_GUARD
#define LIB_WORK_CONVERSIONLABBASE_H_FILE_GUARD

#include <deque>
#include <stdlib.h>         // size_t
#include <string>

namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: ConversionLabBase                                                
///                                                                             
///@brief   Base class of for ConversionLab template.                           
///                                                                             
///         Type OUTPUT_TYPE_PTR will either be a boost::shared_ptr to a        
///         constant object or a boost::shared_ptr to a non-constant object.    
///         This template implements the code common between the two.           
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///         This class is only expected to be directly used by the ConversionLab
///         template.                                                           
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2018-10-30  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename OUTPUT_TYPE_PTR>
class ConversionLabBase
{
    public:
        virtual ~ConversionLabBase() = default;

        //----------------------------------------------------------------------
        ///@brief   Return the next output element from the conversion lab.     
        //----------------------------------------------------------------------
        virtual bool get(OUTPUT_TYPE_PTR& item)
        {
            item = OUTPUT_TYPE_PTR();
            if (!empty()) {
                item = m_Queue.front();
                m_Queue.pop_front();
            }

            return item.get() != NULL;
        }

        //----------------------------------------------------------------------
        ///@brief   Return the number of items that can still be removed from   
        ///         the conversion lab.                                         
        //----------------------------------------------------------------------
        size_t size() const  { return m_Queue.size(); }


        //----------------------------------------------------------------------
        ///@brief   Return true if there are no more elements to be removed     
        ///         from the conversion lab.                                    
        //----------------------------------------------------------------------
        bool   empty() const { return m_Queue.empty(); }

        //----------------------------------------------------------------------
        ///@brief   Return true if we're no longer going to produce data.       
        ///                                                                     
        ///         This method can be used to inform other routines that this  
        ///         object has reached a state that it will no longer produce   
        ///         output.                                                     
        ///                                                                     
        ///         Why would that be the case?  Let's assume that the derived  
        ///         class can only tolerates so many errors before deciding     
        ///         that the results are not worth looking at.  In that case,   
        ///         the derived class should polymorph this method and return   
        ///         true when the condition arises that it will no longer       
        ///         create meaningful results no matter how much input data is  
        ///         put into the conversion lab.                                
        //----------------------------------------------------------------------
        virtual bool stopProcessing() { return false; }

        //----------------------------------------------------------------------
        ///@brief   Return the name of the class.  Derived classes should       
        ///         override this function.                                     
        //----------------------------------------------------------------------
        virtual std::string className() const { return "conversion lab"; }

    protected:
        //----------------------------------------------------------------------
        ///@brief   Storage mechanism for the output data.                      
        //----------------------------------------------------------------------
        std::deque<OUTPUT_TYPE_PTR> m_Queue;

        //----------------------------------------------------------------------
        ///@brief   Add an output item to be retrieved.                         
        //----------------------------------------------------------------------
        void push_back(OUTPUT_TYPE_PTR output)
        {
            m_Queue.push_back(output);
            dataReady(output);
        }

        //----------------------------------------------------------------------
        ///@brief   Called when there is data that can be retreived from the    
        ///         get method.                                                 
        ///                                                                     
        ///         This method is loosely patterned after the Qt frame work.   
        ///         If a derived class wants to know when data is available     
        ///         in an event driven format (versus polling the get method)   
        ///         this is the routine to overload.                            
        ///                                                                     
        ///@note    This routine is called after the data has been enqueued.    
        ///         The derived class could (in theory) use the get method      
        ///         to remove the item from the queue.                          
        //----------------------------------------------------------------------
        virtual void dataReady(OUTPUT_TYPE_PTR evenet) { }

}; // class ConversionLabBase //


} // namespace work
} // namespace lib


#endif // #ifndef LIB_WORK_CONVERSIONLABBASE_H_FILE_GUARD

