//------------------------------------------------------------------------------
///@file dev_test_classes.h                                                     
///                                                                             
///@brief Holds the declaration of support routines for the dev::Test           
///       environment.                                                          
///                                                                             
///@par Classification: unclassified, OPEN SOURCE                                                                                        
///                                                                             
///@section test-classes-usage Usage                                            
///         The way that I would use this file falls into two parts:            
///             @li Include the file into the cpp files of test classes derived 
///                 from dev::Test and insert a                                 
///                 @code                                                       
///                     TEST_REGISTER(fully::qualified:test::class::Name);      
///                 @endcode                                                    
///                 in the .cpp file.                                           
///             @li Create a test harness class that consists of the following  
///                 (yes, this has been tested):                                
///                 @code                                                       
///                     #include "dev_test_classes.h"                           
///                                                                             
///                     int main(int argc, char* argv[])                        
///                     {                                                       
///                         dev::test::classes::run(argc, argv);                
///                     }                                                       
///                 @endcode                                                    
///                                                                             
///@section test-classes-wtf  What Is Going On?                                 
///         If you're just looking to use the dev::test::classes, see Usage     
///         above.  This section is in case you want to know how it works (or   
///         how to maintain it if it doesn't work).                             
///\n\n                                                                         
///@subsection test-classes-wtf-test-register   TEST_REGISTER                   
///         The TEST_REGISTER macro expands out into the following code:        
///         @code                                                               
///         static bool testClassRegistered(                                    
///            dev::test::classes::registerTestObject(new testClass, __FILE__)  
///         )                                                                   
///         @endcode                                                            
///         What this does is declare a compilation unit private, bool variable 
///         testClassRegistered that is global to the compilation unit (test    
///         class) and is therefore (being a global variable) initialized before
///         the program's main routine gets control -- which means that         
///         dev::test::classes::registerTestObject gets called before main      
///         routine gets control.                                               
///\n\n                                                                         
///         What registerTestObject does is save off:                           
///             @li A pointer to the test class.                                
///             @li The "root" of the test class' filename where "root" is      
///                 defined as the filename minus the directory, extension, and 
///                 "_test" (e.g., "dir/file_test.cpp" becomes "file").         
///             @li The number of test classes saved off before the current one 
///                 (effectively giving each test class a unique number between 
///                 zero and the number of test classes registered minus one.)  
///                                                                             
///@subsection test-classes-wtf-test-execution  Exeuction Order                 
///         Again, the order of execution is:                                   
///             @li bool testClassRegistered needs to be initialized, therefore 
///             @li dev::test::classes::regsiterTestObject gets called in       
///                 a "random" order compared to other global variables being   
///                 initialized (yeah, it's deterministic, don't be a sadist).  
///             @li dev::test::classes::regsiterTestObject ensures that the     
///                 global singleton containing pointers to the newly minted    
///                 dev::Test objects is initialized and holds the pointers.    
///             @li After all global variables are initialized, main gains      
///                 control.                                                    
///             @li When main surrenders control (returns gracefully), the      
///                 singleton is destroyed which causes the dev::Test objects   
///                 to be deleted.  <i>Note:  If main is not graceful about its 
///                 demise, then the objects are not deleted.</i>               
///                                                                             
///         An exercise for the reader:                                         
///             @li In dev::test::classes::registerTestObject, insert a DEBUG   
///                 statement announcing that registerTestObject is being called
///                 (and output the filename parameter for amusement value).    
///             @li In either Cleanup::~Cleanup or in Singleton::destory add    
///                 a DEBUG statement announcing that the routine is being      
///                 executed at the beginning of the function (and one at the   
///                 end for good measure).                                      
///             @li In the main test program, add a DEBUG line at the start of  
///                 main and another one just before main terminates.           
///             @li Optionally modify dev::Test::Test and dev::Test::~Test      
///                 to annouce when they are executed.                          
///             @li Compile and run                                             
///                                                                             
///         You should see the debug statement in the following order:          
///             @li dev::Test (parameter passed to registerTestObject)          
///             @li dev::test::classes::registerTestObject                      
///             @li main begining                                               
///             @li main ending                                                 
///             @li Cleanup::~Cleanup (or Singleton::destroy) beginning         
///             @li dev::Test::~Test                                            
///             @li Cleanup::~Cleanup (or Singleton::destroy) ending            
///                                                                             
//------------------------------------------------------------------------------
#ifndef DEV_TEST_CLASSES_H
#define DEV_TEST_CLASSES_H

#include "dev_test.h"
#include "lib_config_filepaths.h"
#include "lib_string.h"

#include <boost/filesystem.hpp>
#include <string>
#include <stdint.h>

