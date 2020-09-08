//------------------------------------------------------------------------------
///                                                                             
///@file lib_ieee_ts754_work_float.h                                            
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
///                                                                             
///@brief   A library of routines for dealing with IEEE technical specficiation 
///         754 floating point numbers.                                         
///                                                                             
///@par Thread Safety:  object                                                  
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
///@version 2020-09-02  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_IEEE_TS754_WORK_FLOAT_H_FILE_GUARD
#define LIB_IEEE_TS754_WORK_FLOAT_H_FILE_GUARD

#include <stdint.h>
#include <vector>

#include "debug.h"

namespace lib {
namespace ieee {
namespace ts754 {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool getFloat16(double& result, void const* memory, uint64_t bit_offset);
bool getFloat32(double& result, void const* memory, uint64_t bit_offset);
bool getFloat64(double& result, void const* memory, uint64_t bit_offset);

//------------------------------------------------------------------------------
///@brief   Return the 16-bit floating point number held at the given bit       
///         offset.                                                             
///                                                                             
///@tparam  INTEGER_TYPE    An integer type for the std::vector holding the data
///@param   result          The double holding the floating point value found.  
///@param   memory          The data to extract the floating point number from. 
///@param   bit_offset      The offset to the first bit in the value to fetch.  
//------------------------------------------------------------------------------
template <typename INTEGER_TYPE>
bool getFloat16(
    double& answer
  , std::vector<INTEGER_TYPE> const& memory
  , uint64_t bit_offset
)
{
    bool result(false);
    answer = 0.0;           /// @todo Should we set this to NaN (not a number)? 

    if ((bit_offset + 16) <= (memory.size() * sizeof(INTEGER_TYPE) * 8))
    {
        result = getFloat16(answer, &memory[0], bit_offset);
    }

    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the 32-bit floating point number held at the given bit       
///         offset.                                                             
///                                                                             
///@tparam  INTEGER_TYPE    An integer type for the std::vector holding the data
///@param   result          The double holding the floating point value found.  
///@param   memory          The data to extract the floating point number from. 
///@param   bit_offset      The offset to the first bit in the value to fetch.  
//------------------------------------------------------------------------------
template <typename INTEGER_TYPE>
bool getFloat32(
    double& answer
  , std::vector<INTEGER_TYPE> const& memory
  , uint64_t bit_offset
)
{
    bool result(false);
    answer = 0.0;           /// @todo Should we set this to NaN (not a number)? 

    if ((bit_offset + 32) <= (memory.size() * sizeof(INTEGER_TYPE) * 8))
    {
        result = getFloat32(answer, &memory[0], bit_offset);
    }

    return result;
}


//------------------------------------------------------------------------------
///@brief   Return the 16-bit floating point number held at the given bit       
///         offset.                                                             
///                                                                             
///@tparam  INTEGER_TYPE    An integer type for the std::vector holding the data
///@param   result          The double holding the floating point value found.  
///@param   memory          The data to extract the floating point number from. 
///@param   bit_offset      The offset to the first bit in the value to fetch.  
//------------------------------------------------------------------------------
template <typename INTEGER_TYPE>
bool getFloat64(
    double& answer
  , std::vector<INTEGER_TYPE> const& memory
  , uint64_t bit_offset
)
{
    bool result(false);
    answer = 0.0;           /// @todo Should we set this to NaN (not a number)? 

    if ((bit_offset + 64) <= (memory.size() * sizeof(INTEGER_TYPE) * 8))
    {
        result = getFloat64(answer, &memory[0], bit_offset);
    }

    return result;
}


} // namespace work
} // namespace ts754 
} // namespace ieee
} // namespace lib


#endif // #ifndef LIB_IEEE_TS754_WORK_FLOAT_H_FILE_GUARD

