//------------------------------------------------------------------------------
///@file dev_test_work_test.cpp                                                 
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief Holds the definition of the dev::test class; the basis for various    
///       @e useful test classes.                                               
///                                                                             
///@note    Since this file is core to the developer test environment, we're    
///         deliberately avoiding using any lib_ files or routines.  We want    
///         the dev_test_work_test* to stand on its own.                        
///                                                                             
///@version 2020-09-08  DHF     Removed dependency on ftime.                    
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2018-03-06  DHF     Added handling not implemented.                 
///                                                                             
///@version 2015-10-27  DHF     Changed the output format to line up failed     
///                             and passed values.                              
///                                                                             
///@version 2015-08-31  DHF     Added className() to error message if an        
///                             exception is thrown by the unit-tests.          
///                                                                             
///@version 2015-03-05  DHF     Added color coding if an exception is thrown.   
///                                                                             
///@versoin 2014-12-16  DHF     Added try/catch around run-levels due to some   
///                             tests throwing exceptions.                      
///                                                                             
///@version 2014-03-21  DHF     Changed numeric outputs to have a comma.        
///                                                                             
///@version 2014-02-11  DHF     Made sure that the test is only run once per    
///                             image (the main driver is now checking for both 
///                             modifications to the test class and the class   
///                             under test.  If both changed it will attempt to 
///                             run the test twice.)                            
///                                                                             
///@version 2014-01-22  DHF     Modification for Boost v1.54                    
///                                                                             
///@version 2013-02-20  DHF     Changed type of conditionalTest's "test"        
///                             parameter to std::string.                       
///                                                                             
///@version 2010-03-02  DHF     Changed outer namespace from lib to dev.        
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2007-??-??  DHF     Made compatible with range division C++         
///                             standards.                                      
///                                                                             
///@version 2005-10-10  DHF     Moved to drs::lib                               
///                             Goal: Platform indenpence.                      
///                                                                             
///@version 2001-04-21  DHF     Corrected namespace to itis::lib                
///                                                                             
///@version 2001-04-13  DHF     File created from notes on Chuck Allison's      
///                             "The Simplest Automated Unit Test Framework     
///                             That Could Possibly Work", SD Conference        
///                             April 12, 2001.                                 
///                                                                             
///@version 2001-04-13  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------

#include "dev_test_work_test.h"
#include "lib_compiler_info.h"
#include "lib_string.h"

#include <chrono>
#include <ctime>
#include <stdio.h>      // sprintf  
#include <string.h>     // strncpy   

#include "debug.h"

#ifdef IS_LINUX
    #include "lib_time_work_walltime.h"
    #define  FONT_RED       "\x1B[1;31m"
    #define  FONT_ORANGE    "\x1B[38;2;196;128;1m"
    #define  FONT_NORMAL    "\x1B[0m"
#else
    #define  FONT_RED       ""
    #define  FONT_ORANGE    ""
    #define  FONT_NORMAL    ""
#endif

