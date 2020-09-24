//------------------------------------------------------------------------------
///                                                                             
///@file dev_test_work.cpp                                                      
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief Holds the definitions of various test library code.                   
///                                                                             
///@todo    This file should be refactored to take advantage of multiple        
///         threads.  The Singleton "needs" to be a test class only from the    
///         perspective of collecting results -- but not from the perspective   
///         of needing runTest[n]() methods.                                    
///                                                                             
///@note    Since this file is core to the developer test environment, we're    
///         deliberately avoiding using any lib_ files or routines.  We want    
///         the dev_test_work_test* to stand on its own.                        
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Moved from namespace classes to work.           
///                                                                             
///@version 2018-03-06  DHF     Added handling of notImplemented.               
///                                                                             
///@version 2014-02-25  DHF     Added test levels (--test-level=n).             
///                                                                             
///@version 2014-02-11  DHF     -   Modified to exectute the test if either the 
///                                 test or the class under test is specified on
///                                 the command line.  (Credit Jason Halbur for 
///                                 pointing out prior deficiency).             
///                             -   Added help option.                          
///                             -   Added revision history                      
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2008-03-24  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#define __STDC_FORMAT_MACROS

#include "dev_test_work.h"
#include "dev_test_work_test.h"

#include <map>
#include <set>
#include <string>
#include <string.h>                 // strcmp                                   

