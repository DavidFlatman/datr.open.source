//------------------------------------------------------------------------------
///@file    lib_bits_work.h                                                     
///                                                                             
///@brief   A collection of bit oriented functions.                             
///                                                                             
///@par     Classification:  UNCLASSIFIED, OPEN SOURCE                          
///                                                                             
///@version 2020-09-01  DHF     File creation.                                  
//------------------------------------------------------------------------------

#include <stdint.h>
#include <vector>

#include "debug.h"

namespace lib {
namespace bits {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool isBitSet(const void* memory, uint64_t bit_number);

//------------------------------------------------------------------------------
///@brief   Return true if the given bit is set to a 1.                         
///@note    The memory is treated like big endian.  Bit 0 is the most           
///         significant bit of the 0th byte.                                    
//------------------------------------------------------------------------------
template <typename INTEGER_TYPE>
bool isBitSet(std::vector<INTEGER_TYPE> const& memory, uint64_t bit_number)
{
    bool result(false);

    if (bit_number <= memory.size() * sizeof(INTEGER_TYPE) * 8)
    {
        result = isBitSet(&memory[0], bit_number);
    }

    return result;
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool getUnsigned(
    uint64_t& result
  , void const* memory
  , uint64_t bit_offset
  , uint64_t bit_count
);

//------------------------------------------------------------------------------
///@brief   Get the unsigned integer from the vector of integers at the given   
///         bit offset and length.                                              
///@tparam  INTEGER_TYPE        The type of integer held in the std::vector.    
///@param   result              The unsigned integer found.                     
//------------------------------------------------------------------------------
template <typename INTEGER_TYPE>
bool getUnsigned(
    uint64_t& result
  , std::vector<INTEGER_TYPE> const& memory
  , uint64_t bit_offset
  , uint64_t bit_count
)
{
    bool answer(false);
    if (bit_offset + bit_count <= memory.size() * sizeof(INTEGER_TYPE) * 8)
    {
        answer = getUnsigned(result, &memory[0], bit_offset, bit_count);
    }

    return answer;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool getSigned(
    int64_t& result
  , void const* memory
  , uint64_t bit_offset
  , uint64_t bit_count
);

//------------------------------------------------------------------------------
///@brief   Retrieve the signed value held at the location in the given memory. 
///                                                                             
///@note    The bit at bit_offset is considered the high order bit of result.   
///         If the bit_offset is 0, then the high bit of memory[0] is used as   
///         the high bit of the result.                                         
///                                                                             
///@tparam  INTEGER_TYPE    An integer (int8_t, int16_t, ... uint32_t, uint64_t)
///                                                                             
///@param   result      The unsigned value found.                               
///@param   memory      A std::vector of integer types that holding the data.   
///@param   bit_offset  The offset to the first bit of the value to be retreived
///@param   bit_count   The number of bits to retreive.                         
///@return  true = There was enough memory to get the result;                   
///         false = There was not enough memory to get the result.              
//------------------------------------------------------------------------------
template <typename INTEGER_TYPE>
bool getSigned(
    int64_t& result
  , std::vector<INTEGER_TYPE> const& memory
  , uint64_t bit_offset
  , uint64_t bit_count
)
{
    bool answer(false);

    if (bit_offset + bit_count <= memory.size() * sizeof(INTEGER_TYPE) * 8)
    {
        answer = getSigned(result, &memory[0], bit_offset, bit_count);
    }

    return answer;
}

} // namespace work
} // namespace bits
} // namespace lib
