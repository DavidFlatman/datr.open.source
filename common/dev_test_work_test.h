//------------------------------------------------------------------------------
///@file dev_test_work_test.h                                                   
///                                                                             
///@brief   Holds the declaration of the dev::test::work::Test class; the basis 
///         for @e useful testing.                                              
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
//------------------------------------------------------------------------------

#ifndef DEV_TEST_WORK_TEST_H_FILE_GUARD
#define DEV_TEST_WORK_TEST_H_FILE_GUARD

#include "lib_ds_shared_ptr.h"

#include <atomic>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/timeb.h>
#include <time.h>

namespace dev  {
namespace test  {
namespace work  {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Test                                                             
///                                                                             
///@brief   Base class for test classes.                                        
///                                                                             
///@par Thread Safety: special                                                  
///         As of 2013-11-01 made the conditionalTest method thread safe.       
///                                                                             
///@par Purpose:                                                                
///         The Test abstract class is used to create test classes -- classes to
///         unit test other classes; e.g., DirectoryTest class would derive from
///         dev::test::work::Test in order to test the Directory class.         
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             namespace lib {                                                 
///             namespace work {                                                
///             namespace test {                                                
///                                                                             
///             class MyClassTest                                               
///                 : public dev::test::work::Test                              
///             {                                                               
///                 public:                                                     
///                     void runTest();                                         
///                 private:                                                    
///                     void function1();                                       
///                     void function2();                                       
///             };                                                              
///                                                                             
///             void MyClassTest::runTest()                                     
///             {                                                               
///                 function1();                                                
///                 function2();                                                
///             }                                                               
///                                                                             
///             void MyClassTest::function1()                                   
///             {                                                               
///                 lib::work::MyClass c;                                       
///                 TEST(c.function1() == 42);                                  
///             }                                                               
///                                                                             
///             void MyClassTest::function2()                                   
///             {                                                               
///                 TEST_NOT_IMPLEMENTED;                                       
///             }                                                               
///                                                                             
///                                                                             
///             } // namespace test                                             
///             } // namespace work                                             
///             } // namespace lib                                              
///         @endcode                                                            
///                                                                             
///@version 2020-09-01  DHF     Removed PROXY macros, and *_ACTUAL macros.      
///                                                                             
///@version 2020-06-19  DHF     Updated m_TestClass in order to make macros     
///                             easier.                                         
///                                                                             
///@version 2020-05-06  DHF     Made use ot std::atomic.                        
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Merged KCG's lib::ds::shared_ptr work.          
///                                                                             
///@version 2020-02-27  DHF     Added PROXY_* and class ProxyTest               
///                             Added TEST...ACTUAL macros                      
///                                                                             
///@version 2019-11-12  MNT     Added TEST_IS_EQUAL, TEST_IS_EQUAL_PREFIX,      
///                                   TEST_NOT_EQUAL, and TEST_NOT_EQUAL_PREFIX 
///                                                                             
///@version 2018-12-06  MNT     Added EXPECT_AN_EXCEPTION_PREFIX                
///                             Added EXPECT_EXCEPTION_PREFIX                   
///                                                                             
///@version 2018-06-06  MNT     Removed commented out code, half the file.      
///                                                                             
///@version 2018-05-31  MNT     Added TEST_PREFIX_ABORT_WITH_RESULT             
///                                                                             
///@version 2018-04-18  DHF/KJS Added TEST_ABORT_WITH_RESULT                    
///                                                                             
///@version 2018-03-06  DHF     Added TEST_NOT_IMPLEMENTED                      
///                             Added expected usage at top.                    
///                                                                             
///@version 2016-06-27  MNT     Added TEST_PREFIX_ABORT_IF_FAIL                 
///                                                                             
///@version 2016-04-13  DHF/MNT Added TEST_PREFIX and updated header file guard 
///                             Added TEST_ABORT_IF_FAIL                        
///                                                                             
///@version 2014-02-25  DHF     Have the conditionalTest method return the value
///                             of success parameter.  (true = pass).           
///                                                                             
///@version 2014-02-13  DHF     Added testLevel to run method.                  
///                                                                             
///@version 2014-02-11  DHF     Added flag if the test has already been executed
///                                                                             
///@version 2014-01-22  DHF     Modification for Boost v1.54                    
///                                                                             
///@version 2013-10-02  DHF     Added TEST_CONSTRUCTOR                          
///                             Added TEST_CONSTRUCTOR_EXPECT_EXCEPTION         
///                                                                             
///@version 2013-02-20  DHF     Changed type of conditionalTest's "test"        
///                             parameter to std::string.                       
///                                                                             
///@version 2010-03-02  DHF     Changed the outer namespace to dev[eloper].     
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2006-10-12  DHF     Converted to range division coding standards    
///                             (as remembered from meeting on 2006-10-10).     
///                                                                             
///@version 2005-12-06  DHF     Factored Log methods into internal class.       
///                                                                             
///@version 2005-10-10  DHF     Changed to tools                                
///                             Removed all non-standard dependencies.          
///                             Plain text output.                              
///                             Goal: platform independence.                    
///                                                                             
///@version 2001-04-21  DHF     Changed to itis::tools namespace.               
///                             Removed dependency on CTestResults.             
///                                                                             
///@version 2001-04-13  DHF     File created from notes on Chuck Allison's      
///                             "The Simplest Automated Unit Test Framework     
///                             That Could Possibly Work", SD Conference        
///                             April 12, 2001.                                 
//------------------------------------------------------------------------------
class Test {
    public:
        typedef void (*OutputFunction)(const std::string& message);
        typedef unsigned int verbosity_t;

        static const verbosity_t vFailed;    ///< failed tests & failed stats   
        static const verbosity_t vHeader;    ///< header for test start         
        static const verbosity_t vPassed;    ///< log passed tests              
        static const verbosity_t vSilent;    ///< don't log anything            
        static const verbosity_t vStatistics;///< log header, summary, & Time   
        static const verbosity_t vSummary;   ///< log summary info after tests  
        static const verbosity_t vTimeStamp; ///< put time stamp on output      
        static const verbosity_t vVerbose;   ///< log everything                
        static const verbosity_t vRunLevelTime;     ///< log length of run level
        static const verbosity_t vRunLevelTimeLong; ///< log long tests         
        static const verbosity_t vNotImplemented;   ///< log not implemented    

        Test(const std::string& className);
        Test(const Test& that);
        Test& operator=(const Test& that);
        virtual ~Test();

        std::string className() const;

        void run(
            verbosity_t 
          , OutputFunction 
          , const std::string& indentLevel 
          , int testLevel 
        );

        int getFailed() const;
        int getPassed() const;
        int getNotImplemented() const;

        void setVerbosity(verbosity_t verbosity);
        verbosity_t getVerbosity() const;

        bool conditionalTest(
            const bool  success
          , const std::string& test
          , const char* file
          , long int line
        );

        template <typename TYPE>
        inline bool conditionalTest(
            const lib::ds::shared_ptr<TYPE>& p
          , const std::string& test
          , const char* file
          , long int line
        ) {
            return conditionalTest(p.get() != NULL, test, file, line);
        }

        void notImplemented(
            const std::string& message
          , const std::string& file
          , long int    line
        );

    protected:
        void recordElapsed(
            const char* test
          , const timeb& startTime
        );

        virtual void runTest() = 0;
        virtual void runTest1();
        virtual void runTest2();
        virtual void runTest3();
        virtual void runTest4();
        virtual void runTest5();

        void passAdd(int count);
        void failAdd(int count);
        void notImplementedAdd(int count);

        void output(verbosity_t level, const std::string& message);
        void outputIndent();
        void outputUnindent();

        OutputFunction  m_OutputFunction;
        std::string     m_OutputIndentation;

        dev::test::work::Test* m_TestClass;

    private:
        std::string             m_ClassName;
        std::atomic<uint32_t>   m_Failed;
        std::atomic<uint32_t>   m_Passed;
        std::atomic<uint32_t>   m_NotImplemented;
        verbosity_t             m_Verbosity;
        bool                    m_AlreadyExecuted;

}; // class Test //                                                             

//------------------------------------------------------------------------------
///@par  Macro:  TEST                                                           
///                                                                             
///@brief   Conduct a conditional test.                                         
///                                                                             
///     The TEST macro is used to perform a test, capturing the character       
///     version of the test (via \#cond), the file  the test was executed       
///     in (__FILE__),  and line number (__LINE__) that the test was            
///     conducted from.                                                         
///                                                                             
///@par  Design Note:  Restrictions                                             
///     This macro is designed for use inside a method of a class derived       
///     from dev::test::work::Test.                                             
///                                                                             
///@par  Design Note:  Why a Macro                                              
///     TEST is made to be a macro in order to pick up the __FILE__ and         
///     __LINE__ values in the correct spot.  If it was an inline function      
///     (normally preferred), it would fail to pick up __FILE__ and __LINE__    
///     properly.                                                               
///\n                                                                           
///     Additionally, TEST needs to be a macro in order to pick up the          
///     condition (entered once) as both executable code and as a string.       
///                                                                             
///@param   cond    The condition that should be tested; true implies pass      
///                 false implies fail.                                         
//------------------------------------------------------------------------------
#define TEST(cond)                                                          \
    try {                                                                   \
        m_TestClass->conditionalTest(cond, #cond, __FILE__, __LINE__);      \
    } catch(const std::exception& e) {                                      \
        std::string msg("std::exception thrown: ");                         \
        msg += e.what();                                                    \
        msg += "  " #cond;                                                  \
        FAIL(msg.c_str());                                                  \
    } catch(...) {                                                          \
        FAIL("exception thrown: " #cond);                                   \
    }


//------------------------------------------------------------------------------
///@par Macro:  TEST_NOT_IMPLEMENTED                                            
///                                                                             
///@brief   Indicate that a test has not been implemented.                      
///                                                                             
///         A passed test indicates that we have conducted a test and all is    
///         well.  A failed test is a cause for concern.  A not implemented     
///         test is a reminder to the developer that something should be        
///         done, but doesn't rise to the concern of a failed test.             
///                                                                             
///@par  Design Note:  Restrictions                                             
///     This macro is designed for use inside a method of a class derived       
///     from dev::test::work::Test.                                             
///                                                                             
///@par  Design Note:  Why a Macro                                              
///     TEST is made to be a macro in order to pick up the __FILE__ and         
///     __LINE__ values in the correct spot.  If it was an inline function      
///     (normally preferred), it would fail to pick up __FILE__ and __LINE__    
///     properly.                                                               
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             namespace lib {                                                 
///             namespace work {                                                
///             namespace test {                                                
///                                                                             
///             void MyClassTest::someFunction()                                
///             {                                                               
///                 TEST_NOT_IMPLEMENTED;                                       
///             }                                                               
///             } // namespace lib //                                           
///             } // namespace work //                                          
///             } // namespace test //                                          
///         @endcode                                                            
///                                                                             
//------------------------------------------------------------------------------
#define TEST_NOT_IMPLEMENTED                                        \
        m_TestClass->notImplemented(__func__, __FILE__, __LINE__);


//------------------------------------------------------------------------------
///@par Macro:  TEST_PREFIX                                                     
///                                                                             
///@par Purpose:                                                                
///     The TEST_PREFIX macro acts like the TEST macro while adding on a given  
///     prefix to any messages generated.                                       
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             for (int i=0; i < 100; ++i) {                                   
///                 TEST_PREFIX(lib::toString(i) + ": ", myFunction(i));        
///             }                                                               
///         @endcode                                                            
///                                                                             
///@param prefix    A string (or item that can be cast to std::string) to       
///                 pre-pend to any message generated.                          
//------------------------------------------------------------------------------
#define TEST_PREFIX(prefix, cond)                                           \
    try {                                                                   \
        m_TestClass->conditionalTest(                                       \
            cond                                                            \
          ,((std::string)prefix) + #cond                                    \
          , __FILE__                                                        \
          , __LINE__                                                        \
        );                                                                  \
    } catch(const std::exception& e) {                                      \
        FAIL(                                                               \
            "std::exception thrown: "                                       \
          + std::string(e.what())                                           \
          + ((std::string)prefix)                                           \
          + std::string(#cond)                                              \
        );                                                                  \
    } catch(...) {                                                          \
        FAIL(                                                               \
            "exception thrown: " + std::string(prefix) + #cond              \
        );                                                                  \
    }


//------------------------------------------------------------------------------
///@par Macro:  TEST_IS_EQUAL                                                   
///                                                                             
///@par Purpose:                                                                
///     The TEST_IS_EQUAL macro is for equal comparisons only. It builds        
///     upon TEST_PREFIX. There is no parameter for any condition to be         
///     tested, instead there is one parameter for each of the two              
///     values to be tested. The two parameters are also used to generate       
///     a string that in turn is used as a prefix-string for TEST_PREFIX.       
///                                                                             
///@note    Submitted values must be of types that have << and == operators.    
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             std::string myName(getGreatName());                             
///             TEST_IS_EQUAL(myName, "Magnus");                                
///             TEST_IS_EQUAL(round(pi), 3);                                    
///         @endcode                                                            
//------------------------------------------------------------------------------
#define TEST_IS_EQUAL(left, right)                                          \
{                                                                           \
    std::stringstream stream;                                               \
    stream << "'" << left << "' == '" << right << "' ";                     \
    TEST_PREFIX(stream.str(), left == right);                               \
}


//------------------------------------------------------------------------------
///@par Macro:  TEST_IS_EQUAL_PREFIX                                            
///                                                                             
///@par Purpose:                                                                
///     The TEST_IS_EQUAL_PREFIX macro is for equal comparisons only.           
///     It is similar to TEST_IS_EQUAL, but a prefix can be submitted           
///     too. As with TEST_IS_EQUAL, there is no parameter for any condition     
///     to be tested. Instead there is one parameter for each of the two        
///     values to be tested. These two parameters are also used, in             
///     combination with the prefix, to generate a new prefix for the           
///     call to the underlying TEST_PREFIX macro.                               
///                                                                             
///@note    Submitted prefix must be of a type that has an << operator.         
///                                                                             
///@note    Submitted values must be of types that have << and == operators.    
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             std::vector<std::string> greatNames;                            
///             getGreatNames(greatNames);                                      
///             for(int i = 0; i < greatNames.size(); ++i) {                    
///                 TEST_IS_EQUAL_PREFIX(                                       
///                     lib::format(" %d: ", i)                                 
///                   , greatNames[i]                                           
///                   , "Magnus"                                                
///                 );                                                          
///             }                                                               
///         @endcode                                                            
//------------------------------------------------------------------------------
#define TEST_IS_EQUAL_PREFIX(prefix, left, right)                           \
{                                                                           \
    std::stringstream stream;                                               \
    stream << prefix << "'" << left << "' == '" << right << "' ";           \
    TEST_PREFIX(stream.str(), left == right);                               \
}


//------------------------------------------------------------------------------
///@par Macro:  TEST_NOT_EQUAL                                                  
///                                                                             
///@par Purpose:                                                                
///     The TEST_NOT_EQUAL macro is a non-equal version of TEST_IS_EQUAL.       
///                                                                             
///@note    See TEST_IS_EQUAL for further information.                          
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             std::string normalBirthday(getNormalBirthday());                
///             TEST_NOT_EQUAL(normalBirthday, "February 29");                  
///             TEST_NOT_EQUAL(pi, 314);                                        
///         @endcode                                                            
//------------------------------------------------------------------------------
#define TEST_NOT_EQUAL(left, right)                                         \
{                                                                           \
    std::stringstream stream;                                               \
    stream << "'" << left << "' != '" << right << "' ";                     \
    TEST_PREFIX(stream.str(), left != right);                               \
}


//------------------------------------------------------------------------------
///@par Macro:  TEST_NOT_EQUAL_PREFIX                                           
///                                                                             
///@par Purpose:                                                                
///     The TEST_NOT_EQUAL_PREFIX macro is a non-equal version                  
///     of TEST_IS_EQUAL_PREFIX.                                                
///                                                                             
///@note    See TEST_IS_EQUAL_PREFIX for further information.                   
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             std::vector<std::string> badNames;                              
///             getBadNames(badNames);                                          
///             for(size_t i = 0; i < badNames.size(); ++i) {                   
///                 TEST_NOT_EQUAL_PREFIX(                                      
///                     lib::format(" %d: ", i)                                 
///                   , badNames[i]                                             
///                   , "Magnus"                                                
///                 );                                                          
///             }                                                               
///         @endcode                                                            
//------------------------------------------------------------------------------
#define TEST_NOT_EQUAL_PREFIX(prefix, left, right)                          \
{                                                                           \
    std::stringstream stream;                                               \
    stream << prefix << "'" << left << "' != '" << right << "' ";           \
    TEST_PREFIX(stream.str(), left != right);                               \
}


//------------------------------------------------------------------------------
///@par Macro TEST_ABORT_IF_FAIL                                                
///                                                                             
///@par Purpose:                                                                
///         The TEST_ABORT_IF_FAIL will register a failure similar to TEST      
///         macro, but if the test fails, the macro invokes a @e return to      
///         terminate the current set of tests.                                 
///                                                                             
///@note    This macro is mainly designed for when you're trying to test the    
///         test environment.                                                   
///                                                                             
///@par Expeced Usage:                                                          
///     @code                                                                   
///         FILE* fp = fopen("datafile.dat", "rb");                             
///         TEST_ABORT_IF_FAIL(fp != NULL);                                     
///         fread(buffer, sizeof(buffer), 1, fp);                               
///     @endcode                                                                
//------------------------------------------------------------------------------
#define TEST_ABORT_IF_FAIL(cond)                                            \
    try {                                                                   \
        if (                                                                \
            !m_TestClass->conditionalTest(                                  \
                cond                                                        \
              , #cond                                                       \
              , __FILE__                                                    \
              , __LINE__                                                    \
            )                                                               \
        ) return;                                                           \
    } catch(const std::exception& e) {                                      \
        std::string msg("std::exception thrown: ");                         \
        msg += e.what();                                                    \
        msg += "  " #cond;                                                  \
        FAIL(msg.c_str());                                                  \
        return;                                                             \
    } catch(...) {                                                          \
        FAIL("exception thrown: " #cond);                                   \
        return;                                                             \
    }


//------------------------------------------------------------------------------
///@par Macro:  TEST_PREFIX_ABORT_IF_FAIL                                       
///                                                                             
///@par Purpose:                                                                
///     The TEST_PREFIX_ABORT_IF_FAIL macro acts like a combination of the      
///     TEST_PREFIX macro and the TEST_ABORT_IF_FAIL macro. It adds a given     
///     prefix to any messages generated and, if the test fails, the macro      
///     invokes a @e return to terminate the current set of tests.              
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///         std::string fileName = "datafile.dat"                               
///         FILE* fp = fopen(fileName, "rb");                                   
///         TEST_PREFIX_ABORT_IF_FAIL(fileName + ": ", fp != NULL);             
///         fread(buffer, sizeof(buffer), 1, fp);                               
///     @endcode                                                                
///                                                                             
///@param prefix    A string (or item that can be cast to std::string) to       
///                 pre-pend to any message generated.                          
//------------------------------------------------------------------------------
#define TEST_PREFIX_ABORT_IF_FAIL(prefix, cond)                             \
    try {                                                                   \
        if (                                                                \
            !m_TestClass->conditionalTest(                                  \
                cond,((std::string)prefix) + #cond, __FILE__, __LINE__      \
            )                                                               \
        ) {                                                                 \
            return;                                                         \
        }                                                                   \
    } catch(const std::exception& e) {                                      \
        std::string msg("std::exception thrown: ");                         \
        msg += e.what();                                                    \
        msg += ((std::string)prefix);                                       \
        msg += "  " #cond;                                                  \
        FAIL(msg.c_str());                                                  \
        return;                                                             \
    } catch(...) {                                                          \
        FAIL(                                                               \
            "exception thrown: " + std::string(prefix) + #cond              \
        );                                                                  \
        return;                                                             \
    }


//------------------------------------------------------------------------------
///@par Macro TEST_ABORT_WITH_RESULT                                            
///                                                                             
///@par Purpose:                                                                
///         The TEST_ABORT_WITH_RESULT will work in the same way as             
///         TEST_ABORT_IF_FAIL, but it will instead require a second parameter. 
///         Make sure to include what is to be returned for this second         
///         parameter. The main reason a second paramter is for the             
///         compilation of helper methods that call for the test macros.        
///                                                                             
///@note    This macro is mainly designed for when you're trying to test the    
///         test environment.                                                   
///                                                                             
///@par Expeced Usage:                                                          
///     @code                                                                   
///             FILE* fp = fopen("datafile.dat", "rb");                         
///                                                                             
///             lib::ds::shared_ptr<                                            
///                 std::vector<lib::pcm::work::ConstFrameVarPtr>               
///             > frames;                                                       
///                                                                             
///             TEST_ABORT_WITH_RESULT(fp != NULL, frames);                     
///             fread(buffer, sizeof(buffer), 1, fp);                           
///     @endcode                                                                
//------------------------------------------------------------------------------
#define TEST_ABORT_WITH_RESULT(cond, result)                                \
    try {                                                                   \
        if (                                                                \
            !m_TestClass->conditionalTest(cond, #cond, __FILE__, __LINE__)  \
        ) return result;                                                    \
    } catch(const std::exception& e) {                                      \
        std::string msg("std::exception thrown: ");                         \
        msg += e.what();                                                    \
        msg += "  " #cond;                                                  \
        FAIL(msg.c_str());                                                  \
        return result;                                                      \
    } catch(...) {                                                          \
        FAIL("exception thrown: " #cond);                                   \
        return result;                                                      \
    }
    



//------------------------------------------------------------------------------
///@par Macro TEST_PREFIX_ABORT_WITH_RESULT                                     
///                                                                             
///@par Purpose:                                                                
///         The TEST_PREFIX_ABORT_WITH_RESULT macro acts like a combination     
///         of the TEST_PREFIX macro and the TEST_ABORT_WITH_RESULT macro.      
///         It adds a given prefix to any messages generated and, if the test   
///         fails, the macro invokes a @e return with the provided return       
///         value to terminate the current set of tests.                        
///                                                                             
///@note    This macro is mainly designed for when you are testing the          
///         test environment.                                                   
///                                                                             
///@par Expeced Usage:                                                          
///     @code                                                                   
///             std::string fileName = "datafile.dat"                           
///                                                                             
///             FILE* fp = fopen(fileName, "rb");                               
///                                                                             
///             lib::ds::shared_ptr<                                            
///                 std::vector<lib::pcm::work::ConstFrameVarPtr>               
///             > frames;                                                       
///                                                                             
///             TEST_ABORT_WITH_RESULT(fileName + ": ", fp != NULL, frames);    
///             fread(buffer, sizeof(buffer), 1, fp);                           
///     @endcode                                                                
//------------------------------------------------------------------------------
#define TEST_PREFIX_ABORT_WITH_RESULT(prefix, cond, result)                 \
    try {                                                                   \
        if (                                                                \
            !m_TestClass->conditionalTest(                                  \
                cond                                                        \
              , ((std::string)prefix) + #cond                               \
              , __FILE__                                                    \
              , __LINE__                                                    \
            )                                                               \
        ) return result;                                                    \
    } catch(const std::exception& e) {                                      \
        std::string msg("std::exception thrown: ");                         \
        msg += e.what();                                                    \
        msg += ((std::string)prefix);                                       \
        msg += "  " #cond;                                                  \
        FAIL(msg.c_str());                                                  \
        return result;                                                      \
    } catch(...) {                                                          \
        FAIL(                                                               \
            "exception thrown: " + std::string(prefix) + #cond              \
        );                                                                  \
        return result;                                                      \
    }


inline bool CLOSE(double l, double r, double m) {
    return (l < r) ? (r-l <= m) : (l-r <= m);
}

inline bool CLOSE(double l, double r) {return CLOSE(l, r, 0.00001L);}

//------------------------------------------------------------------------------
///@par Macro:  TEST_CONSTRUCTOR                                                
///                                                                             
///@brief   Test a constructor.                                                 
///                                                                             
///@par Purpose:                                                                
///         The TEST macro expects the given parameter to evaluate to a boolean 
///         expression.  Unfortunately, when testing constructors, we [usually] 
///         can't have the constructor devolve to a boolean.  This macro is     
///         for when the constructor doesn't throw an exception, the test       
///         passes.                                                             
///                                                                             
//------------------------------------------------------------------------------
#define TEST_CONSTRUCTOR(constructor)                                       \
    try {                                                                   \
        constructor;                                                        \
        m_TestClass->conditionalTest(true, #constructor,__FILE__,__LINE__); \
    } catch(const std::exception& e) {                                      \
        std::string msg("std::exception thrown: ");                         \
        msg += e.what();                                                    \
        msg += "  " #constructor;                                           \
        FAIL(msg.c_str());                                                  \
    } catch(...) {                                                          \
        FAIL("exception thrown: " #constructor);                            \
    }


//------------------------------------------------------------------------------
///@par Macro:  TEST_CONSTRUCTOR_EXPECT_EXCEPTION                               
///                                                                             
///@brief   Test a constructor when we expect it to throw a given exception.    
///                                                                             
///@par Purpose:                                                                
///         The TEST macro expects the given parameter to evaluate to a boolean 
///         expression.  Unfortunately, when testing constructors, we [usually] 
///         can't have the constructor devolve to a boolean.  This macro is     
///         for when the constructor should throw a given exception.            
///                                                                             
//------------------------------------------------------------------------------
#define TEST_CONSTRUCTOR_EXPECT_EXCEPTION(constructor, exception)           \
    try {                                                                   \
        constructor;                                                        \
        m_TestClass->conditionalTest(                                       \
            false                                                           \
          , #constructor " failed to throw " #exception, __FILE__, __LINE__ \
        );                                                                  \
    } catch(exception)                                                      \
    {                                                                       \
        m_TestClass->conditionalTest(                                       \
            true                                                            \
          , #constructor " threw " #exception, __FILE__, __LINE__           \
        );                                                                  \
    } catch(...)                                                            \
    {                                                                       \
        m_TestClass->conditionalTest(                                       \
            false                                                           \
          , #constructor " threw unexpected exception", __FILE__, __LINE__  \
        );                                                                  \
    }


//------------------------------------------------------------------------------
///@par  Macro:  FAIL                                                           
///                                                                             
///@par  Purpose:                                                               
///     The FAIL macro is used to force a failed test (e.g., exception          
///     wasn't thrown when one was expected) to be recorded.                    
///                                                                             
///@par  Design Note:  Restrictions                                             
///     This macro is designed for use inside a method of a class derived       
///     from CTest.                                                             
///                                                                             
///@par  Design Note:  Why a Macro                                              
///     FAIL is made to be a macro in order to pick up the __FILE__ and         
///     __LINE__ values in the correct spot.  If it was an inline function      
///     (normally preferred), it would fail to pick up __FILE__ and __LINE__    
///     properly.                                                               
///                                                                             
///@param   str     The string to be outputted as part of the fail condition    
//------------------------------------------------------------------------------
#define FAIL(str)                                                           \
    m_TestClass->conditionalTest(false, str,  __FILE__, __LINE__)


//------------------------------------------------------------------------------
///@par  Macro: PASS                                                            
///                                                                             
///@par  Purpose:                                                               
///     The PASS macro is used to force a passed test (e.g., an exception       
///     was thrown when expected) to be recorded.                               
///                                                                             
///@par  Design Note:  Restrictions                                             
///     This macro is designed for use inside a method of a class derived       
///     from CTest.                                                             
///                                                                             
///@par  Design Note:  Why a Macro                                              
///     PASS is made to be a macro in order to pick up the __FILE__ and         
///     __LINE__ values in the correct spot.  If it was an inline function      
///     (normally preferred), it would fail to pick up __FILE__ and __LINE__    
///     properly.                                                               
///                                                                             
///@param   str     The string to be outputted as part of the pass condition    
//------------------------------------------------------------------------------
#define PASS(str)                                                       \
    m_TestClass->conditionalTest(true,  str,  __FILE__, __LINE__)


//------------------------------------------------------------------------------
///@par  Macro:  EXPECT_AN_EXCEPTION                                            
///@par  Purpose:                                                               
///     The EXPECT_AN_EXCEPTION macro is used when a particular test            
///     succeeds when a specific exception is thrown.                           
///@par  Design Note:  Why a Macro                                              
///     EXPECT_AN_EXCEPTION is made to be a macro in order to pick up the       
///     __FILE__ and __LINE__ values in the correct spot.  If it was an         
///     inline function (normally preferred), it would fail to pick up          
///     __FILE__ and __LINE__ properly.                                         
///@param   cond            The condition that should be executed that will     
///                         throw an exception.                                 
///@param   exceptionType   The exception that is expected by the condition.    
//------------------------------------------------------------------------------
#define EXPECT_AN_EXCEPTION(cond, exceptionType)                            \
    try {                                                                   \
        cond;                                                               \
        FAIL(#cond ": exception not thrown");                               \
    } catch(exceptionType) {                                                \
        PASS(#cond ": exception thrown");                                   \
    } catch(...) {                                                          \
        FAIL(#cond ": wrong exception thrown");                             \
    }


//------------------------------------------------------------------------------
///@par  Macro:  EXPECT_AN_EXCEPTION_PREFIX                                     
///@par  Purpose:                                                               
///     The EXPECT_AN_EXCEPTION_PREFIX macro is used when a particular test     
///     succeeds when a specific exception is thrown and a prefix is needed.    
///@par  Design Note:  Why a Macro                                              
///     EXPECT_AN_EXCEPTION_PREFIX is made to be a macro in order to pick up    
///     the __FILE__ and __LINE__ values in the correct spot.  If it was an     
///     inline function (normally preferred), it would fail to pick up          
///     __FILE__ and __LINE__ properly.                                         
///@param   prefix          A string (or item that can be cast to std::string)  
///                         to pre-pend to any message generated.               
///@param   cond            The condition that should be executed that will     
///                         throw an exception.                                 
///@param   exceptionType   The exception that is expected by the condition.    
//------------------------------------------------------------------------------
#define EXPECT_AN_EXCEPTION_PREFIX(prefix, cond, exceptionType)             \
    try {                                                                   \
        cond;                                                               \
        FAIL(std::string(prefix) + ": " + #cond ": exception not thrown");  \
    } catch(exceptionType) {                                                \
        PASS(std::string(prefix) + ": " + #cond ": exception thrown");      \
    } catch(...) {                                                          \
        FAIL(std::string(prefix) + ": " + #cond ": wrong exception thrown");\
    }


//------------------------------------------------------------------------------
///@par  Macro:  EXPECT_EXCEPTION                                               
///@par  Purpose:                                                               
///     The EXPECT_EXCEPTION macro is used when a particular test succeeds      
///     when any exception is thrown.                                           
///@par  Design Note:  Why a Macro                                              
///     EXPECT_EXCEPTION is made to be a macro in order to pick up the          
///     __FILE__ and __LINE__ values in the correct spot.  If it was an         
///     inline function (normally preferred), it would fail to pick up          
///     __FILE__ and __LINE__ properly.                                         
///@param   cond    The condition that should be executed that should toss      
///                 an exception.                                               
//------------------------------------------------------------------------------
#define EXPECT_EXCEPTION(cond)                                              \
    try {                                                                   \
        cond;                                                               \
        FAIL(#cond ": exception not thrown");                               \
    } catch(...) {                                                          \
        PASS(#cond ": exception thrown");                                   \
    }


//------------------------------------------------------------------------------
///@par  Macro:  EXPECT_EXCEPTION_PREFIX                                        
///@par  Purpose:                                                               
///     The EXPECT_EXCEPTION_PREFIX macro acts like a combination               
///     of the TEST_PREFIX macro and the EXPECT_EXCEPTION macro.                
///     The EXPECT_EXCEPTION_PREFIX macro is used when a particular test        
///     succeeds when any exception is thrown and a prefix is needed.           
///@par  Design Note:  Why a Macro                                              
///     EXPECT_EXCEPTION_PREFIX is made to be a macro in order to pick up the   
///     __FILE__ and __LINE__ values in the correct spot.  If it was an         
///     inline function (normally preferred), it would fail to pick up          
///     __FILE__ and __LINE__ properly.                                         
///@param   prefix  A string (or item that can be cast to std::string) to       
///                 pre-pend to any message generated.                          
///@param   cond    The condition that should be executed that should toss      
///                 an exception.                                               
//------------------------------------------------------------------------------
#define EXPECT_EXCEPTION_PREFIX(prefix, cond)                               \
    try {                                                                   \
        cond;                                                               \
        FAIL((prefix)+": "+#cond ": exception not thrown");                 \
    } catch(...) {                                                          \
        PASS((prefix)+": "+#cond ": exception thrown");                     \
    }


//------------------------------------------------------------------------------
///@par  Macro:  ELAPSED                                                        
///@par  Purpose:                                                               
///     The ELAPSED macro is used to record the execution time of a             
///     method.                                                                 
///@par  Design Note:  Why a Macro                                              
///     \see EXPECT_EXCEPTION                                                   
///@param   cond    The condition / code that should be executed within the     
///                 timestamp period.                                           
//------------------------------------------------------------------------------
#define ELAPSED(cond) {                                                     \
            _timeb conditionTime_;                                          \
            _ftime(&conditionTime_);                                        \
            cond;                                                           \
            RecordElapsed( #cond, _conditionTime_ ); }

//------------------------------------------------------------------------------
///@par  Macro:  ELAPSED_BEGIN                                                  
///@par  Purpose:                                                               
///     The ELAPSED_BEGIN macro works in conjunction with _ELAPSED_END to       
///     time stamp a set of tests.                                              
///@param   message     The message that should accompany the timestamp.        
//------------------------------------------------------------------------------
#define ELAPSED_BEGIN(message) {                                            \
    char*  elapsed_time_test_message_ = message;                            \
    _timeb elapsed_time_test_begin_;                                        \
    _ftime(&elapsed_time_test_begin_);

//------------------------------------------------------------------------------
///@par  Macro:  _ELAPSED_END                                                   
///@par  Purpose:                                                               
///     The _ELAPSED_END macro works in conjunction with _ELAPSED_BEGIN to      
///     time stamp a set of tests.                                              
//------------------------------------------------------------------------------
#define _ELAPSED_END()                                                      \
    RecordElapsed(elapsed_time_test_message_, elapsed_time_test_begin_);    \
    }


//------------------------------------------------------------------------------
///@brief   When a test support class needs to report status back to a test     
///         class.                                                              
///                                                                             
///         There are times where a test class needs to have a support class in 
///         order to properly test the results (think publisher / subscriber).  
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             class TestParser                                                
///                 : public Parser                                             
///                 , pbulic TestProxy                                          
///             {                                                               
///                 public:                                                     
///                     TestParser(dev::test::work::Test* test_object)          
///                         : TestProxy(test_object)                            
///                         , m_ElementCount(0)                                 
///                     {                                                       
///                                                                             
///                     };                                                      
///                     virtual bool found(Element* e) override                 
///                     {                                                       
///                         switch(m_ElementCount)                              
///                         {                                                   
///                             case 0:                                         
///                                 TEST(e->nodeName() == "xml");               
///                                 break;                                      
///                             case 1:                                         
///                                 TEST(e->nodeName() == "child-node");        
///                                 break;                                      
///                             case 2:                                         
///                                 TEST(e->nodeName() == "child-node");        
///                                 break;                                      
///                             default:                                        
///                                 FAIL("called too many times");              
///                                 break;                                      
///                         } // switch(m_ElementCount)                         
///                         ++m_ElementCount;                                   
///                     } // virtual bool found(Element* e) override            
///                                                                             
///                 private:                                                    
///                     size_t m_ElementCount;                                  
///             };                                                              
///                                                                             
///             // ... snip ...                                                 
///                                                                             
///             void ParserTest::put()                                          
///             {                                                               
///                 //----------------------------------------------------------
///                 //  The base class Parser is being tested, but needs a class
///                 //  derived from it to test the virtual "call back" methods.
///                 //----------------------------------------------------------
///                 TestParser parser(this);                                    
///                 parser.put("<xml>");                                        
///                 parser.put("<child-node id='1' />");                        
///                 parser.put("<child-node id='2' />");                        
///             }                                                               
///         @endcode                                                            
//------------------------------------------------------------------------------
class TestProxy
{
    public:
        TestProxy(dev::test::work::Test*  test_class)
            : m_TestClass(test_class)
        {
        }

    protected:
        dev::test::work::Test* m_TestClass;
};


}  // namesapce work                                                            
}  // namespace test                                                            
}  // namespace dev                                                             

#endif // #ifndef DEV_TEST_WORK_TEST_H_FILE_GUARD
