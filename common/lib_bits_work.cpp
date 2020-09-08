//------------------------------------------------------------------------------
///@file    lib_bits_work.cpp                                                   
///                                                                             
///@par     Classification:  UNCLASSIFIED, OPEN SOURCE                          
///                                                                             
///@version 2020-09-01  DHF     File creation.                                  
//------------------------------------------------------------------------------

#include "lib_bits_work.h"


namespace lib {
namespace bits {
namespace work {
                                                                                
//------------------------------------------------------------------------------
///@brief   Return true if the given bit is set to a 1.                         
///@note    The memory is treated like big endian.  Bit 0 is the most           
///         significant bit of the 0th byte.                                    
//------------------------------------------------------------------------------
bool isBitSet(void const* memory, uint64_t bit_number)
{
    uint8_t* b((uint8_t*) memory);

    uint64_t byte(bit_number / 8);
    bit_number = bit_number % 8;

    return (b[byte] & (1 << (7-bit_number))) != 0;
}

//------------------------------------------------------------------------------
///@brief   Return the unsigned value at the given bit offset for the given     
///         number of bits.                                                     
///                                                                             
///@warning It is assumed that there is sufficient memory pointed to by memory  
///         to retrieve the value.                                              
///                                                                             
///@note    Bit 0 is the most significant bit of the byte pointed to by memory. 
///                                                                             
///@param   result      The unsigned value found at the location.               
///@param   memory      Pointer to memory holding the bits.                     
///@param   bit_offset  How many bits to skip from the beginning of memory.     
///@param   bit_count   The number of bits to return.                           
//------------------------------------------------------------------------------
bool getUnsigned(
    uint64_t& result
  , void const* memory
  , uint64_t bit_offset
  , uint64_t bit_count
)
{
    bool answer(true);

    result = 0;

    for (uint64_t bit = 0; bit < bit_count; ++bit)
    {
        result = (result << 1);
        if (isBitSet(memory, bit_offset + bit))
        {
            result |= 1;
        }
    }

    return answer;
}

//------------------------------------------------------------------------------
///@brief   Return the signed value at the given location.                      
///                                                                             
///         Get the two's compliment signed value.  As a refresher, 2's         
///         compliment is where you flip all the bits and add one.  E.g.,       
///         assume a 16-bit word:                                               
///              1  is 0000000000000001                                         
///                    1111111111111110 (flip bits)                             
///             -1  is 1111111111111111 (add one)                               
///                                                                             
///         So, add 1 and -1:                                                   
///                     0000000000000001                                        
///                   + 1111111111111111                                        
///                   ------------------                                        
///                    10000000000000000                                        
///                                                                             
///         but in 16 bits that's:                                              
///                     0000000000000000                                        
///                                                                             
///@param   result      The unsigned value found at the location.               
///@param   memory      Pointer to memory holding the bits.                     
///@param   bit_offset  How many bits to skip from the beginning of memory.     
///@param   bit_count   The number of bits to return.                           
//------------------------------------------------------------------------------
bool getSigned(
    int64_t&    result
  , void const* memory
  , uint64_t    bit_offset
  , uint64_t    bit_count
)
{
    //--------------------------------------------------------------------------
    //  Copy in everyting except the sign bit.  So, bit_offset is +1 to skip    
    //  over the sign bit, and the number of bits (bit_count) since we are not  
    //  getting the sign bit.                                                   
    //--------------------------------------------------------------------------
    uint64_t unsign;
    getUnsigned(unsign, memory, bit_offset + 1, bit_count - 1);

    //--------------------------------------------------------------------------
    //  If the sign bit is set, we need to extend it all the way through the    
    //  value.                                                                  
    //--------------------------------------------------------------------------
    if (isBitSet(memory, bit_offset))
    {
        //----------------------------------------------------------------------
        //  Get all of the bits to be 1 and then get the trailing bits 0.       
        //                                                                      
        //  The -1 is for the sign bit.                                         
        //----------------------------------------------------------------------
        uint64_t neg(0xFFFFFFFFFFFFFFFFL);
        neg = (neg << (bit_count - 1));
        unsign |= neg;
    }

    result = int64_t(unsign);

    return true;
}


} // namespace work //
} // namespace bits //
} // namespace lib //


