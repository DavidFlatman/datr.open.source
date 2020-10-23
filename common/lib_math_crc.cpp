//------------------------------------------------------------------------------
///@file lib_math_crc.cpp 
///@par CLASSIFICATION: UNCLASSIFIED - OPEN SOURCE                                                       
//------------------------------------------------------------------------------
///                                                                             
///@version                                                                     
///     25-May-2007     AGB     v1.00 - Original Incarnation                    
///                                                                             
///@version                                                                     
///     2010-02-17      DHF     Made to conform with lib coding standard;       
///                             mainly commenting style, naming style, and      
///                             use of stdint.h                                 
///                                                                             
//------------------------------------------------------------------------------

#include "lib_math_crc.h"
#include "lib_math_himath.h"

#include "lib_crc.h"

namespace lib {
namespace math {

//------------------------------------------------------------------------------
/// @brief The "CRC-32" constructor                                             
///                                                                             
/// @par Purpose:                                                               
///         This constructor will compute a 32-bit CRC without reflecting either
///         the input or the output values.  If you need a different bit width  
///         or if you'd like to reflect the values as they are input or output, 
///         you must use the 6-argument constructor.                            
///\n\n                                                                         
///         The default values should correspond to the CRC-32-MPEG2            
///         implementation.                                                     
//------------------------------------------------------------------------------
CRC::CRC ( 
    const uint32_t poly
  , const uint32_t init
  , const uint32_t xorout
) : m_Register(init)
  , m_Poly(poly)
  , m_Init(init)
  , m_XorOut(xorout)
  , m_Width(32)
  , m_ReflectIn(false)
  , m_ReflectOut(false)
{
    makeTable();
}

//------------------------------------------------------------------------------
/// @brief The "6-argument" constructor                                         
///                                                                             
/// @par Purpose:                                                               
///         This constructor provides much more flexibility as it allows the    
///         calling routine to specify the bit width and input and output       
///         reflection.                                                         
///                                                                             
/// @warning                                                                    
///         However, non-32-bit width have not been tested as rigorously as     
///         32-bit widths.                                                      
//------------------------------------------------------------------------------
CRC::CRC ( 
    const uint32_t poly
  , const uint32_t init
  , const uint32_t xorout
  , const uint16_t width
  , bool reflectin
  , bool reflectout
) : m_Register(init)
  , m_Poly(poly)
  , m_Init(init)
  , m_XorOut(xorout)
  , m_Width(width)
  , m_ReflectIn(reflectin)
  , m_ReflectOut(reflectout)
{
    makeTable();
}

//------------------------------------------------------------------------------
/// @brief  Restore the CRC algorithm to its initial state.                     
//------------------------------------------------------------------------------
void CRC::reset()
{
    m_Register = m_Init;
}

//------------------------------------------------------------------------------
/// @brief  Do the real work of the CRC by acceptingg an 8-bit value and        
///         returning the resulting CRC value.                                  
//------------------------------------------------------------------------------
uint32_t CRC::message(uint8_t msg)
{

    /*
    m_Register = update_crc_32(m_Register, msg);
    return m_Register;
    */

    uint32_t lmsg = msg;
    uint32_t outmask = (((1 << (m_Width-1)) - 1) << 1) | 1;
    uint32_t ans;
    uint32_t topbit = 1L << (m_Width-1);
    uint8_t  topbyte;
    
    if (m_ReflectIn) {
        lmsg = reflect(lmsg, 8);
    }

    //--------------------------------------------------------------------------
    //  For now, use the slow version of the algorithm when m_ReflectIn is      
    //  true.  Otherwise, use the table-driven version.                         
    //--------------------------------------------------------------------------
    if (m_ReflectIn)
    {
        m_Register ^= (lmsg << (m_Width-8));
        for (int i=0 ; i < 8 ; i++)
        {
            if (m_Register & topbit) {
                m_Register = (m_Register << 1) ^ m_Poly;
            } else {
                m_Register <<= 1;
            }
            m_Register &= outmask;
        }

    } else {
        //----------------------------------------------------------------------
        //  New fast way (it doesn't work for reflected).                       
        //----------------------------------------------------------------------
        topbyte = (m_Register & 0xff000000) >> 24;
        topbyte ^= lmsg;
        m_Register = (m_Register << 8) ^ m_Table[topbyte];
    }

    if (m_ReflectOut) {
        ans = reflect(m_Register, m_Width);
    } else {
        ans = m_Register;
    }

    return ans ^ m_XorOut;

} // uint32_t CRC::message(uint8_t msg)                                         

//------------------------------------------------------------------------------
/// @brief  Accept a 16-bit value returning the resulting CRC value.            
/// @note   Effectively big endian; do the high byte first, low second.         
//------------------------------------------------------------------------------
uint32_t CRC::message(uint16_t msg)
{
    message(uint8_t(msg >> 8));
    return message(uint8_t(msg & 0x00FF));
}

//------------------------------------------------------------------------------
/// @brief  Accept a 32-bit value returning the resulting CRC value.            
/// @note   Effectively big endian; do the high short first, low second.        
//------------------------------------------------------------------------------
uint32_t CRC::message(uint32_t msg)
{
    message(uint16_t(msg >> 16));
    return message(uint16_t(msg & 0x0000FFFF));
}

//------------------------------------------------------------------------------
/// @brief  Query the algorithm for the current CRC value.                      
//------------------------------------------------------------------------------
uint32_t CRC::value() const
{
    uint32_t result;

    if (m_ReflectOut) {
        result = reflect(m_Register, m_Width);
    } else {
        result = m_Register;
    }

    return (result ^ m_XorOut);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void CRC::makeTable()
{
    for (int16_t i=0 ; i < 256 ; i ++)
    {
        m_Table[i] = tableCalc(i);
    }
}

//------------------------------------------------------------------------------
//          outmask exists for CRC widths less than 32 bits.  For 32 bits,      
//          its value should be 0xffffffff.                                     
//------------------------------------------------------------------------------
uint32_t CRC::tableCalc(uint16_t val)
{
    uint32_t result;
    uint32_t inval = val;
    uint32_t topbit = 1L << (m_Width-1);
    
    if (m_ReflectIn) {
        inval = reflect(inval, 8);
    }

    result = inval << (m_Width-8);
    for (int i=0 ; i < 8 ; i++)
    {
        if (result & topbit) {
            result = (result << 1L) ^ m_Poly;
        } else {
            result <<= 1;
        }
    }

    if (m_ReflectIn) {
        result = reflect(inval, m_Width);
    }

    return result;

} // uint32_t CRC::tableCalc(uint16_t val)                                      

} // namespace math                                                             
} // namespace lib                                                              
