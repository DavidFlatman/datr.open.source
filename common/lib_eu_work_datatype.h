//------------------------------------------------------------------------------
///@file lib_eu_work_datatype.h                                                 
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_EU_WORK_DATATYPE_H_FILE_GUARD
#define LIB_EU_WORK_DATATYPE_H_FILE_GUARD

#include "lib_ds_enum.h"


namespace lib {
namespace eu {
namespace work {


//------------------------------------------------------------------------------
///                                                                             
///@par Class: DataType                                                         
///                                                                             
///@brief   The data type is an integer, fixed, string, other ...               
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@version 2020-09-01  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
ENUM(DataType, Fixed, String, Integer, Other);


} // namespace work
} // namespace eu
} // namespace lib


#endif // #ifndef LIB_EU_WORK_DATATYPE_H_FILE_GUARD

