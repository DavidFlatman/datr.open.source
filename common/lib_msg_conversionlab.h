//------------------------------------------------------------------------------
///@file lib_msg_conversionlab.h                                                
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Removed boost::scoped_ptr                       
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                                                                             
///@version 2018-10-30  DHF     Changed publishAll to use pointer to constant.  
///                                                                             
///@version 2016-11-01  DHF     lib::msg::NULL* moved to lib::ds::NULL*         
///                                                                             
///@version 2015-05-14  DHF     Added more comment information.                 
///                             Added stopProcessing method.                    
///                                                                             
///@versoin 2015-04-01  YBS     Added typename P4.                              
///                                                                             
///@version 2014-12-03  DHF     Added name to Subscriber of "conversion-lab".   
///                                                                             
///@version 2014-08-06  DHF     File creation                                   
///                                                                             
///@version 2014-08-06  MNT     Design suggestion for template.                 
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_MSG_CONVERSIONLAB_H_FILE_GUARD
#define LIB_MSG_CONVERSIONLAB_H_FILE_GUARD

#include "lib_ds_null.h"
#include "lib_ds_shared_ptr.h"
#include "lib_msg_publisher.h"
#include "lib_msg_subscriber.h"
#include "lib_work_conversionlab.h"

#include "debug.h"

#include <type_traits>

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///@brief   Determine if the call should be ByPointer's put or ByReference's put
///                                                                             
///         The base ConversionLab may pass the input type ByReference or       
///         ByPointer.  This template allows the compiler to decide at compile  
///         time without instantiating a call to a method that doesn't exist.   
///                                                                             
///@tparam  ByPointer       boolean that we will use for template specialization
///@tparam  CONVERSION_LAB  The underlying ConversionLab class.                 
///@tparam  INPUT_TYPE      The type that the conversion lab accepts in its put 
///                         method.                                             
//------------------------------------------------------------------------------
template <bool ByPointer, typename CONVERSION_LAB, typename INPUT_TYPE> 
class ConversionLabHelper;

//------------------------------------------------------------------------------
///@brief   Specialize the ConversionLabHelper and call the put ByPointer.      
//------------------------------------------------------------------------------
template <typename CONVERSION_LAB, typename INPUT_TYPE> 
class ConversionLabHelper<true, CONVERSION_LAB, INPUT_TYPE>
{
    public:
        static void put(
            CONVERSION_LAB* lab
         ,  lib::ds::shared_ptr<const INPUT_TYPE>& p
        )
        {
            lab->put(p);
        }
};

//------------------------------------------------------------------------------
///@brief   Specialize the ConversionLabHelper and call the put ByReference.    
//------------------------------------------------------------------------------
template <typename CONVERSION_LAB, typename INPUT_TYPE> 
class ConversionLabHelper<false, CONVERSION_LAB, INPUT_TYPE>
{
    public:
        static void put(
            CONVERSION_LAB* lab
         ,  lib::ds::shared_ptr<const INPUT_TYPE>& p
        )
        {
            lab->put(*p);
        }
};

//------------------------------------------------------------------------------
///                                                                             
///@par Class: ConversionLab                                                    
///                                                                             
///@brief   Provide publisher/subscriber functionality to code derived from     
///         lib::work::ConversionLab.                                           
///                                                                             
///@par Thread Safety:  class/special                                           
///         The pub/sub portion inherit pub/sub thread-safety (somewhat object).
///         The conversion lab inherits from the base conversion lab            
///         is probably class thread-safe.                                      
///                                                                             
///@par Expected Usage                                                          
///     @code                                                                   
///         #include "lib_msg_conversionlab.h"                                  
///         #include "my_work_messagecollectionlab.h"                           
///                                                                             
///         namespace my {                                                      
///         namespace msg {                                                     
///                                                                             
///         typedef lib::msg::ConversionLab<                                    
///             my::work::MessageCollectionLab                                  
///           , my::ds::InputType                                               
///           , my::ds::OutputType                                              
///         > MessageCollectionLab;                                             
///                                                                             
///         } // namespace msg                                                  
///         } // namespace my                                                   
///                                                                             
///     @endcode                                                                
///                                                                             
///     See also demo_msg_conversionlab.cpp                                     
///                                                                             
///@par Why Not Just?                                                           
///         There is a template complexity added via the ConversionLabHelper    
///         specialized templates above.  Why have them?  Why not just do:      
///         @code                                                               
///             if (dynamic_cast<lib::work::ConversionLabByPointer*>(this)) {   
///                 CONVERSION_LAB::put(p);                                     
///             } else {                                                        
///                 CONVERSION_LAB::put(*p);                                    
///             }                                                               
///     @endcode                                                                
///\n\n                                                                         
///     The problem is that the compiler will still force the creation of both  
///     calls to "put" even though one of them will never be called.  The       
///     compiler will then complain because one of the put methods (which       
///     happens to be the one that is never called) does not exist -- creating  
///     a compiler error.                                                       
///\n\n                                                                         
///     By using template specialization, we avoid having the compiler          
///     generating code that will never be executed (which relies on a method   
///     that doesn't exist).  Template specialization doesn't generate the code 
///     until the template is instantiated.                                     
///                                                                             
///@tparam  CONVERSION_LAB  The ::work:: covnersion laboratory that will        
///                         actually do the type conversions.  This class should
///                         be derived from either                              
///                             lib::work::ConversionLabByPointer               
///                         or                                                  
///                             lib::work::ConversionLabByReference             
///                                                                             
///@tparam  INPUT_TYPE      The type of the input conversion.  Note that this   
///                         is the base type (versus a boost shared pointer)    
///                         This should be the same INPUT_TYPE as specified     
///                         by the CONVERSION_LAB's INPUT_TYPE.                 
///                                                                             
///@tparam  OUTPUT_TYPE     The type to convert to. Note that this is the base  
///                         type (versus a boost shared pointer). This should   
///                         be the same INPUT_TYPE as specified by the          
///                         CONVERSION_LAB's OUTPUT_TYPE.                       
///                                                                             
///@tparam  OTHER_PUB_0     If other items need to be published (e.g., stats)   
///                         by a derived class, add it here.                    
///                                                                             
///@tparam  OTHER_PUB_1     If yet another type needs to be published by a      
///                         derived class, add it here.                         
///                                                                             
//------------------------------------------------------------------------------
template <
    typename CONVERSION_LAB
  , typename INPUT_TYPE
  , typename OUTPUT_TYPE
  , typename OTHER_PUB_0 = ds::NULL_DEFAULT_TYPE_1
  , typename OTHER_PUB_1 = ds::NULL_DEFAULT_TYPE_2
