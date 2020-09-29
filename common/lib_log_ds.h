#ifndef LIB_LOG_DS_H_FILE_GUARD                                                 
#define LIB_LOG_DS_H_FILE_GUARD
//------------------------------------------------------------------------------
///                                                                             
///@file lib_log_ds.h                                                           
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Hold the data strcutures (and types) for the lib::log facility.     
///                                                                             
///@todo    Make use of lib_ds_enum.h                                           
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2019-10-02  KJS/DHF Added MSG_INSUFFICIENT_DATA                     
///                                                                             
///@version 2018-09-11  DMN     Added MSG_BUS_MAP                               
///                                                                             
///@version 2018-11-06  MNT     Added READ/WRITE_FAILURE/SUCCESS                
///                                                                             
///@version 2016-12-22  MNT     Added COMMAND_LINE_PARAMETER_MISSING            
///                                                                             
///@version 2016-11-07  DHF     Added Kaylee Sheppeard                          
///                                                                             
///@version 2016-04-07  MNT     Added OUTPUT_PATH_EQUALS_INPUT_PATH             
///                                                                             
///@version 2015-08-25  YBS     Added MSG_FILE_EXISTS                           
///                                                                             
///@version 2015-04-14  MNT     Changed MESSAGESIGNATURE to MESSAGEFILTERRULE.  
///                                                                             
///@version 2014-03-25  DHF     Merged in Jason Hall and Jason Halbur's IDs.    
///                                                                             
///@version 2013-10-08  DHF     Added standard message IDs.                     
///                                                                             
///@version 2012-12-10  DHF/AGB Changed error codes to the following bit layout:
///                                 -   16 bits facility (really class)         
///                                 -    5 bits reserved                        
///                                 -    3 bits severity                        
///                                 -    8 bits error code                      
///                                                                             
///@version 2012-12-10  DHF     Added class_t.                                  
///                                                                             
///@version 2012-09-21  DHF     Moved operator>> & operator<< to lib::log::work 
///                                                                             
///@version 2012-04-24  AGB     Added UNDEFINED to the level_t enum.  Added     
///                             functions enable reading level_t values from    
///                             lib::CommandLine and lib::ConfigurationFile     
///                             objects.                                        
///                                                                             
///@version 2011-08-02  DHF     Added ApplicationRun structure.                 
///                                                                             
///@version 2011-05-18  DHF     File creation basd on old lib::Log class.       
///                                                                             
//------------------------------------------------------------------------------

#include "lib_time_work_datetime.h"

#include <stdint.h>

namespace lib {
namespace log {
namespace ds {

//------------------------------------------------------------------------------
///@brief   Define the "m_Class" code identifitying the C++ class that          
///         generated the error.  @see MessageID.                               
///                                                                             
///@note    In general, we do @e not like to have an uber file that must be     
///         modified and maintained that knows so much about the system at      
///         large.  However, we need a mechanism that will coordinate error     
///         "facility" codes that map back to the class that generated them.    
///         Hence, this file / enumerated type.                                 
///                                                                             
///@warning Do @e not change the values of class_t.  @e Always append to the end
///         of the appropriate area.  These values will eventually be stored in 
///         a database that cannot be changed by a simple recompile.            
///                                                                             
//------------------------------------------------------------------------------
using class_t = uint16_t;

static const class_t MAIN                                              = 0x0001;
static const class_t LIB_MP_THREADABLE_COLLECTION                      = 0x0002;
static const class_t LIB_MP_THREADINFO                                 = 0x0003;
static const class_t LIB_LOG_WORK_EXCEPTION                            = 0x0004;
static const class_t LIB_WORK_FILE                                     = 0x0005;
static const class_t CLASS_UNKNOWN                                     = 0xFFFF;


//------------------------------------------------------------------------------
///@brief Relative level of the log message.                                    
///                                                                             
///@par Definitions and Usage                                                   
///         -   SILENT          Produce no output.  No message should be at this
///                             level.                                          
///         -   FATAL           The program cannot continue.  There's no ability
///                             to guess at this point.  The program must die.  
///                             Messages should be FATAL when the system must   
///                             terminate. E.g., the input file could not be    
///                             found).                                         
///         -   CRITICAL        In an interactive session, the program should   
///                             terminate (with a message letting the user know 
///                             why) but in a batch version, the program should 
///                             make some form of educated guess and keep going.
///                             @todo  This of a good example here.             
///         -   WARNING         Not fatal, but not good.  E.g., the output file 
///                             doesn't conform to standards.  That's not enough
///                             reason to terminate the program -- just scold   
///                             the user a bit.                                 
///         -   INFORMATIONAL   Information that the user probably wants to see 
///                             every run.  E.g., % complete.                   
///         -   VERBOSE         Information that the user probably does not     
///                             want to see on every run.  E.g., where things   
///                             went wrong within the input file.               
///         -   DEBUG_LEVEL     Information that only the developer would care  
///                             about.  E.g., trace statements.                 
//------------------------------------------------------------------------------
enum level_t {
    SILENT          = 0         ///< shut up already; don't produce silent msg. 
  , FATAL                       ///< processing cannot continue                 
  , CRITICAL                    ///< nearly fatal; can guess and continue       
  , WARNING                     ///< something isn't right.                     
  , INFORMATIONAL               ///< you may be interested in this              
  , VERBOSE                     ///< blah, blah, blah, blah, blah               
  , DEBUG_LEVEL_1               ///< programmer info, level 1                   

