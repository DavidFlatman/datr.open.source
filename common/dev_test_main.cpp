//------------------------------------------------------------------------------
///                                                                             
///@file dev_test_main.cpp                                                      
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
//------------------------------------------------------------------------------

#include "dev_test_work.h"
#include "dev_test_work_test.h"
#include "dev_debug.h"

#include <iostream>
#include <stdexcept>
#include <string>

//------------------------------------------------------------------------------
///                                                                             
///@page program_dev_test_work_test_main  Program: dev_test_work_test_main      
///                                                                             
///@brief  A main function for running the project's test code.                 
///                                                                             
///         The dev_test_work_test_main program runs the unit tests for the DATR
///         suite of programs.  It can optionally:                              
///             -   Run all tests or only for file (classes) specified.         
///             -   Display test summary information (test results).            
///             -   Display test statistics (summary + when begin).             
///             -   Failed tests.                                               
///             -   Passed tests.                                               
///                                                                             
///         The most useful command line appears to be:                         
///             % dev_test_work_test_main --summary --failed --slow             
///                                                                             
///@sa      dev_test_work_test_main.cpp                                         
///@sa      lib::test::classes::displayHelp()                                   
///@sa      lib::test::classes::displayHistory()                                
///                                                                             
///@section dtm_why Why This Unit Test Frame Work?                              
///     -   This unit test frame work was initially created based on            
///         Chuck Allison's "The Simplest Automated Test Framework That         
///         Could Possibly Work" delivered at the Software Developer's          
///         conference on April 12, 2001.                                       
///     -   The framework has evolved over time (so that it is probably not     
///         the simplest that can possibly work).                               
///     -   The framework is thread-safe -- at the object level.                
///     -   When combined with the Makefile files generated by the makemake     
///         utility, the framework can test only the classess that have been    
///         modified since the last build.  This speeds up the test process     
///         by only testing what has changed.                                   
///     -   Of course the previous step does not prevent one from running the   
///         entire test suite.                                                  
///     -   The tests can be segregated into different "test levels" which, at  
///         the time of this writing, are designed to put slower tests into     
///         higher test levels.                                                 
///                                                                             
///@section dtm_unit_vs_useful  Unit Test vs Useful Test                        
///         There was a debate at one point on what constituted "unit test"     
///         and the self-appointed guru at the time quoted someone that stated  
///         that unit tests could not do any disk I/O (too slow) and should     
///         mock nearly everything it could.                                    
///                                                                             
///         The counter to that was "do you want your definition of 'unit test' 
///         or do you want 'useful tests'?"  We're going with useful.           
///                                                                             
///         If your test needs to hit the disk (e.g., try testing a file reader 
///         without hitting I/O), hit the disk.  If running the test is going   
///         to take too long, put it into a higher runTest level.               
///                                                                             
///@section dtm_how_to_code How To Code                                         
///         The best advice is to use the maketest utility to automatically     
///         generate the skeleton test code from the .h file.                   
///                                                                             
///         If you're going to generate the test classes by hand, the key       
///         points you need to cover are:                                       
///             -   The test class inherits from the dev::Test class.           
///             -   The test class has to implement the runTest method          
///             -   The cpp file needs to use the TEST_REGISTER macro to        
///                 register an instance of the test class (this is used        
///                 to automagically "know" what test classes are available).   
///             -   The individual tests conducted by the test class need to    
///                 call conditionalTest to register a test and the results.    
///                 This is generally done via the TEST macro that is           
///                 defined in dev_test_work_test.h.                            
///                                                                             
///         Things the coder should look to when coding tests:                  
///             -   External files:                                             
///                     -   Use lib::config::unitTestDataFilePath() to get the  
///                         location of external unit test data files.          
///                     -   Test that the file exists before using it:          
///                                                                             
///                         @code                                               
///                             TEST(lib::file::Info::exists(path));            
///                             if (lib::file::Info::exists(path)) {            
///                                 // tests with file                          
///                             }                                               
///                         @endcode                                            
///                                                                             
///@section dtm_cmdlin  Command Line                                            
///     The following command line qualifiers are interpretted by the           
///     dev_test_work_test_main program:                                        
///         -   --failed        displays the failed tests (with file and line   
///                             number).                                        
///         -   --passed        displays the passed tests (with file and line #)
///         -   --stats         display statistics regarding number of tests    
///                             failed and number of tests passed.              
///         -   --files f1 f2   Execute the tests associated with the test      
///                             file f1, f2, ...                                
///                                                                             
///@section dtm_sample  Sample Usage                                            
///     @code                                                                   
///         % dev_test_work_test_main --stats --files lib_stringtest lib_configfiletest
///     @endcode                                                                
///                                                                             
///@version 2020-05-04  DHF     Open sourced.                                   
///                             Moved dev::test::classes to dev::test::work.    
///                             Added handling of unknown exception.            
///                                                                             
//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    try {
        dev::debug::initialize(argc, argv);
        dev::test::work::run(argc, argv);

    } catch(std::exception const& err) 
    {
        std::cout << "std::exception thrown: " << err.what() << std::endl;
    } catch(std::string const& err)
    {
        std::cout << "std::string exception thrown: " << err << std::endl;
    } catch(...)
    {
        std::cout << "unknown exception thrown" << std::endl;
        throw;
    }

}

