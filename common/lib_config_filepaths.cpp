//------------------------------------------------------------------------------
///                                                                             
///@file lib_config_filepaths.cpp                                               
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                                                                      
///                                                                             
///                                                                             
///@version 2013-12-11  AGB     Added / modiifed WIN32 pathes.                  
///                                                                             
///@version 2013-02-08  DHF     File creation based on discussions with AGB.    
///                                                                             
//------------------------------------------------------------------------------

#include "lib_config_filepaths.h"
#include <stdlib.h>                     // getenv   

namespace lib {
namespace config {

static const char* env(const char* var) {
    static const char* emtpy = "";
    return getenv(var) ? getenv(var) : emtpy;
}

inline std::string ENV(const char* var) { return std::string(env(var)); }

//------------------------------------------------------------------------------
//  If the o_ values are defined, they override the default values. This allows 
//  the user to override the application defaults.                              
//------------------------------------------------------------------------------
static const std::string    o_sys   (ENV("DATR_SYSTEM"));
static const std::string    o_user  (ENV("DATR_USER"));
static const std::string    o_test  (ENV("DATR_UNITTEST"));
static const std::string    o_temp  (ENV("DATR_UNITTEMP"));

//------------------------------------------------------------------------------
//  The d_ values are default values for the DATR system and are used if the    
//  DATR_* environment variables are not set.                                   
//------------------------------------------------------------------------------
#ifdef _WIN32

// g_SystemPath ... use ALLUSERS's APPDATA
//                  use DATR_SYSTEM     
//                  use DATR_USER       
//                  use DATR_UNITTEST   
//                  use DATR_UNITTEMP   
#if 0
static const std::string    e_sys(
    ENV("ALLUSERSPROFILE") 
  + ENV("APPDATA").substr(ENV("APPDATA").rfind("\\"))
  + "\\"
);
#endif

    static const std::string    d_sys (ENV("ALLUSERSPROFILE") + "\\datr\\");
    static const std::string    d_user(ENV("APPDATA") + "\\datr\\");
    static const std::string    d_test(ENV("HOMEDRIVE") + "\\data\\unit-test\\");
    static const std::string    d_temp(ENV("TEMP") + "\\.datr\\");

#else

    static const std::string    d_sys ("/opt/datr/etc/");
    static const std::string    d_test("/data/unit-test/");
    static const std::string    d_temp("/tmp/");
    static const std::string    d_user(ENV("HOME") + "/.datr/");

#endif

//------------------------------------------------------------------------------
//  The g_ values are set based on whether the overriding DATR environmental    
//  values are set or the default values should be used.                        
//------------------------------------------------------------------------------
static const std::string    g_SystemPath(o_sys.empty()    ? d_sys  : o_sys);
static const std::string    g_UserPath(o_temp.empty()     ? d_user : o_user);
static const std::string    g_UnitTestPath(o_test.empty() ? d_test : o_test);
static const std::string    g_UnitTempPath(o_temp.empty() ? d_temp : o_temp);

//------------------------------------------------------------------------------
///@brief   Return the full path for the system configuration file filename.    
///@param   filename    A string with the relative path of the system           
///                     configuration file whose full path is desired.          
///                     E.g., aircraft_config.xml  will return the appropriate  
///                     full path, e.g., /opt/datr/etc/aircraft_config.xml      
//------------------------------------------------------------------------------
std::string systemConfigurationFilePath(const std::string& filename)
{
    return g_SystemPath + filename;
}

//------------------------------------------------------------------------------
///@brief   Return the full path for the given unit test file.                  
///@param   filename    The relative path to the unit test file that whose      
///                     fullpath is desired.  E.g., irig106/ch10/big-file.ch10  
//------------------------------------------------------------------------------
std::string unitTestDataFilePath(const std::string& filename)
{
    return g_UnitTestPath + filename;
}

//------------------------------------------------------------------------------
///@brief   Return the full path for the given temporary test file name.        
///                                                                             
///         When a unit test needs to create a temporary file, this function    
///         should be used to translate from a relative path to an absolute     
///         path.                                                               
///                                                                             
///@param   filename    The relative path whose full path is desired.           
///                                                                             
//------------------------------------------------------------------------------
std::string unitTestTemporaryFilePath(const std::string& filename)
{
    return g_UnitTempPath + filename;
}

//------------------------------------------------------------------------------
///@brief   Return the full path for the given user specific configuration file.
///                                                                             
///         userConfigurationFilePath is used to translate realative paths of   
///         user specific configuration file to absolute paths.                 
///                                                                             
///@param   filename    The relative path to the user configuration file whose  
///                     absolute path is desired.                               
//------------------------------------------------------------------------------
std::string userConfigurationFilePath(const std::string& filename)
{
    return g_UserPath + filename;
}

} // namespace config
} // namespace lib

