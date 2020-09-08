//------------------------------------------------------------------------------
///@file lib_eu_work_value.h                                                    
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_EU_WORK_VALUE_H_FILE_GUARD
#define LIB_EU_WORK_VALUE_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"
#include "lib_eu_work_metadata.h"

#include <memory>
#include <stdint.h>
#include <stddef.h>                     /// size_t
#include <vector>


namespace lib {
namespace eu {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Value                                                            
///                                                                             
///@brief   An engineering unit value.                                          
///                                                                             
///         There are other ways that this could be implemented, but this will  
///         probably be the easiest for our needs.  A template system that has  
///         the various exponents on the measurements as template parameters    
///         would work if everything an an SI unit and we were mainly intersted 
///         in making sure that someone doesn't accidently add volume to area.  
///                                                                             
///         However, the C++ code is not going to be doing arithmetic with the  
///         engineering units.  It will merely be pulling the values out of     
///         bit streams and writing them in text format.                        
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par IEEE Floating point Number Representation                               
///         IEEE (Institute of Electrical and Electronics Engineers) has        
///         standardized Floating-Point Representation as following diagram.    
///                                                                             
///         So, actual number is (-1)s(1+m)x2(e-Bias), where s is the sign bit, 
///         m is the mantissa, e is the exponent value, and Bias is the bias    
///         number. The sign bit is 0 for positive number and 1 for negative    
///         number. Exponents are represented by or two's complement            
///         representation.                                                     
///                                                                             
///         According to IEEE 754 standard, the floating-point number is        
///         represented in following ways:                                      
///             --  Half Precision (16 bit): 1 sign bit, 5 bit exponent, and    
///                 10 bit mantissa                                             
///             --  Single Precision (32 bit): 1 sign bit, 8 bit exponent, and  
///                 23 bit mantissa                                             
///             --  Double Precision (64 bit): 1 sign bit, 11 bit exponent, and 
///                 52 bit mantissa                                             
///             --  Quadruple Precision (128 bit): 1 sign bit, 15 bit exponent, 
///                 and 112 bit mantissa                                        
///                                                                             
///         Special Value Representation:                                       
///                                                                             
///         There are some special values depended upon different values of the 
///         exponent and mantissa in the IEEE 754 standard.                     
///             --  All the exponent bits 0 with all mantissa bits 0            
///                 represents 0. If sign bit is 0, then +0, else -0.           
///             --  All the exponent bits 1 with all mantissa bits 0 represents 
///                 infinity. If sign bit is 0, then +infinit, else -infinit.   
///             --  All the exponent bits 0 and mantissa bits non-zero          
///                 represents denormalized number.                             
///             --  All the exponent bits 1 and mantissa bits non-zero          
///                 represents error.                                           
///                                                                             
///         The above taken from: https://www.tutorialspoint.com/fixed-point-and-floating-point-number-representations#:~:text=The%20sign%20bit%20is%200,exponent%2C%20and%2010%20bit%20mantissa
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-08-27  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Value
{
    public:
        Value(ConstMetaDataPtr metaData);

        template <typename TYPE>
        Value(
            ConstMetaDataPtr metaData 
          , TYPE value
        ) : m_MetaData(metaData)
          , m_Double(value)
          , m_Unsigned(value)
          , m_Integer(value)
        {
        }

        template <typename INTEGER_TYPE>
        Value(
            ConstMetaDataPtr metaData
          , std::vector<INTEGER_TYPE> const& vec
        ) : m_MetaData(metaData)
        {
            set(vec);
        }

        Value(const Value& that) = default;
        Value(Value&& that) = default;
        Value& operator=(const Value& that) = default;
        virtual ~Value();

        ConstMetaDataPtr metaData() const;

        double      toDouble() const;
        void        setDouble(double d);

        uint64_t    toUnsigned() const;
        void        setUnsigned(uint64_t u);

        uint64_t    toInteger() const;
        void        setInteger(int64_t i);

        template <typename INTEGER_TYPE>
        void set(std::vector<INTEGER_TYPE> const& vec)
        {
            void const* p(&vec[0]);
            setInternal(p, vec.size() * sizeof(INTEGER_TYPE));
        }


        enum PARTS
        {
            VALUE       =   0x01
          , UNITS       =   0x02
          , COMMA       =   0x04
          , E_FORMAT    =   0x08

        };
        std::string toString(
            PARTS       parts = PARTS(VALUE | UNITS)
          , uint32_t    total_size    = 0
          , uint32_t    fraction_size = 6
        );
        std::string toString(
            uint32_t    total_size 
          , uint32_t    fraction_size = 6
        ) 
        { 
            return toString(
                PARTS(VALUE | UNITS)
              , total_size
              , fraction_size
            ); 
        }


    
    protected:
        virtual void setInternal(void const* p, size_t size);

    private:
        ConstMetaDataPtr    m_MetaData;
        double              m_Double;
        uint64_t            m_Unsigned;
        int64_t             m_Integer;

        template <typename TYPE>
        void setInternal(TYPE v)
        {
            m_Double    = v;
            m_Unsigned  = v;
            m_Integer   = v;
        }

}; // class Value //

using  ValuePtr = lib::ds::shared_ptr<Value>;
using  ConstValuePtr = lib::ds::shared_ptr<const Value>;

} // namespace work
} // namespace eu
} // namespace lib


#endif // #ifndef LIB_EU_WORK_VALUE_H_FILE_GUARD