  , NORMAL          = INFORMATIONAL ///< normally, display warnings     
  , LEVEL_MAX       = DEBUG_LEVEL_1

  , UNDEFINED       = 0xFF

};

typedef uint32_t    messageid_t;

union MessageID {
    messageid_t     m_ID;

    struct {
        messageid_t   m_Class:16;
        messageid_t   m_Reserved:5;
        messageid_t   m_Severity:3;
        messageid_t   m_Code:8;
    };

    MessageID() : m_ID(0)  { }
    MessageID(messageid_t x) : m_ID(x) { }
    MessageID(uint16_t c, level_t l, uint8_t code) 
        : m_Class(c)
        , m_Reserved(0)
        , m_Severity(l)
        , m_Code(code)
    {
    }
};

//------------------------------------------------------------------------------
///@warning     Add new values at will but do NOT change existing values.       
///             They are written in stone!!                                     
///                                                                             
///@warning     Initially we're alphabetizing the values, in the future you will
///             want to append the values to the end so that the next poor slob 
///             doesn't have to hunt down what the next available value is.     
///                                                                             
///@note        We're putting the values in descending order starting at 0xFF   
///             to reduce the chances of collision with class specific messages 
///             that are defined in ascending order starting at 0.              
///                                                                             
///@note        Ideally we'd like to use "extern const messageid_t" instead of  
///             "static const messageid_t"; however, in order to use the values 
///             in constant expressions at compile time, we need to declare them
///             as "static const messageid_t"                                   
///                                                                             
//------------------------------------------------------------------------------
static const messageid_t APPLICATION_STARTED                (0xFF);
static const messageid_t APPLICATION_EXIT_SUCCESS           (0xFE);
static const messageid_t APPLICATION_EXIT_FAILURE           (0xFD);

static const messageid_t COMMAND_LINE                       (0xFC);
static const messageid_t VALUE_ERROR                        (0xFB);

static const messageid_t FILE_NAME_NOT_STANDARD             (0xFA);
static const messageid_t FILE_NAME_NOT_SPECIFIED            (0xF9);
static const messageid_t FILE_OPEN_READ_SUCCESS             (0xF8);
static const messageid_t FILE_OPEN_READ_FAILURE             (0xF7);
static const messageid_t FILE_OPEN_WRITE_SUCCESS            (0xF6);
static const messageid_t FILE_OPEN_WRITE_FAILURE            (0xF5);
static const messageid_t FILE_WRONG_TYPE                    (0xF4);

static const messageid_t LOGIC_ERROR_MSG                    (0xF3);

static const messageid_t TIME_FIRST                         (0xF1);
static const messageid_t TIME_FIRST_SYNC                    (0xF0);
static const messageid_t TIME_LAST                          (0xEF);
static const messageid_t TIME_LAST_SYNC                     (0xEE);

static const messageid_t VERSION                            (0xED);

static const messageid_t UNKNOWN_MESSAGE                    (0xEC);

static const messageid_t XML_LOAD_FAILURE                   (0xEB);
static const messageid_t XML_SCHEMA_ERROR                   (0xEA);

static const messageid_t NOT_INITIALIZED                    (0xE9);

static const messageid_t MSG_FILE_EXISTS                    (0xE8);

static const messageid_t OUTPUT_PATH_EQUALS_INPUT_PATH      (0xE7);

static const messageid_t COMMAND_LINE_PARAMETER_MISSING     (0xE6);

static const messageid_t READ_FAILURE                       (0xE5);
static const messageid_t READ_SUCCESS                       (0xE4);
static const messageid_t WRITE_FAILURE                      (0xE3);
static const messageid_t WRITE_SUCCESS                      (0xE2);

static const messageid_t MSG_BUS_MAP                        (0xE1);

static const messageid_t MSG_INSUFFICIENT_DATA              (0xE0);

//------------------------------------------------------------------------------
///@todo    These should be remvoed when lib_ds_enum.h is used.                 
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, lib::log::ds::level_t e);
std::istream& operator>>(std::istream& in, lib::log::ds::level_t& e);

} // namespace ds   
} // namespace log  
} // namespace lib  

#endif // #ifndef LIB_LOG_DS_H_FILE_GUARD
