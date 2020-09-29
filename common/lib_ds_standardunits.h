//------------------------------------------------------------------------------
///@file lib_ds_standardunits.h                                                 
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_DS_STANDARDUNITS_H_FILE_GUARD
#define LIB_DS_STANDARDUNITS_H_FILE_GUARD

#include "lib_ds_enum.h"

#include <stddef.h>                     // size_t


namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: StandardUnits                                                    
///                                                                             
///@brief   Define stanard units (bits, bytes, words, ...).                     
///                                                                             
///@par Thread Safety:  special                                                 
///         @see ENUM macro in lib_ds_enum.h                                    
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-09-14  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
ENUM(
    StandardUnits
  , Bits
  , Bytes
  , Words
  , DoubleWords
  , DWORD = DoubleWords
  , QuadWords
  , QWORD = QuadWords
);

size_t sizeInBits(StandardUnits su, size_t count = 1);

size_t sizeInBytes(StandardUnits su, size_t count = 1);

} // namespace ds
} // namespace lib


#endif // #ifndef LIB_DS_STANDARDUNITS_H_FILE_GUARD

