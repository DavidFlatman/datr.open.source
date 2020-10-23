//------------------------------------------------------------------------------
///@file lib_math_crc.h                                                         
///@par Classification:  UNCLASSIFIED - OPEN SOURCE                                          
//------------------------------------------------------------------------------
#ifndef LIB_MATH_CRC_H
#define LIB_MATH_CRC_H

#include <stdint.h>

namespace lib {
namespace math {

//------------------------------------------------------------------------------
///                                                                             
///@class CRC                                                                   
///                                                                             
///@brief   Class that makes a running computation of a Cyclic Redundancy Check 
///         (CRC) checksum.                                                     
///@par Purpose:                                                                
///         The class will perform a table-driven cyclic redundancy check of a  
///         message of indefinite size using a user-provided generator          
///         polynomial.                                                         
///                                                                             
///@par Usage:                                                                  
///         Simply instantiate an object using one of the two constructors.     
///         Then feed the class the data block using one of the Message methods.
///         The return value of the method is the current CRC value.  In        
///         addition, at any point in the process, you can query the CRC result 
///         without affecting   the running computation using the GetCRC method.
///         To reinitialize the object, call the reset method.                  
///                                                                             
///@par Attribution:                                                            
///         The algorithm is based almost entirely on the article "A Painless   
///         Guide to CRC Error Detection Algorithms" by Ross Williams           
///         (http://www.repairfaq.org/filipg/LINK/F_crc_v3.html)                
///                                                                             
///@version                                                                     
///     25-May-2007     AGB     v1.00 - Original Incarnation                    
///                                                                             
///@version                                                                     
///     2010-02-17      DHF     Made to conform with lib coding standard;       
///                             mainly commenting style, naming style, and      
///                             use of stdint.h                                 
///@version
///     2020-10-23      PN      Open Source                                                                             
//------------------------------------------------------------------------------
class CRC 
{
    public:
        CRC ( 
            const uint32_t poly = 0x04c11db7
          , const uint32_t init = 0xffffffff
          , const uint32_t xorout = 0xffffffff
        );

        CRC ( 
            const uint32_t poly
          , const uint32_t init
          , const uint32_t xorout
          , const uint16_t width
          , bool reflectin = false
          , bool reflectout = false
        );

        void reset();

        uint32_t message(uint8_t msg);
        uint32_t message(uint16_t msg);
        uint32_t message(uint32_t msg);

//        uint32_t message(const uint16_t* pMsg, const uint32_t size);          
//        uint32_t message(const uint32_t* pMsg, const uint32_t size);          

        template <typename TYPE>
        uint32_t message(const TYPE* msg, uint32_t size)
        {
            uint32_t result = 0;
            for (uint32_t i=0; i < size; ++i) {
                result = message(msg[i]);
            }
            return result;
        }

        uint32_t value() const;
        operator uint32_t() const {return value();} 

    protected:
        void makeTable();
        uint32_t tableCalc(uint16_t val);

        uint32_t m_Table[256];  // I don't think this will ever change          

        uint32_t m_Register;
        uint32_t m_Poly;
        uint32_t m_Init;
        uint32_t m_XorOut;
        uint16_t m_Width;
        bool m_ReflectIn;
        bool m_ReflectOut;

}; // class CRC                                                                 

} // namespace math                                                             
} // namespace lib                                                              

#endif // #ifndef LIB_MATH_CRC_H                                                
