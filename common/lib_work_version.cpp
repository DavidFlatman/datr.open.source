//------------------------------------------------------------------------------
///                                                                             
///@file lib_work_version.cpp                                                   
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Hold the versioning information for the datr project.               
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-11-19  DHF     Changed VERSION_RELEASE_MONTH to const char*    
///                             to allow "-beta" in the month string.           
//------------------------------------------------------------------------------

#include "lib_work_version.h"
#include "lib_string.h"


//------------------------------------------------------------------------------
//  Major releases to DATR library:                                             
///@version 2015-07-20  DHF                                                     
///     -   InstrumentationTime replaces DateTime for lib::ms1553::work::Message
///     -   "extra" removed from unit-test directory structure(s).              
///                                                                             
///@version 2013-08-28  DHF                                                     
///     -   The lib::msg::Publisher and lib::msg::Subscriber were modified to   
///         automatically publish boost::shared_ptr<const TYPE> of the TYPE's   
///         passed.  Specifically, the ability to publish items for modification
///         (non const) was removed.                                            
///         @code                                                               
///             class MyPublisher : public lib::msg::Publisher<ConstMyTypePtr>  
///             {                                                               
///                 ...                                                         
///             };                                                              
///         // becomes                                                          
///             class MyPublisher : public lib::msg::Publisher<MyType>          
///             {                                                               
///                 ...                                                         
///             };                                                              
///         @endcode                                                            
//------------------------------------------------------------------------------

namespace lib {
namespace work {

//------------------------------------------------------------------------------
/// The VERSION_MAJOR should never increment; incrmenting the major version     
/// requires DADMS approval.                                                    
//------------------------------------------------------------------------------
const uint32_t  VERSION_MAJOR(3);

//------------------------------------------------------------------------------
/// The VERSION_MINOR should be incremented after major functionality is planned
/// or a data persistents format (database structure) is changed that breaks    
/// backward compatibility.                                                     
//------------------------------------------------------------------------------
const uint32_t  VERSION_MINOR(1);

//------------------------------------------------------------------------------
/// Increment the VERSION_DOT after a release is put into production (represents
/// next release).                                                              
//------------------------------------------------------------------------------
const uint32_t  VERSION_DOT(0);

const uint32_t  VERSION_BUILD(20200908);

const uint32_t  VERSION_RELEASE_YEAR(20);
const char*     VERSION_RELEASE_MONTH("09-RC.1");

std::string version_build()
{
    return format(
        "v%d.%d.%d.%d-%02d.%s"
      ,  VERSION_MAJOR
      ,  VERSION_MINOR
      ,  VERSION_DOT
      ,  VERSION_BUILD
      ,  VERSION_RELEASE_YEAR
      ,  VERSION_RELEASE_MONTH
    );
}

std::string version()
{
    return version_build() + " built on " __DATE__ " " __TIME__;
}

} // namespace work //
} // namespace lib  //