>
class ConversionLab
    : public lib::msg::Subscriber<INPUT_TYPE>
    , public lib::msg::Publisher<OUTPUT_TYPE, OTHER_PUB_0, OTHER_PUB_1>
    , public CONVERSION_LAB
{
    public:
        //----------------------------------------------------------------------
        //  Allow the constructor have up to three types.  Except for the       
        //  default constructor, the other constructors will not be "compiled"  
        //  unless they are instantiated.                                       
        //----------------------------------------------------------------------
        ConversionLab() 
            : lib::msg::Subscriber<INPUT_TYPE>(CONVERSION_LAB::className())
        { }

        template<typename P1>
        ConversionLab(P1 p1) 
            : lib::msg::Subscriber<INPUT_TYPE>(CONVERSION_LAB::className())
            , CONVERSION_LAB(p1) 
            { }

        template<typename P1, typename P2>
        ConversionLab(P1 p1, P2 p2) 
            : lib::msg::Subscriber<INPUT_TYPE>(CONVERSION_LAB::className())
            ,CONVERSION_LAB(p1, p2) 
            { }

        template<typename P1, typename P2, typename P3>
        ConversionLab(P1 p1, P2 p2, P3 p3) 
            : lib::msg::Subscriber<INPUT_TYPE>(CONVERSION_LAB::className())
            ,CONVERSION_LAB(p1, p2, p3) 
            { }

        template<typename P1, typename P2, typename P3, typename P4>
        ConversionLab(P1 p1, P2 p2, P3 p3, P4 p4) 
            : lib::msg::Subscriber<INPUT_TYPE>(CONVERSION_LAB::className())
            ,CONVERSION_LAB(p1, p2, p3, p4) 
            { }

        virtual ~ConversionLab() { }

        //----------------------------------------------------------------------
        ///@brief   Handle the published input type for the conversion lab.     
        //----------------------------------------------------------------------
        void process(lib::ds::shared_ptr<const INPUT_TYPE>& input)
        {
            //------------------------------------------------------------------
            //  The ConversionLabHelper decides whether we need to use put      
            //  by reference or put by pointer.                                 
            //------------------------------------------------------------------
            ConversionLabHelper<
                std::is_base_of<
                    lib::work::ConversionLabByPointer<INPUT_TYPE, OUTPUT_TYPE, true>
                  , CONVERSION_LAB
                >::value
              || std::is_base_of<
                    lib::work::ConversionLabByPointer<INPUT_TYPE, OUTPUT_TYPE, false>
                  , CONVERSION_LAB
                >::value
              , CONVERSION_LAB
              , INPUT_TYPE
            >::put(this, input);

            //------------------------------------------------------------------
            //  Publish any of the output types that are available.             
            //------------------------------------------------------------------
            publishAll();

        }

        //----------------------------------------------------------------------
        ///@brief   Inform the conversion lab that no more input is coming and  
        ///         publish all queued up data.                                 
        //----------------------------------------------------------------------
        virtual bool beforeEndPublication() 
        { 
            CONVERSION_LAB::finalize();
            publishAll();
            return true; 
        }

        //----------------------------------------------------------------------
        ///@brief   Return true if there is any reason that we should stop      
        ///         processing.                                                 
        //----------------------------------------------------------------------
        virtual bool stopProcessing()
        {
            return CONVERSION_LAB::stopProcessing() 
                || lib::msg::Subscriber<INPUT_TYPE>::stopProcessing()
            ;
        }
    
    protected:
        //----------------------------------------------------------------------
        ///@brief   Publish all of the output items currently queued up.        
        //----------------------------------------------------------------------
        virtual void publishAll()
        {
            //------------------------------------------------------------------
            //  While we have more OUTPUT_TYPE's in the queue, get them and     
            //  publish them.                                                   
            //------------------------------------------------------------------
            lib::ds::shared_ptr<const OUTPUT_TYPE> output;
            while (CONVERSION_LAB::get(output)) {
                lib::msg::Publisher<
                    OUTPUT_TYPE
                  , OTHER_PUB_0
                  , OTHER_PUB_1
                >::publish(
                    lib::ds::shared_ptr<const OUTPUT_TYPE>(output)
                );
            }
        }

    private:
        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        ConversionLab(const ConversionLab& that);
        ConversionLab& operator=(const ConversionLab& that);

}; // class ConversionLab //

} // namespace msg
} // namespace lib

#endif // #ifndef LIB_MSG_CONVERSIONLAB_H_FILE_GUARD