namespace dev {
namespace test {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

static void display_error(
    const std::string& exception
  , const std::string& message
)
{
    char ESC(27);

    std::cerr 
        << ESC << "[38;2;255;1;1m"
        << "Exception " << exception << " thrown: "
        << ESC << "[0m"
        << message
        << std::endl
    ;
            
}

//------------------------------------------------------------------------------
///@brief Return the root of the given full path. dir/root[_test].ext           
//------------------------------------------------------------------------------
inline std::string filenameRoot(const std::string& full_path)
{
    //--------------------------------------------------------------------------
    //  Last character is first character before _test.ext or .ext.             
    //--------------------------------------------------------------------------
    uint64_t dot = full_path.rfind("_test.");
    if (dot == std::string::npos) dot = full_path.rfind(".");

    //--------------------------------------------------------------------------
    //  First character is first character after the last slash.                
    //--------------------------------------------------------------------------
    uint64_t slash = full_path.rfind("/");
    if (slash == std::string::npos) slash = full_path.rfind("\\");

    return full_path.substr(slash+1, dot-slash-1);
}

//------------------------------------------------------------------------------
///@brief Check if argv indicates the given option, if so set (unset) the bit   
///       pattern in verbosity.                                                 
//------------------------------------------------------------------------------
inline void checkOption(
    Test::verbosity_t& verbosity
  , const char* argv
  , const char* option
  , const Test::verbosity_t& optionValue
) {
    if (!strncmp(argv, "--no-", 5) && !strcmp(&argv[5], option)) {
        verbosity &= ~optionValue;
    } else if (!strncmp(argv, "--", 2) && !strcmp(&argv[2], option)) {
        verbosity |= optionValue;
    }
}

//------------------------------------------------------------------------------
///@brief Type for holding a unique (but inconsistent) number for the registered
///       test and a pointer to an object of the registered test type.          
///                                                                             
///@note  The number will be unique and consistent during a run, but may be     
///       inconsistent between runs.                                            
///                                                                             
///@par std::pair<int, Test*>                                                   
///         Okay, we shouldn't talk about "standard" stuff, but ...             
///\n\n                                                                         
///         @code RegisterIDTest x; @endcode gives us a x.first that is of type 
///         int (holding the unique number and x.second which is the pointer to 
///         the Test object.                                                    
//------------------------------------------------------------------------------
using RegisterIDTest = std::pair<int, Test*>;

//------------------------------------------------------------------------------
///@brief Type for holding the results of a FilenameTestMap::find.              
///                                                                             
///@par std::pair<std::string, RegisterIDTest>                                  
///         Okay, we shouldn't talk about "standard" stuff [again], but ...     
///\n\n                                                                         
///         @code FileNameTestMapElement e @endcode gives us a variable that has
///         the following:                                                      
///             @li e.first is a std::string that ultimately is the root of a   
///                 filename (the filename passed in to registerTestObject, if  
///                 using TEST_REGISTER then the cpp __FILE__ of the test class)
///             @li e.second is a RegisterIDTest (itself a std::pair)           
///             @li e.second.first The unique number for the test object.       
///             @li e.second.second A pointer to a Test object.                 
//------------------------------------------------------------------------------
using FileNameTestMapElement = std::pair<std::string, RegisterIDTest>;

//------------------------------------------------------------------------------
///@brief Type for holding a collection of root filenames (std::string) and     
///       associated RegisterIDTest objects.                                    
//------------------------------------------------------------------------------
using FilenameTestMap = std::map<std::string, RegisterIDTest>;

//------------------------------------------------------------------------------
///@brief A singleton test class for registered (TEST_REGISTER) classes.        
///                                                                             
///@par Evolution                                                               
///         At first, we just had Test and a main program to call all of        
///         the test individually.  All was right with the world, we just copy  
///         and pasted all the code from test harness to test harness.          
///\n\n                                                                         
///         A TestSuite sprang up which was a Test that ran a set               
///         of Test objects (with the passed/failed stats being                 
///         accumulated from the individual Test objects).                      
///\n\n                                                                         
///         The duplicated code was then collected into the test::classes       
///         library where the Test classes could automatically register         
///         themselves with a single, *gasp* global "TestSuite" (the            
///         actual "TestSuite" class was morphed into this private Singleton).  
///\n\n                                                                         
///         Library functions were added to manipulate the Singleton and        
///         simplify the calling code.                                          
///                                                                             
///@par Class Declaration?  In a .CPP File?!?                                   
///         Yeah.  Get over it.                                                 
///\n\n                                                                         
///         This class should only be accessed via the library routines.  I     
///         don't particularly like the Singleton implementation anyway.  If    
///         you really like it *cough* then feel free to factor it out into a   
///         dev::Singleton template and stuff it into it's own file.  I'm       
///         going for simplicity of usage here.  Leave it alone already.  Hey,  
///         isn't that a bug in your code over there ... shouldn't you be fixing
///         that instead?                                                       
//------------------------------------------------------------------------------
class Singleton 
    : public Test 
    , public FilenameTestMap
{
    public:
        //----------------------------------------------------------------------
        ///@brief Return a pointer the single instance of our singleton.        
        //----------------------------------------------------------------------
        static Singleton* instance();

        //----------------------------------------------------------------------
        ///@brief Deconstruct, destroy, delete our singleton.                   
        //----------------------------------------------------------------------
        static void destroy();

        //----------------------------------------------------------------------
        ///@brief Execute the tests, parsing out the command line for what tests
        ///       should be executed.                                           
        //----------------------------------------------------------------------
        static void run(
            int argc
          , char** argv
          , OutputFunction
          , const std::string& indentLevel = ""
          , int first = -1
          , int last = -1
        );

    protected:
        //----------------------------------------------------------------------
        ///@brief Implementation of the pure virtual Test::runTest method.      
        //----------------------------------------------------------------------
        void runTest() override;

    private:
        //----------------------------------------------------------------------
        //  Only the class gets to create & destroy objects of the class.       
        //----------------------------------------------------------------------
        Singleton();
        ~Singleton();
        Singleton(const Singleton& that);
        Singleton& operator=(const Singleton& that);

        static Singleton* pInstance;

        int m_Argc;
        char** m_Argv;

        int m_First;
        int m_Last;

        static int m_TestLevel;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Singleton* Singleton::pInstance = nullptr;
int Singleton::m_TestLevel(0);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Singleton* Singleton::instance()
{
    if (pInstance == nullptr) pInstance = new Singleton;
    return pInstance;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Singleton::destroy()
{
    if (pInstance != nullptr) {
        delete pInstance;
        pInstance = nullptr;
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Singleton::Singleton() : Test("registered classes")
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Singleton::~Singleton()
{
    for (auto & i : *this) {
        delete i.second.second;
    }
    erase(begin(), end());
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void displayHelp()
{
    std::cout << "dev_test_work_test_main --stats --failed\n"
    "\t--[no-]silent      Don't display anything except what's after --silent\n"
    "\t--[no-]failed      Display failed tests.\n"
    "\t--[no-]passed      Display passed tests.\n"
    "\t--[no-]not-impl    Display not implemented tests.\n"
    "\t--[no-]header      Display 'starting...' when beginning test class.\n"
    "\t--[no-]stats       Display --header and --summayr\n"
    "\t--[no-]summary     Display restuls of class test.\n"
    "\t--[no-]stamp       Display a time stamp on each message dipslayed.\n"
    "\t--[no-]time        Display each runLevel's wall-time.\n"
    "\t--[no-]slow        Display how long it took to run \"slow\" runLevels\n"
    "\t--[no-]verbose     Turn all options on.\n"
    "\t--help             Display this help text.\n"
    "\t--revision-history Display the program's revision history.\n"
    "\t--test-level=n     Run all tests at and below the given test level.\n"
    "\t                   E.g., --test-level=3 runs test level 0, 1, 2, and 3.\n"
    "\t--files file1 [file2 [file3]\n"
    "\t                   Run the tests associated with the class in the\n"
    "\t                   files.\n"

    ;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void displayHistory()
{
    std::cout << 
        "----------------------------------------"
        "---------------------------------------\n"
        "dev_test_work_test_main          v2.0.0           2014-02-19\n"
        "----------------------------------------"
        "---------------------------------------\n"
        "   New Features:\n"
        "       -   Added --help option.\n"
        "       -   Added --revision-history option.\n"
        "       -   Added --test-level=n.\n"
        "   Bug Fixes:\n"
        "       None\n"
        "   Known Bugs:\n"
        "       None\n"
        #if 0
        "----------------------------------------"
        "---------------------------------------\n"
        "dev_test_work_test_main          v2.0.0           2014-02-19\n"
        "----------------------------------------"
        "---------------------------------------\n"
        "   New Features:\n"
        "   Bug Fixes:\n"
        "   Known Bugs:\n"
        #endif
        ;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Singleton::run(
    int argc
  , char** argv
  , OutputFunction outputFunc
  , const std::string& indentLevel
  , int first
  , int last
) {
    bool help(false);
    bool history(false);

    instance()->m_Argc = argc;
    instance()->m_Argv = argv;

    instance()->m_First = (first < 0) ? 0 : first;
    instance()->m_Last =  (last  < 0) ? int(instance()->size()) : last;

    Test::verbosity_t verbosity(Test::vSilent);

    for (int a=0; a < argc; ++a) {
        checkOption(verbosity, argv[a], "silent", Test::vSilent);
        checkOption(verbosity, argv[a], "failed", Test::vFailed);
        checkOption(verbosity, argv[a], "passed", Test::vPassed);
        checkOption(verbosity, argv[a], "not-impl", Test::vNotImplemented);
        checkOption(verbosity, argv[a], "header", Test::vHeader);
        checkOption(verbosity, argv[a], "stats",  Test::vStatistics);
        checkOption(verbosity, argv[a], "summary",Test::vSummary);
        checkOption(verbosity, argv[a], "verbose",Test::vVerbose);
        checkOption(verbosity, argv[a], "stamp",  Test::vTimeStamp);
        checkOption(verbosity, argv[a], "time",   Test::vRunLevelTime);
        checkOption(verbosity, argv[a], "slow",   Test::vRunLevelTimeLong);

        std::string arg(argv[a]);
        if (arg.substr(0,13) == "--test-level=") {
            sscanf(arg.substr(13).c_str(), "%d", &m_TestLevel);
        } else if (arg.substr(0,6) == "--help") {
            help = true;
        } else if (arg.substr(0,18) == "--revision-history") {
            history = true;
        }
    }

    if (help)    displayHelp();
    if (history) displayHistory();

    if (!help && !history) {
        instance()->Test::run(verbosity, outputFunc, indentLevel, m_TestLevel);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Singleton::runTest()
{
    //--------------------------------------------------------------------------
    //  Parse argc, argv                                                        
    //--------------------------------------------------------------------------
    bool filesOnCommandLine(false);
    std::set<std::string> fileList;

    bool parsingFiles(false);
    for (int a=0; a < m_Argc; ++a) {
        if (!strcmp(m_Argv[a], "--files") || !strcmp(m_Argv[a], "-f")) {
            parsingFiles = true;
            filesOnCommandLine = true;
        } else if (parsingFiles && m_Argv[a][0] == '-') {
            parsingFiles = false;
        } else if (parsingFiles) {
            fileList.insert(filenameRoot(m_Argv[a]));
        }

    } // for (int a=0; a < 0; ++a) //                                           

    //--------------------------------------------------------------------------
    //  If --file was specified on the command line, only process those tests.  
    //--------------------------------------------------------------------------
    std::string indent(m_OutputIndentation + "    ");

    if (filesOnCommandLine) {
        std::set<std::string>::iterator i;
        for (i=fileList.begin(); i != fileList.end(); ++i) {
            iterator test = find(*i);
            if (test == end()) test = find(*i + "test");

            //------------------------------------------------------------------
            //  Only run the tests if it's both in files and within the range   
            //  we're supposed to work with.                                    
            //------------------------------------------------------------------
            if (
                test != end() 
             && test->second.first >= m_First 
             && test->second.first < m_Last
            ) {
                try {
                    test->second.second->run(
                        getVerbosity()
                      , m_OutputFunction
                      , indent
                      , m_TestLevel
                    );
                    failAdd(test->second.second->getFailed());
                    passAdd(test->second.second->getPassed());
                    notImplementedAdd(test->second.second->getNotImplemented());
                } catch (const std::exception& err) {
                    display_error("std::exception", err.what());
                    #if 0
                    std::cerr 
                        << "test threw unexpected exception: " 
                        << err.what() 
                        << std::endl;
                    #endif
                } catch(...) {
                    display_error("unknown exception", "unknown exception");
                    #if 0
                    std::cerr << "test threw an unknown exception" << std::endl;
                    #endif
                }
            }
        }
    } 
    
    //--------------------------------------------------------------------------
    //  If --file was not specified on the command line, rull all registered    
    //  tests.                                                                  
    //--------------------------------------------------------------------------
    else {
        for (auto & test : *this) {
            if (test.second.first >= m_First && test.second.first < m_Last) {
                try {
                    test.second.second->run(
                        getVerbosity()
                      , m_OutputFunction
                      , indent
                      , m_TestLevel
                    );
                    failAdd(test.second.second->getFailed());
                    passAdd(test.second.second->getPassed());
                    notImplementedAdd(test.second.second->getNotImplemented());
                } catch(const std::exception& err) {
                    display_error("std::exception", err.what());
                    #if 0
                    std::cerr 
                        << "test threw unexpected exception: " 
                        << err.what() 
                        << std::endl;
                    #endif
                } catch(...) {
                    display_error("unknown exception", "unknown exception");
                    #if 0
                    std::cerr << "test threw an unknown exception" << std::endl;
                    #endif
                }
            }
        }
    }

} // void Singleton::runTest() //                                               

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool registerTestObject(
    Test* testobject
  , const std::string& filename
) {
    Singleton* p = Singleton::instance();

    std::string root(filenameRoot(filename));

    p->insert(
        FileNameTestMapElement(root, RegisterIDTest(int(p->size()),testobject))
    );
    return p != nullptr && testobject != nullptr;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void run(
    int argc
  , char** argv
  , Test::OutputFunction outputFunc
  , const std::string& indentLevel
  , int first
  , int last
) {
    Singleton::instance()->run(argc,argv,outputFunc,indentLevel,first,last);
}

//------------------------------------------------------------------------------
///@brief A simple class to ensure that the singleton is destroyed when the     
///       application terminates.                                               
///                                                                             
///@par What's going on here?!?                                                 
///         Have you ever noticed that people using that singleton pattern      
///         above merrily create the object whenever it's needed, but completely
///         fail to delete it when the application terminates?  Jerks.          
///\n\n                                                                         
///         Okay, maybe they only have to worry about allocating memory or      
///         taking out database locks and are relying on the system to magically
///         clean up their mess when the process termintes.  Good for them.     
///         In our world (Test) it is normal for someone to create a            
///         temporary directory and populate it when the object is created or   
///         used with the assuption that the destructor will delete the         
///         directory.  If we don't clean up after ourselves, we leave all those
///         temporary directories and files laying around.                      
///\n\n                                                                         
///         Okay, why did I spend so much time justifying something that should 
///         be standard practice that singleton users tend to overlook?  Sigh.  
///                                                                             
///@par Execution Order                                                         
///         @li The @e cleanup global variable is created at application        
///             startup (before main is called).                                
///         @li The Cleanup constructor is non-existent and the object just     
///             hangs around until the application terminates gracefully.       
///         @li After main is left (gracefully), the cleanup destructor is      
///             called -- which calls Singleton::destroy in order to delete the 
///             global singleton.                                               
///                                                                             
///@note    Just as the order of creation / initialization of global objects    
///         should be viewed as random, the destruction should also be viewed   
///         as random. If a global variable's destructor relies on the Singleton
///         still being in existence, then indeterminant results will happen    
///         based on whether that variable is destroyed before or after         
///         @e cleanup is destroyed.                                            
//------------------------------------------------------------------------------
class Cleanup {
    public:
        ~Cleanup() {Singleton::destroy(); }
} cleanup;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void cout(const std::string& message)
{
    std::cout << message << std::endl;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void cerr(const std::string& message)
{
    std::cerr << message << std::endl;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int64_t registeredCount()
{
    return Singleton::instance()->size();
}

//------------------------------------------------------------------------------
///  Return the number of tests that have passed.                               
//------------------------------------------------------------------------------
int64_t passed()
{
    return Singleton::instance()->getPassed();
}

//------------------------------------------------------------------------------
///  Return the number of tests that have failed.                               
//------------------------------------------------------------------------------
int64_t failed()
{
    return Singleton::instance()->getFailed();
}

//------------------------------------------------------------------------------
///@brief   Return the number of tests that have not been implemented.          
//------------------------------------------------------------------------------
int64_t notImplemented()
{
    return Singleton::instance()->getNotImplemented();
}

} // namespace work //                                                          
} // namespace test //                                                          
} // namespace dev //                                                           

