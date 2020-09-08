//------------------------------------------------------------------------------
///                                                                             
///@file lib_work_conversionlab.h                                               
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-03  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                             Changed typedef to using                        
///                                                                             
///@version 2018-10-30  DHF     Major refactoring.                              
///                                 -   Removed unused INPUT_TYPE.              
///                                 -   Added ConversionLabBase                 
///                                 -   Added CONSTANT template specialization  
///                                                                             
///@version 2015-05-17  DHF     Added dataReadyEvent(event).                    
///                                                                             
///@version 2015-05-14  DHF     Added stopProcessing().                         
///                                                                             
///@version 2014-08-07  DHF     Renamed to ConversionLab from misnomer          
///                             ConversionFactory.                              
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//@par Class Hierarchy                                                          
//          There are a number of templates and classes at play here.  The      
//          following (hopefully) will clear up some of the relationships.      
//                                                                              
//          Note:  In the work namespace, there are only two classes that a     
//          developer needs to be concerned with:  ConversionLabByPointer and   
//          ConversionLabByReference.  The former should be used when the input 
//          must be a boost shared_ptr, the latter should be used in all other  
//          cases.                                                              
//                                                                              
//          -   class-template lib::work::ConversionLabByPointer                
//                  -   inherits from class-template ConversionLab              
//                         -    inherits from class-template ConversionLabBase  
//                                                                              
//          -   class-template lib::work::ConversionLabByReference              
//                  -   inherits from class-template ConversionLab              
//                         -    inherits from class-template ConversionLabBase  
//                                                                              
//          -   class-template ConversionLabBase implements the output queueing 
//              mechanism.  The queue holds boost::shared_ptr<OUTPUT_TYPE>      
//              objects or boost::shared_ptr<const OUTPUT_TYPE> objects.        
//                                                                              
//          -   class-template ConversionLab adds the get methods for the       
//              non-constant ConversionLabs.                                    
//                                                                              
//          -   class-template ConverionLabByReference adds the input mechanism 
//              to the conversion lab, allowing the input to be a reference to  
//              an object (versus a pointer).                                   
//                                                                              
//          -   class-template ConversionLabByPointer adds the input mechanism  
//              to the conversion lab, allowing the input to be a boost         
//              shared_ptr.                                                     
//------------------------------------------------------------------------------
                                                                                
#ifndef LIB_WORK_CONVERSIONLAB_H_FILE_GUARD
#define LIB_WORK_CONVERSIONLAB_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"
#include "lib_work_conversionlabbase.h"

#include <boost/scoped_ptr.hpp>

