//------------------------------------------------------------------------------
///@file lib_file_info_pathseparator.h                                          
///                                                                             
///                                                                             
///@par Classification: unclassified, OPEN SOURCE                                                                                        
///                                                                             
///                                                                             
///@brief   Help method for lib_file_info.h.                                    
///@version 2020-03-09  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///@brief   Return the expected path separator for this operating system.       
///                                                                             
///@warning This file is expected to @e only be included from within            
///         lib_file_info.h                                                     
//------------------------------------------------------------------------------
inline static const std::string pathSeparator()
{
    #ifdef IS_WINDOWS
    return "\\";
    #else
    return "/";
    #endif
}