namespace dev {

//------------------------------------------------------------------------------
///@brief   Namespace containing the test frame work code.                      
//------------------------------------------------------------------------------
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Library: Classes                                                        
///                                                                             
///@par Purpose:                                                                
///                                                                             
///@par Thread Safety:  none                                                    
///         Any thread safety issues must be coordinated by outside code.       
///                                                                             
///                                                                             
///@version 2020-02-28  MNT/KCG Added lib_string.h include.                     
///                                                                             
///@version 2018-03-06  DHF     Updated to include not implemented tests.       
///                                                                             
///@version 2017-05-16  MNT     Renamed FILESTEM FILENAME_ROOT                  
///                                                                             
///@version 2017-05-12  MNT     Added FILENAME, FILESTEM, and TEMP_FILENAME     
///                                                                             
///@version 2010-03-02  DHF     Changed outer namespace to dev.                 
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2008-03-24  DHF     File creation                                   
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///@brief   Support classes for the test framework.                             
//------------------------------------------------------------------------------
namespace classes {

//------------------------------------------------------------------------------
///@brief A dev::Test::OutputFunction to send results to std::cout.  Designed   
///       to be a parameter to dev::test::classes::run.                         
///                                                                             
///@param message       The message to send to std::cout, this is called by the 
///                     dev::Test class (and probably not by anyone else).      
//------------------------------------------------------------------------------
void cout(const std::string& message);

//------------------------------------------------------------------------------
///@brief A dev::Test::OutputFunction to send results to std::cerr.  Designed   
///       to be a parameter to dev::test::classes::run.                         
///                                                                             
///@param message       The message to send to std::cout, this is called by the 
///                     dev::Test class (and probably not by anyone else).      
//------------------------------------------------------------------------------
void cerr(const std::string& message);

//------------------------------------------------------------------------------
///@brief  Register the object so that they can be dev::test::classes::run.     
///                                                                             
///@param testobject    A pointer to dynamically created dev::Test object. The  
///                     pointer is owned by the dev::test::classes routines     
///                     for eventual destruction.                               
///                                                                             
///@param filename      The C++ file that the test object is defined in, used   
///                     to determine whether the test should be executed if the 
///                     --files or -f is specified in the run/argc & argv params
//------------------------------------------------------------------------------
bool registerTestObject(
    dev::Test* testobject
  , const std::string& filename
);

//------------------------------------------------------------------------------
///@brief  Parse the command (argc,argv) and execute the tests piping the output
///        to the given outputFunc.                                             
///                                                                             
///@param argc  The argument count, number of arguments in argv.                
///                                                                             
///@param argv  The argument vector, main things looking for here are the -f    
///             or --files argument followed by files that have been changed    
///             and should be tested.  Only the root of the filename is         
///             considered with any trailing "_test" stripped off.              
///                                                                             
///@param outputFunc    Pointer to the output function to send messages from    
///                     the test to.  By default, a std::cout function is used. 
///                                                                             
///@param indentLevel   Used to force the indentation level, can be used to     
///                     prepend text to output messages.                        
///                                                                             
///@param firstTest     Index to the first test to execute.                     
///                                                                             
///@param lastTest      Index past the last test to execute.                    
//------------------------------------------------------------------------------
void run(
    int argc
  , char** argv
  , dev::Test::OutputFunction outputFunc = cout
  , const std::string& indentLevel = ""
  , int firstTest = -1
  , int lastTest = -1
);

//------------------------------------------------------------------------------
/// Return the number of tests registered.                                      
//------------------------------------------------------------------------------
int64_t registeredCount();

int64_t passed();
int64_t failed();
int64_t notImplemented();

//------------------------------------------------------------------------------
///@brief Returns a string representing the name of the calling file.           
///                                                                             
///@par Expected Usage                                                          
///     @code                                                                   
///         TEST(FILENAME == "lib_wtf_muxmon_work_filewritertest.cpp")          
///     @endcode                                                                
///                                                                             
///@note __FILE__ on the other hand returns e.g.                                
///      "../common/lib_wtf_muxmon_work_filewritertest.cpp"                     
//------------------------------------------------------------------------------
#define FILENAME boost::filesystem::path(__FILE__).filename().string()

//------------------------------------------------------------------------------
///@brief Returns a string representing the stem/root of calling file's name.   
///                                                                             
///@par Expected Usage                                                          
///     @code                                                                   
///         TEST(FILENAME_ROOT == "lib_wtf_muxmon_work_filewritertest")         
///     @endcode                                                                
///                                                                             
///@note __FILE__ on the other hand returns e.g.                                
///      "../common/lib_wtf_muxmon_work_filewritertest.cpp"                     
//------------------------------------------------------------------------------
#define FILENAME_ROOT boost::filesystem::path(__FILE__).stem().string()

//------------------------------------------------------------------------------
///@brief Returns a file path intended to be used for temporary unit test files.
///                                                                             
///@par Expected Usage                                                          
///     @code                                                                   
///         lib::work::File file(TEMP_FILENAME, "wb");                          
///             <write to file>                                                 
///         file.remove();                                                      
///     @endcode                                                                
//------------------------------------------------------------------------------
#define TEMP_FILENAME lib::config::unitTestTemporaryFilePath(               \
    lib::format("%s-line-%d", FILENAME_ROOT.c_str(), __LINE__)              \
)

//------------------------------------------------------------------------------
///@brief Macro for registering a test class derived from dev::Test for         
///       automatic execution by dev::test::classes::run.                       
///@param testClass     The fully qualified class name of the test class.       
//------------------------------------------------------------------------------
#define TEST_REGISTER(testClass)                                            \
    static bool testClassRegistered(                                        \
        dev::test::classes::registerTestObject(new testClass, __FILE__)     \
    )

}  // namespace classes //
}  // namespace test //
}  // namespace dev //

#endif // #ifndef DEV_TEST_CLASSES_H //
