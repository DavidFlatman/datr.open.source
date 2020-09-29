//------------------------------------------------------------------------------
///@file lib_ds_standardunits.cpp                                               
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#include "lib_ds_standardunits.h"

static const size_t g_BitsPerByte(8);

namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///@brief   Return the number of bits in count units.                           
//------------------------------------------------------------------------------
size_t sizeInBits(StandardUnits su, size_t count)
{
    return (StandardUnits::Bits == su) 
        ? count 
        : count * sizeInBytes(su) * g_BitsPerByte
    ;
}


//------------------------------------------------------------------------------
///@brief   Return the number of bytes needed to hold count units.              
///                                                                             
///@note    When it comes to bits, the number is rounded up since it takes a    
///         byte to hold 1 bit (0 bytes holds a maximum of 0 bits).             
//------------------------------------------------------------------------------
size_t sizeInBytes(StandardUnits su, size_t count)
{
    switch (su)
    {
        case StandardUnits::Bits:   
            return (count + g_BitsPerByte - 1) / g_BitsPerByte;

        case StandardUnits::Bytes:  return count;
        case StandardUnits::Words:  return 2 * count;
        case StandardUnits::DWORD:  return 4 * count;
        case StandardUnits::QWORD:  return 8 * count;
    }

    return 0;
}

} // namespace ds
} // namespace lib

