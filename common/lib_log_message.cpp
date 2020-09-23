//------------------------------------------------------------------------------
///                                                                             
///@file lib_log_message.cpp                                                    
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
///                                                                             
///@brief   Object respenting a single log message.                             
///                                                                             
///@version 2017-02-16  DHF     Modified Messages empty severity level to       
///                             UNDEFINED.                                      
///                                                                             
///@version 2014-10-02  DHF     lib::DateTime moved to lib::time::work::DateTime
///                                                                             
///@version 2013-11-04  DHF     Changed output format of code to                
///                             %02x-%02x-%02x-%c to make it easier for end-user
///                             error reporting.                                
///                                                                             
///@version 2013-10-03  DHF     Removed references to facility.                 
///                                                                             
///@version 2012-??-??  AGB     Add support for Visual Studio.                  
///                                                                             
///@version 2011-11-17  DHF     Added setSeverityLevel(const std::string&);     
///                                                                             
///@version 2011-03-17  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_log_message.h"
#include "lib_log_work.h"
#include "lib_si_prefixes.h"
#include "lib_string.h"

#include <boost/algorithm/string.hpp>

#ifdef IS_VISUAL_STUDIO
#include <process.h>            //  Microsoft is not standard.  
#else
#include <unistd.h>             // getpid                       
#endif

#include "debug.h"

