//------------------------------------------------------------------------------
///                                                                             
///@file lib_ieee_ts754_work_float.cpp                                          
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
///                                                                             
///@par Notes About IEEE 764 Floating Point Numbers                             
///         --  The floating point numbers are comprised of 3 parts:            
///                 --  sign bit                                                
///                 --  mantissa                                                
///                 --  exponents                                               
///         --  If the sign bit is 0, the value is positive; if 1, negative.    
///         --  The exponent has a bias:                                        
///                 --  stored_exponent = actual_exponent + exponent_bias;      
///                 --  The length of the bias is the same as the length of the 
///                     exponent.                                               
///                 --  The value of the the exponent bias is unsigned with the 
///                     high bit 0 and the other bits 1.                        
///         --  The mantissa has an implied leading 1.  Since there is always a 
///             1 there; it is not actually stored.                             
///         --  Assuming the mantissa has the imlied bit set, the three         
///             components go together with:                                    
///                 (sign ? -1 : 1) * mantissa * pow(2, exponent);              
///                                                                             
///@version 2020-09-02  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_ieee_ts754_work_float.h"

#include "lib_bits_work.h"

#include <stdint.h>
#include <string.h>                 // memcpy


namespace lib {
namespace ieee {

//------------------------------------------------------------------------------
///@brief   IEEEE Techncial Specification 754                                   
//------------------------------------------------------------------------------
namespace ts754 {
namespace work {

//==============================================================================
//      If there is need for this elsewhere in the code, it can probably be put 
//      into a header ... but for now, I don't think it is required elsewhere.  
//                                                                              
//      These are basic facts about the internal CPU double layout and also map 
//      to IEEE 754 64-bit floating point number.                               
//==============================================================================
union DoubleBitLayout
{
    double  m_Double;

    struct 
    {
        #ifdef HIGH_BIT_FIRST
            uint64_t    m_IsNegative:1;
            uint64_t    m_Exponent:11;
            uint64_t    m_Mantissa:52;
        #else
            uint64_t    m_Mantissa:52;
            uint64_t    m_Exponent:11;
            uint64_t    m_IsNegative:1;
        #endif
    };
};

static const uint64_t g_MantissaSize(52);


//------------------------------------------------------------------------------
///@brief   Pull the bits from the memory and convert to a double based on the  
///         parameters given.                                                   
///@param   answer      The 64-bit double to receive the value.                 
///@param   memory      Pointer to the memory holding the floating point value. 
///@param   bit_offset  The number of bits to skip in memory to get to the      
///                     first bit of the floating point number.                 
///@param   exp_size    The number of bits in the exponent.                     
///@param   man_size    The number of bits in the mantissa.                     
//------------------------------------------------------------------------------
static bool getFloatInternal(
    double&         answer
  , void const*     memory
  , uint64_t        bit_offset
  , uint64_t        exp_size
  , uint64_t        man_size
)
{
    answer = 0;

    uint64_t    exponent(0);
    uint64_t    mantissa(0);

    //--------------------------------------------------------------------------
    //  The exponent bias is effectively binary all ones except the high bit.   
    //  We have to convent the exponent from having the source bias to having   
    //  the double's bias (ten bits set to 1).                                  
    //--------------------------------------------------------------------------
    static const uint64_t double_bias(0x3FF);       // 10 bits on   

    uint64_t exponent_bias((~uint64_t(0)) >> (65-exp_size));

    bool result = lib::bits::work::getUnsigned(
        exponent
      , memory
      , bit_offset+1                // + 1 to skip sign bit
      , exp_size
    );

    exponent += double_bias - exponent_bias;



    //--------------------------------------------------------------------------
    //  The mantissa needs to be shoved as far as it can to the                 
    //  left ...                                                                
    //--------------------------------------------------------------------------
    result = result && lib::bits::work::getUnsigned(
        mantissa
      , memory
      , bit_offset+1+exp_size       //  skip sign and exponent
      , man_size
    );

    mantissa = mantissa << (g_MantissaSize - man_size);

    DoubleBitLayout dbl;
    dbl.m_IsNegative = lib::bits::work::isBitSet(memory, bit_offset);
    dbl.m_Exponent = exponent;
    dbl.m_Mantissa = mantissa;

    if (result)
    {
        answer = dbl.m_Double;
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
bool getFloat16(double& result, void const* memory, uint64_t bit_offset)
{
    bool answer = getFloatInternal(
        result
      , memory
      , bit_offset
      , 5
      , 10
    );
    return answer;
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
bool getFloat32(double& result, void const* memory, uint64_t bit_offset)
{
    return getFloatInternal(
        result
      , memory
      , bit_offset
      , 8
      , 23
    );
}

//------------------------------------------------------------------------------
///@brief   Return the 64-bit floating point number held at the given bit       
///         offset.                                                             
///                                                                             
///@tparam  INTEGER_TYPE    An integer type for the std::vector holding the data
///@param   result          The double holding the floating point value found.  
///@param   memory          The data to extract the floating point number from. 
///@param   bit_offset      The offset to the first bit in the value to fetch.  
//------------------------------------------------------------------------------
bool getFloat64(double& answer, void const* memory, uint64_t bit_offset)
{
    uint64_t u64;

    bool result = lib::bits::work::getUnsigned(u64, memory, bit_offset, 64);
    memcpy(&answer, &u64, sizeof(answer));

    return result;
}


} // namespace work //
} // namespace ts754 //
} // namespace ieee //
} // namespace lib //

