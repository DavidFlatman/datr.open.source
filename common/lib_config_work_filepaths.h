//------------------------------------------------------------------------------
///                                                                             
///@file lib_config_work_filepaths.h                                            
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   This file holds compile-time pathing information for external       
///         entities.  For example, want to know where external unit test       
///         data should be found, look in g_UnitTestDataFilePath.               
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2013-02-08  DHF     File creation based on discussions with AGB.    
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_CONFIG_WORK_FILEPATHS_H_FILE_GUARD
#define LIB_CONFIG_WORK_FILEPATHS_H_FILE_GUARD

#include <string>

namespace lib {
namespace config {
namespace work {

std::string systemConfigurationFilePath(const std::string& filename);
std::string userConfigurationFilePath(const std::string& filename);
std::string unitTestDataFilePath(const std::string& filename);
std::string unitTestTemporaryFilePath(const std::string& filename);

} // namespace work
} // namespace config
} // namespace lib

#endif // #ifndef LIB_CONFIG_WORK_FILEPATHS_H_FILE_GUARD
