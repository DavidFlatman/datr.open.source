//------------------------------------------------------------------------------
///@file lib_ds_offset.h                                                        
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_DS_OFFSET_H_FILE_GUARD
#define LIB_DS_OFFSET_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"

#include <stdint.h>

namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Offset                                                           
///                                                                             
///@brief   A file offset ... based class for other classes might be handy.     
///                                                                             
///@par Thread Safety:  unknown none class object                               
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Removed boost::scoped_ptr                       
///                                                                             
///@version 2020-02-03  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                             Changed typedef to using                        
///                                                                             
///@version 2014-08-07  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
struct Offset
{
    Offset(uint64_t offset = 0) : m_FileOffset(offset) { }
    Offset(const Offset& o) = default;
    uint64_t    m_FileOffset;

}; // class Offset //

using OffsetPtr = lib::ds::shared_ptr<Offset>;
using ConstOffsetPtr = lib::ds::shared_ptr<const Offset>;

} // namespace ds
} // namespace lib

#endif // #ifndef LIB_DS_OFFSET_H_FILE_GUARD