//------------------------------------------------------------------------------
///@brief   <b>dev</b>elopment namespace; code not in final product.            
//------------------------------------------------------------------------------
namespace dev {
namespace test {
namespace work {

const Test::verbosity_t Test::vSilent           (0x00);
const Test::verbosity_t Test::vFailed           (0x01);
const Test::verbosity_t Test::vHeader           (0x02);
const Test::verbosity_t Test::vPassed           (0x04);
const Test::verbosity_t Test::vSummary          (0x08);
const Test::verbosity_t Test::vTimeStamp        (0x10);
const Test::verbosity_t Test::vRunLevelTime     (0x20);
const Test::verbosity_t Test::vRunLevelTimeLong (0x40);
const Test::verbosity_t Test::vNotImplemented   (0x80);
const Test::verbosity_t Test::vVerbose          (0xff);

const Test::verbosity_t Test::vStatistics(vHeader | vSummary | vTimeStamp);

///-----------------------------------------------------------------------------
///@par  Method:  Test                                                          
///                                                                             
///@par  Purpose:                                                               
///         Construct the test class.                                           
///                                                                             
///@param   className   The name of the class being tested.  If the class       
///                     derived from Test is designed to test lib::log::Output  
///                     then pass in "lib::log::Output".                        
///-----------------------------------------------------------------------------
Test::Test (
    const std::string& className
) 
  : m_OutputFunction(NULL)
  , m_OutputIndentation("")
  , m_TestClass(this)
  , m_ClassName(className)
  , m_Failed(0)
  , m_Passed(0)
  , m_NotImplemented(0)
  , m_Verbosity(vSilent)
  , m_AlreadyExecuted(false)
{
    
} /// Test::Test ///


///-----------------------------------------------------------------------------
///@par  Method:  Test                                                          
///                                                                             
///@par  Purpose:                                                               
///                                                                             
///-----------------------------------------------------------------------------
Test::Test(
    const Test& that
) 
  : m_OutputFunction(that.m_OutputFunction)
  , m_OutputIndentation(that.m_OutputIndentation)
  , m_TestClass(this)
  , m_ClassName(that.m_ClassName)
  , m_Failed((int32_t)that.m_Failed)
  , m_Passed((int32_t)that.m_Passed)
  , m_NotImplemented((int32_t)that.m_Passed)
  , m_Verbosity(that.m_Verbosity)
  , m_AlreadyExecuted(that.m_AlreadyExecuted)
{
    
} /// Test::Test(const Test& that)  ///


///-----------------------------------------------------------------------------
///-----------------------------------------------------------------------------
Test& Test::operator=(const Test& that)
{
    m_ClassName = that.m_ClassName;
    m_Failed    = (int32_t) that.m_Failed;
    m_Passed    = (int32_t) that.m_Passed;
    m_NotImplemented = (int32_t) that.m_NotImplemented;
    m_Verbosity = that.m_Verbosity;
    m_AlreadyExecuted = that.m_AlreadyExecuted;

    return *this;
}


///-----------------------------------------------------------------------------
///@par  Method:  ~Test                                                         
///                                                                             
///@par  Purpose:                                                               
///                                                                             
///-----------------------------------------------------------------------------
Test::~Test() {
    
}

///-----------------------------------------------------------------------------
///@par  Method:  string className                                              
///                                                                             
///@par  Purpose:                                                               
///                                                                             
///-----------------------------------------------------------------------------
std::string Test::className() const {
   return m_ClassName;
} 

///-----------------------------------------------------------------------------
///@par  Method:  run                                                           
///                                                                             
///@par  Purpose:                                                               
///                                                                             
///-----------------------------------------------------------------------------
void Test::run(
    verbosity_t verbosity
  , OutputFunction fun
  , const std::string& indentLevel
  , int testLevel
)  {
    if (m_AlreadyExecuted) return;
    m_AlreadyExecuted = true;

    m_Verbosity = verbosity;
    m_OutputFunction = fun;
    m_OutputIndentation = indentLevel;
    m_StartMilliseconds = 0;
    
    output(vHeader, "starting class " + m_ClassName);
    outputIndent();

#ifdef IS_LINUX
    lib::time::work::WallTime wallTime;

    std::string red(lib::format("%c[38;2;255;1;1m",27));
    std::string normal(lib::format("%c[0m",27));
    std::string classRunLevel;

    #define RUN_LEVEL(LEVEL, RUN_TEST, MAX_TIME)                            \
        try {                                                               \
            classRunLevel = lib::format(                                    \
                " in %s (run-level %d)"                                     \
              , className().c_str()                                         \
              , LEVEL                                                       \
            );                                                              \
            if (testLevel >= LEVEL) {                                       \
                lib::time::work::WallTime wallTime;                               \
                RUN_TEST();                                                 \
                double seconds = wallTime.seconds();                        \
                verbosity_t verbosity(vRunLevelTime);                       \
                std::string line(m_ClassName + "::runLevel");               \
                line += #LEVEL;                                             \
                if (seconds > MAX_TIME) {                                   \
                    line += FONT_RED;                                       \
                    verbosity |= vRunLevelTimeLong;                         \
                }                                                           \
                line += lib::format(" %lf seconds", seconds);               \
                output(verbosity, line + FONT_NORMAL);                      \
            }                                                               \
        } catch(const std::exception& e) {                                  \
            FAIL(                                                           \
                red                                                         \
              + std::string("std::exception thrown:  ") + e.what()          \
              + normal                                                      \
              + classRunLevel                                               \
            );                                                              \
        } catch(...) {                                                      \
            FAIL(                                                           \
                red                                                         \
              + "unknown exception thrown"                                  \
              + normal                                                      \
              + classRunLevel                                               \
            );                                                              \
        }
#else
    #define RUN_LEVEL(LEVEL, RUN_TEST, MAX_TIME)                            \
        if (testLevel >= LEVEL) RUN_TEST();

#endif

    //--------------------------------------------------------------------------
    //  Note:   The realtive times before we flag a test as being "slow" are    
    //          completely arbitrary.                                           
    //--------------------------------------------------------------------------
    RUN_LEVEL(0, runTest , 0.1);
    RUN_LEVEL(1, runTest1, 0.2);
    RUN_LEVEL(2, runTest2, 0.5);
    RUN_LEVEL(3, runTest3, 1.0);
    RUN_LEVEL(4, runTest4, 5.0);
    RUN_LEVEL(5, runTest5, 999990999);

    outputUnindent();

    #if 0
    std::string outputLine("class " + m_ClassName);

    if ((m_Verbosity & vFailed) || (m_Verbosity & vSummary)) {

        if (m_Failed == 0) {
            outputLine += "; failed " + lib::comma((int32_t) m_Failed);
        } else {
            outputLine += 
                "; failed " FONT_RED 
              + lib::comma((int32_t) m_Failed) 
              + FONT_NORMAL
            ;
        }

    }

    if ((m_Verbosity & vPassed) || (m_Verbosity & vSummary)) {
        outputLine += "; passed " + lib::comma((int32_t) m_Passed); 
    }
    #endif
    std::string outputLine = lib::format(
        "%s%7s%s failed; %s%3d%s not implemented; %7s passed; class %s"
      , (m_Failed ? FONT_RED : FONT_NORMAL)
      , lib::comma((int32_t)m_Failed).c_str()
      , FONT_NORMAL

      , (m_NotImplemented ? FONT_ORANGE : FONT_NORMAL)
      , (int32_t) m_NotImplemented
      , FONT_NORMAL

      , lib::comma((int32_t)m_Passed).c_str()

      , m_ClassName.c_str()
    );

    verbosity_t level = vSummary;
    if (m_Failed > 0) {
        level |= vFailed;
    }
    if (m_NotImplemented > 0) {
        level |= vNotImplemented;
    }

    if (m_Passed > 0) {
        level |= vPassed;
    }

    output(level, outputLine);

} /// void Test::run()  ///

//------------------------------------------------------------------------------
///@brief   Return the number of failed tests.                                  
//------------------------------------------------------------------------------
int Test::getFailed() const {
    return m_Failed;
} 

//------------------------------------------------------------------------------
///@brief   Return the number of passed tests.                                  
//------------------------------------------------------------------------------
int Test::getPassed() const  {
    return m_Passed;
} 

//------------------------------------------------------------------------------
///@brief   Return the number of not implemented tests.                         
//------------------------------------------------------------------------------
int Test::getNotImplemented() const {
    return m_NotImplemented;
}

//------------------------------------------------------------------------------
///@par  Method:  setVerbosity                                                  
///                                                                             
///@par  Purpose:                                                               
///         Set the verbosity -- how "chatty" the test is.                      
//------------------------------------------------------------------------------
void Test::setVerbosity(Test::verbosity_t verbosity)  {
    m_Verbosity = verbosity;
} /// void Test::setVerbosity(verbosity_t verbosity)  ///

//------------------------------------------------------------------------------
///@par  Method:  getVerbosity                                                  
///                                                                             
///@par  Purpose:                                                               
///                                                                             
//------------------------------------------------------------------------------
Test::verbosity_t Test::getVerbosity() const {
    return m_Verbosity;
} /// verbosity_t Test::getVerbosity() const ///

///-----------------------------------------------------------------------------
///@par  Method:  conditionalTest                                               
///                                                                             
///@par  Purpose:                                                               
///                                                                             
///-----------------------------------------------------------------------------
bool Test::conditionalTest (
    const bool success
  , const std::string& test
  , const char* file
  , long int line
) {
    std::string red   (lib::format("%c[48;2;196;1;1;38;2;255;255;255m", 27));
    std::string normal(lib::format("%c[0m", 27));
    std::string outputLine("      test ");
    verbosity_t level;
    if (success) {
        ++m_Passed;
        outputLine += "passed;  file=";
        level = vPassed;
    } else {
        ++m_Failed;
        outputLine += red + "failed; " + normal  + " file=";
        level = vFailed;
    }
    outputLine += file;

    char buffer[64];

    sprintf(
        buffer
      , "; line=%s %ld %s"
      , (success ? "" : red.c_str())
      , line
      , normal.c_str()
    );

    outputLine += buffer;

    outputLine += "; test=" + test;

    output(level, outputLine);

    return success;

} /// void Test::conditionalTest ///

///-----------------------------------------------------------------------------
///@brief   Called to indicate that a test has not been implemented.            
///                                                                             
///         Failed tests are a cause for concern.  Not implemented tests are    
///         a reminder to the developer.                                        
///-----------------------------------------------------------------------------
void Test::notImplemented(
    const std::string& message
  , const std::string& file
  , long int    line
)
{
    std::string orange(lib::format("%c[48;2;196;128;1;38;2;255;255;255m", 27));
    std::string normal(lib::format("%c[0m", 27));

    ++m_NotImplemented;

    std::string outputLine("      test ");

    outputLine += orange + "not-imp;" + normal 
        + " file=" + file
        + lib::format("; line=%s% ld %s", orange.c_str(), line, normal.c_str())
        + "; message=" + message
    ;

    output(vNotImplemented, outputLine);

}


//------------------------------------------------------------------------------
///@brief   Increment the number of passed tests by the given amount.           
//------------------------------------------------------------------------------
void Test::passAdd(int count) {
    m_Passed += count;

}

//------------------------------------------------------------------------------
///@brief   Increment the number of failed tests by the given amount.           
//------------------------------------------------------------------------------
void Test::failAdd(int count)  {
    m_Failed += count;
} 
                                                                                
//------------------------------------------------------------------------------
///@brief   Increment the not implemented tests by the given amount.            
//------------------------------------------------------------------------------
void Test::notImplementedAdd(int count)
{
    m_NotImplemented += count;
}

///-----------------------------------------------------------------------------
///@par  Method:  output                                                        
///                                                                             
///@par  Purpose:                                                               
///                                                                             
///-----------------------------------------------------------------------------
void Test::output(verbosity_t level, const std::string& message)  
{
    if (level & m_Verbosity && m_OutputFunction != NULL) {
        char buffer[25];
        if (m_Verbosity & vTimeStamp) {
//------------------------------------------------------------------------------
//  ftime has been deprecated.  Need to replace with std::chrono...             
//------------------------------------------------------------------------------
#if 0
            timeb current;
            ftime(&current);
            tm time;
//            #ifdef _MT          // multi-thread, localtime is thread safe     
            memcpy(&time, localtime(&current.time), sizeof(tm));
//            #else                                                             
//            localtime_r(&current.time, &time);                                
//            #endif                                                            
            sprintf(
                buffer
              , "%04d-%02d-%02d %02d:%02d:%02d.%03d "
              , time.tm_year+1900
              , time.tm_mon+1
              , time.tm_mday
              , time.tm_hour
              , time.tm_min
              , time.tm_sec
              , current.millitm
            );
#else
            strncpy(buffer, "", sizeof(buffer));
#endif
        } else {
            strncpy(buffer, "", sizeof(buffer));
        }
        m_OutputFunction(m_OutputIndentation + buffer + message);

    } // if (level & m_Verbosity) 
} // void Test::output(verbosity_t level, const std::string& message)  


//------------------------------------------------------------------------------
///@brief   Increment the indentation level (for tests calling tests).          
//------------------------------------------------------------------------------
void Test::outputIndent()  {
    m_OutputIndentation += "    ";
} 

//------------------------------------------------------------------------------
///@brief   Decrement the indentation level.                                    
//------------------------------------------------------------------------------
void Test::outputUnindent()  {
    size_t size = m_OutputIndentation.size();
    m_OutputIndentation = m_OutputIndentation.substr(0,size-4);
} 

void Test::runTest1()   {  }
void Test::runTest2()   {  }
void Test::runTest3()   {  }
void Test::runTest4()   {  }
void Test::runTest5()   {  }

} // namespace work //
} // namespace test //
} // namespace dev  //