namespace lib {
namespace log {

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
std::string  Message::s_ApplicationMnemonic("not-set");

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Message::Message() 
  : m_TimeStamp(lib::time::work::DateTime::now())
  , m_ApplicationMnemonic(s_ApplicationMnemonic)
  , m_ClassID(ds::CLASS_UNKNOWN)
//  , m_FacilityMnemonic("not_set")
  , m_SeverityLevel(ds::WARNING)
  , m_MessageID(0)
  , m_Message("default message")
  , m_PID(uint32_t(getpid()))
{
    m_TimeStamp.truncateSecondsTo(lib::si::micro);

}

//------------------------------------------------------------------------------
///@note    Refactor away if Microsoft ever figures out that "= default" is     
///         supposed to mean.                                                   
//------------------------------------------------------------------------------
Message::Message(const Message& that)
  : m_TimeStamp(that.m_TimeStamp)
  , m_ApplicationMnemonic(that.m_ApplicationMnemonic)
  , m_ClassID(that.m_ClassID)
//  , m_FacilityMnemonic(that.m_FacilityMnemonic)
  , m_SeverityLevel(that.m_SeverityLevel)
  , m_MessageID(that.m_MessageID)
  , m_Message(that.m_Message)
  , m_PID(that.m_PID)

{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Message::Message(const std::string& str)
{
    *this = str;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Message::Message(
    ds::class_t                 classID
  , ds::level_t                 severity    
  , ds::messageid_t             message_id  
  , const std::string&          message     
  , lib::time::work::DateTime   time_stamp  
  , const std::string&          appMnemonic 
) : m_TimeStamp(time_stamp)
  , m_ApplicationMnemonic(appMnemonic != "" ? appMnemonic : s_ApplicationMnemonic)
  , m_ClassID(classID)
  , m_SeverityLevel(severity)
  , m_MessageID(message_id)
  , m_Message(message)
  , m_PID(uint32_t(getpid()))
{
    m_TimeStamp.truncateSecondsTo(lib::si::micro);

} // Message:: //
    
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Message::Message(
    ds::class_t                 classID
  , const std::string&          severity    
  , ds::messageid_t             message_id  
  , const std::string&          message     
  , lib::time::work::DateTime   time_stamp  
  , const std::string&          appMnemonic 
) : m_TimeStamp(time_stamp)
  , m_ApplicationMnemonic(appMnemonic != "" ? appMnemonic : s_ApplicationMnemonic)
  , m_ClassID(classID)
  , m_MessageID(message_id)
  , m_Message(message)
  , m_PID(uint32_t(getpid()))
{
    setSeverityLevel(severity);
    m_TimeStamp.truncateSecondsTo(lib::si::micro);

} // Message:: //
    
//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
Message::~Message()
{

} // Message::~Message

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Message::setDefaultApplicationMnemonic(const std::string& mnemonic)
{
    s_ApplicationMnemonic = mnemonic;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
std::string Message::defaultApplicationMnemonic()
{
    return s_ApplicationMnemonic;

} 

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Message::setTimeStamp(const time::work::DateTime& when) 
{
    m_TimeStamp = when;
    m_TimeStamp.truncateSecondsTo(lib::si::micro);

} // void Message::setTimeStamp(const time::work::DateTime& = time::work::DateTime::now()) //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
time::work::DateTime Message::timeStamp() const
{
    return m_TimeStamp;

} // time::work::DateTime Message::timeStamp() const //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Message::setApplicationMnemonic(const std::string& app)
{
    m_ApplicationMnemonic = app;

} // void Message::setApplicationMnemonic(const std::string& app) //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
std::string Message::applicationMnemonic() const
{
    return m_ApplicationMnemonic;

} // std::string Message::applicationMnemonic() const //

//------------------------------------------------------------------------------
///@brief   Set the severity level based on the given string.                   
///@return  true = the string was understood and parsed; false = string not     
///         understood.                                                         
//------------------------------------------------------------------------------
bool Message::setSeverityLevel(const std::string& s)
{
    bool result = true;

    //--------------------------------------------------------------------------
    //  Convert the value to lower case -- or if it is a digit, leave it alone. 
    //  ASCII '0' is 0x30  0011 0000                                            
    //  ASCII '9' is 0x39  0011 1001                                            
    //  ASCII 'A' is 0x41  0100 0001                                            
    //  ASCII 'Z' is 0x5A  0101 1010                                            
    //  ASCII 'a' is 0x61  0110 0001                                            
    //  ASCII 'z' is 0x6A  0111 1010                                            
    //--------------------------------------------------------------------------
    switch(s[0] | 0x20) {
        case 'f':   setSeverityLevel(ds::FATAL);            break;
        case 'c':   setSeverityLevel(ds::CRITICAL);         break;
        case 'w':   setSeverityLevel(ds::WARNING);          break;
        case 'i':   setSeverityLevel(ds::INFORMATIONAL);    break;
        case 'v':   setSeverityLevel(ds::VERBOSE);          break;
        case 'd':   setSeverityLevel(ds::DEBUG_LEVEL_1);    break;
        default:    setSeverityLevel(ds::FATAL); result = false; break;
    }

    return result;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Message::setSeverityLevel(ds::level_t level)
{
    m_SeverityLevel = level;

} // void Message::setSeverityLevel(ds::level_t level) //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
ds::level_t Message::severityLevel() const
{
    return m_SeverityLevel;

} // ds::level_t Message::severityLevel() const //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Message::setClassID(lib::log::ds::class_t cls)
{
    m_ClassID = cls;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
lib::log::ds::class_t Message::classID() const
{
    return m_ClassID;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Message::setMessageID(ds::messageid_t num)
{
    m_MessageID = num;

} // void Message::setMessageID(ds::messageid_t num) //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
ds::messageid_t Message::messageID() const
{
    return m_MessageID;

} // ds::messageid_t Message::messageID() const //

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Message::setMessage(const std::string& msg)
{
    m_Message = msg;

} // Message::void setMessage(const std:;string& fac) //

//------------------------------------------------------------------------------
///@brief   The process identifier that generated the message.                  
//------------------------------------------------------------------------------
int Message::pid() const
{
    return m_PID;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string Message::toString(PARTS part) const
{
    std::string result;

    #define IF(which_part, append)                                          \
        if (part & which_part) {                                            \
            result += append + " ";                                         \
        }

    std::string spaces(80, ' ');

    IF (TIME_STAMP, timeStamp().toString());
    IF (PID, lib::format("%04" PRIx32, m_PID));
    IF (MNEMONIC, (applicationMnemonic() + spaces).substr(0,20));

    std::string message_id = format(
        "%02x-%02x-%02x-%c"
      , (m_ClassID >> 8) & 0xFF
      , (m_ClassID >> 0) & 0xFF
      , messageID() & 0xFF
      , lib::log::work::toString(m_SeverityLevel)[0]
    );

    IF (MESSAGE_ID, message_id);

    IF (MESSAGE_TEXT, message());

    //--------------------------------------------------------------------------
    // Trim off the extra space on the end ...                                  
    //--------------------------------------------------------------------------
    result = left(result, result.length()-1);

    #undef IF

    return result;

}

//------------------------------------------------------------------------------
///@note    Refactor away if Microsoft ever figures out that "= default" is     
///         supposed to mean.                                                   
//------------------------------------------------------------------------------
Message& Message::operator=(const Message& that)
{
    m_TimeStamp             = that.m_TimeStamp;
    m_ApplicationMnemonic   = that.m_ApplicationMnemonic;
    m_SeverityLevel         = that.m_SeverityLevel;
    m_ClassID               = that.m_ClassID;
    m_MessageID             = that.m_MessageID;
    m_Message               = that.m_Message;
    m_PID                   = that.m_PID;

    return *this;

}

//------------------------------------------------------------------------------
//  str.substr(0,26)            time string                                     
//  str.substr(26, 1)           blank character                                 
//  str.substr(27, 4)           process ID (%04x)                               
//  str.substr(31, 1)           blank character                                 
//  str.substr(32, 20)          application mnemonic                            
//  str.substr(52, 1)           blank character                                 
//  str.substr(53, 2)           class ID (upper %02x)                           
//  str.substr(55, 1)           dash (-)                                        
//  str.substr(56, 2)           class ID (lower %02x)                           
//  str.substr(58, 1)           dash (-)                                        
//  str.substr(59, 2)           message ID (%02x)                               
//  str.substr(61, 1)           dash (-)                                        
//  str.substr(62)              severity code                                   
//  str.substr(63)              blank character                                 
//  str.substr(64)              text message                                    
//------------------------------------------------------------------------------
Message& Message::operator=(const std::string& str)
{
    if (str.size() < 64) throw "too small";
    m_TimeStamp = str.substr(0,26);
    m_PID       = toUnsigned("0x" + str.substr(27,4));
    m_ApplicationMnemonic = str.substr(32, 20);
    uint32_t cid(0);
    cid |= (toUnsigned("0x" + str.substr(53,2)) << 8);
    cid |= (toUnsigned("0x" + str.substr(56,2)));
    m_ClassID = ds::class_t(cid);
    m_MessageID = toUnsigned("0x" + str.substr(59, 2));
    m_SeverityLevel = lib::log::work::fromString(str.substr(62,1));
    m_Message = str.substr(64, str.size());

    boost::trim(m_ApplicationMnemonic);

    return *this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const Message& m)
{
    return out << m.toString();

}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& in, Message& m)
{
    char buffer[8192];
    in.getline(buffer, sizeof(buffer));
    m = std::string(buffer);
    
    return in;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
std::string Message::message() const
{
    return m_Message;

} // std::string Message::message() const //

//------------------------------------------------------------------------------
///@brief   Return true if this message is worse than the given severity level. 
///@note    FATAL is worse than SEVERE which is worse than WARNING ...          
//------------------------------------------------------------------------------
bool Message::worseThan(ds::level_t level) const
{
    return m_SeverityLevel < level;
}

//------------------------------------------------------------------------------
///@brief   Return true if this message is worse than the given severity level. 
///@note    FATAL is worse than SEVERE which is worse than WARNING ...          
//------------------------------------------------------------------------------
bool Message::asBadOrWorseThan(ds::level_t level) const
{
    return m_SeverityLevel <= level;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
Messages::Messages()
    : m_HighestSeverityLevel(ds::UNDEFINED)
{
}

//------------------------------------------------------------------------------
///@note    Refactor away if Microsoft ever figures out that "= default" is     
///         supposed to mean.                                                   
//------------------------------------------------------------------------------
Messages::Messages(const Messages& msgs)
    : std::vector<ConstMessagePtr>(msgs)
    , m_HighestSeverityLevel(msgs.m_HighestSeverityLevel)
{
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Messages::push_back(ConstMessagePtr msg)
{
    if (msg->severityLevel() == ds::SILENT) {
        //----------------------------------------------------------------------
        // ignore it                                                            
        //----------------------------------------------------------------------

    } else if (msg->severityLevel() < m_HighestSeverityLevel) {
        m_HighestSeverityLevel = msg->severityLevel();

    }

    std::vector<ConstMessagePtr>::push_back(msg);
        
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Messages::push_back(const Messages& msg)
{
    for (Messages::const_iterator m = msg.begin(); m != msg.end(); ++m) {
        push_back(*m);
    }
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Messages::clear()
{
    std::vector<ConstMessagePtr>::clear();
    m_HighestSeverityLevel = ds::UNDEFINED;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
ds::level_t Messages::severityLevel() const
{
    return m_HighestSeverityLevel;
}

//------------------------------------------------------------------------------
///@brief   Return true if this message is worse than the given severity level. 
///@note    FATAL is worse than SEVERE which is worse than WARNING ...          
//------------------------------------------------------------------------------
bool Messages::worseThan(ds::level_t level) const
{
    return m_HighestSeverityLevel < level;
}

//------------------------------------------------------------------------------
///@brief   Return true if this message is worse than the given severity level. 
///@note    FATAL is worse than SEVERE which is worse than WARNING ...          
//------------------------------------------------------------------------------
bool Messages::asBadOrWorseThan(ds::level_t level) const
{
    return m_HighestSeverityLevel <= level;
}

//------------------------------------------------------------------------------
///@brief   Given a messageid_t return the index to the first message that has  
///         that messageid_t.                                                   
///@param   messageid       The messageid_t to find in the collection.          
///@return  If the messageid is found, returns the index to the first message   
///         found.\n                                                            
///         If the messageid is not found, returns -1.                          
//------------------------------------------------------------------------------
int Messages::find(ds::messageid_t messageid)
{
    int result(-1);

    for (size_t m=0; m < size() && result == -1; ++m) {
        if ((*this)[m]->messageID() == messageid) result = m;
    }

    return result;
}

//------------------------------------------------------------------------------
///@brief   Given a class_t and a messageid_t return the index to the first     
///         message that has both the class_t and messageid_t.                  
///@param   classid         The class_t   to find in the collection.            
///@param   messageid       The messageid_t to find in the collection.          
///@return  If the messageid is found, returns the index to the first message   
///         found.\n                                                            
///         If the messageid is not found, returns -1.                          
//------------------------------------------------------------------------------
int Messages::find(
    lib::log::ds::class_t   classid
  , ds::messageid_t         messageid
)
{
    int result(-1);

    for (size_t m=0; m < size() && result == -1; ++m) {
        if (
            (*this)[m]->messageID() == messageid 
         && (*this)[m]->classID() == classid
        ) 
        {
            result = m;
        }
    }

    return result;
}

} // namespace log //
} // namespace lib //