namespace lib {
namespace work {


//------------------------------------------------------------------------------
///@brief   Provide a standard interface for converting from one type to        
///         another within the DATR environment.                                
///                                                                             
///         The purpose of ConversionLab class is make using the                
///         @ref di_conversion_lab easier.                                      
///                                                                             
///@warning To be correct, you should derive your class from either             
///         ConversionLabByReference or ConversionLabByPointer.  @e Why?        
///             -#  The ConversionLab does not have a "put" method for          
///                 the input type.                                             
///             -#  The ConversionLabByReference is for when the conversion     
///                 lab does not have to hold onto the input object beyond      
///                 the call to put.                                            
///             -#  The ConversionLabByPointer is for when the conversion       
///                 lab might possibly (or definitely well) hang onto an        
///                 input object beyond the call to put.  This can be because   
///                     -   Multiple inputs go into a single output             
///                     -   An output is split between multiple inputs.         
///                     -   The memory of the input object is kept as part of   
///                         the output object (e.g., a CH10 packet to MS-1553   
///                         holds the CH10 packet pointer on output to prevent  
///                         having to duplicate the data inside the CH10        
///                         packet.                                             
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@tparam  OUTPUT_TYPE     The type of the output from the conversion lab      
///                         (without the boost::shared_ptr around it).          
///@param   CONSTANT        If true, then the output is a boost::shared_ptr     
///                         to a constant object.  This is useful when the      
///                         constant input data is passed to the output without 
///                         copying the data (think filter).                    
///                         If the output object is a new object and doesn't    
///                         require the input data at all anymore, using false  
///                         is preferred since it will be more flexible for the 
///                         users of the derived class.                         
///                                                                             
//------------------------------------------------------------------------------
template <typename OUTPUT_TYPE, bool CONSTANT>
class ConversionLab;

//------------------------------------------------------------------------------
///@brief   Template specialization where the output from the conversion lab    
///         is a pointer to a constant object.                                  
//------------------------------------------------------------------------------
template <typename OUTPUT_TYPE>
class ConversionLab<OUTPUT_TYPE, true>
    : public ConversionLabBase<lib::ds::shared_ptr<const OUTPUT_TYPE> >
{
    public:
        using BASE_CLASS =  ConversionLabBase<lib::ds::shared_ptr<const OUTPUT_TYPE> > ;
        using OutputType = lib::ds::shared_ptr<const OUTPUT_TYPE>;

    protected:

    private:

};


//------------------------------------------------------------------------------
///@brief   Template specialization where the output from the conversion lab    
///         is either a pointer to a constant object or a non-constant object.  
//------------------------------------------------------------------------------
template <typename OUTPUT_TYPE>
class ConversionLab<OUTPUT_TYPE, false>
    : public ConversionLabBase<lib::ds::shared_ptr<OUTPUT_TYPE> >
{
    public:
        using BASE_CLASS = ConversionLabBase<lib::ds::shared_ptr<OUTPUT_TYPE> >;
        using OutputType = lib::ds::shared_ptr<OUTPUT_TYPE>;

        //----------------------------------------------------------------------
        ///@brief   Return a pointer to a non-constant element.                 
        //----------------------------------------------------------------------
        virtual bool get(OutputType& item)
        {
            return BASE_CLASS::get(item);
        }

        //----------------------------------------------------------------------
        ///@brief   Return a pointer to a constant element.                     
        //----------------------------------------------------------------------
        virtual bool get(lib::ds::shared_ptr<const OUTPUT_TYPE>& item)
        {
            OutputType temp;
            get(temp);
            item = lib::ds::shared_ptr<const OUTPUT_TYPE>(temp);
            return item.get() != NULL;
        }

    protected:

    private:

};


//------------------------------------------------------------------------------
///@brief   A conversion lab where the input data does not need to be held      
///         as part of the conversion process.                                  
///                                                                             
///         Unless there is a compelling reason to use ConversionLabByPointer,  
///         then ConversionLabByReference should be used since the user of the  
///         ConversionLab does not have to have a pointer to the object.        
///                                                                             
///@tparam  INPUT_TYPE      The type that will be passed to the put command.    
///@tparam  OUTPUT_TYPE     The base type of the output (the templates will     
///                         eventually wrap this type in boost::shared_ptr).    
///@tparam  CONSTANT        If true, the parameter type of the get method is    
///                         boost::shared_ptr<const OUTPUT_TYPE>                
///                         If false, boost::shared_ptr<OUTPUT_TYPE>            
//------------------------------------------------------------------------------
template <typename INPUT_TYPE, typename OUTPUT_TYPE, bool CONSTANT = false>
class ConversionLabByReference
    : public ConversionLab<OUTPUT_TYPE, CONSTANT>
{
    public:
        using BASE_CLASS = ConversionLab<OUTPUT_TYPE, CONSTANT>;
        using InputType = INPUT_TYPE&;

        //----------------------------------------------------------------------
        ///@brief   Called when no further data will be passed to the           
        ///         ConversionLab.                                              
        ///                                                                     
        ///         Since the input is by reference, there is a good chance that
        ///         no action needs to be taken when finalize is called.        
        //----------------------------------------------------------------------
        virtual void finalize() { }

        //----------------------------------------------------------------------
        ///@brief   Called when the ConversionLab has data to be processed.     
        ///                                                                     
        ///         This is the main processing method for the ConversionLab.   
        ///         In general, a derived class will implement the put method   
        ///         and call the push_back method with the generated output.    
        //----------------------------------------------------------------------
        virtual void put(INPUT_TYPE const&  input_item) = 0;

    protected:
};

//------------------------------------------------------------------------------
///@brief   A conversion lab where the input data must come from a boost        
///         shared_ptr.                                                         
///                                                                             
///         An explicit reason should be stated for using the ByPointer version 
///         versus the ByReference.  The are two main reasons for using         
///         ByPointer                                                           
///             -#  The output either directly or indirectly contains a         
///                 pointer to the input data, or                               
///             -#  Input data needs to be held between calls to put.           
///                                                                             
///@tparam  INPUT_TYPE      The base type that passed to the put command; the   
///                         template will add the boost::shared_ptr to the      
///                         parameter.                                          
///@tparam  OUTPUT_TYPE     The base type of the output (the templates will     
///                         eventually wrap this type in boost::shared_ptr).    
///@tparam  CONSTANT        If true, the parameter type of the get method is    
///                         boost::shared_ptr<const OUTPUT_TYPE>                
///                         If false, boost::shared_ptr<OUTPUT_TYPE>            
//------------------------------------------------------------------------------
template <typename INPUT_TYPE, typename OUTPUT_TYPE, bool CONSTANT = false>
class ConversionLabByPointer
    : public ConversionLab<OUTPUT_TYPE, CONSTANT>
{
    public:
        using BASE_CLASS = ConversionLab<OUTPUT_TYPE, CONSTANT>;
        using InputType = lib::ds::shared_ptr<const INPUT_TYPE>;

        //----------------------------------------------------------------------
        ///@brief   Called when no further data will be passed to the object.   
        ///                                                                     
        ///         Since the data is being passed by pointer, there is a good  
        ///         chance that the internal buffers will need to be flushed;   
        ///         hence, the derived class must implement a finalize method.  
        //----------------------------------------------------------------------
        virtual void finalize() = 0;

        //----------------------------------------------------------------------
        ///@brief   Called when the ConversionLab has data to be processed.     
        ///                                                                     
        ///         This is the main processing method for the ConversionLab.   
        ///         In general, a derived class will implement the put method   
        ///         and call the push_back method with the generated output.    
        //----------------------------------------------------------------------
        virtual void put(const InputType input_item) = 0;

    protected:
};


} // namespace work
} // namespace lib

#endif // #ifndef LIB_WORK_CONVERSIONLAB_H_FILE_GUARD 
