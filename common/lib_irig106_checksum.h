//------------------------------------------------------------------------------
///                                                                             
///@file lib_irig106_checksum.h                                                 
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@brief Holds the definition of the lib::irig106::checksum function template  
///                                                                             
//------------------------------------------------------------------------------
#ifndef LIB_IRIG106_CHECKSUM_H
#define LIB_IRIG106_CHECKSUM_H

#include <stdint.h>

namespace lib {
namespace irig106 {

//------------------------------------------------------------------------------
///                                                                             
///@par Function Template: checksum                                             
///                                                                             
///@par Purpose:                                                                
///                                                                             
///@par Thread Safety:  unknown none class object                               
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-09-01  DHF     File creation                                   
//------------------------------------------------------------------------------
template <typename TYPE>
TYPE checksum(const TYPE* data, uint64_t length)
{
    TYPE result = 0;

    for (uint64_t i=0; i < length; ++i) {
        result += data[i];
    }

    return result;

} // TYPE Checksum(TYPE* data, uint64_t length) //

}  // namespace irig106 //
}  // namespace lib //

#endif // #ifndef LIB_IRIG106_CHECKSUM_H //
