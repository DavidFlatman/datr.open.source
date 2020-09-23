//------------------------------------------------------------------------------
///@file lib_log_message.h                                                      
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                                          
//------------------------------------------------------------------------------

#ifndef LIB_LOG_MESSAGE_H_FILE_GUARD
#define LIB_LOG_MESSAGE_H_FILE_GUARD

#include "lib_time_work_datetime.h"
#include "lib_flags.h"
#include "lib_log_ds.h"

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace lib {
namespace log {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Message                                                          
///                                                                             
///@brief   Object respenting a single log message.                             
///                                                                             
///@par Thread Safety:  class                                                   
///         Note that the static method application mnemonic is not thread-safe,
///         but it should only be called once per application instance anyway.  
///                                                                             
///@version 2014-10-02  DHF     lib::DateTime moved to lib::time::work::DateTime
///                                                                             
///@version 2014-02-12  DHF     Added find methods to class Messages.           
///                             Added accessor methods for classid to Message.  
///                                                                             
///@version 2013-10-03  DHF     Removed references to facility.                 
///                                                                             
///@version 2012-04-25  DHF     Added bool worseThan(...) to Messages class.    
///                                                                             
///@version 2011-11-17  DHF     Added setSeverityLevel(const std::string&);     
///                                                                             
///@version 2011-03-17  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Message
{
    public:
        Message();
        Message(const Message& that);
        //----------------------------------------------------------------------
        //  Warning:  This method expects a string formatted by toString ...    
        //----------------------------------------------------------------------
        explicit Message(const std::string& str);

        Message(
            lib::log::ds::class_t   classid
          , ds::level_t         severity    
          , ds::messageid_t     message_id  = 0
          , const std::string&          message     = ""
          , lib::time::work::DateTime   time_stamp  = lib::time::work::DateTime::now()
          , const std::string&  appMnemonic = ""
        );
        Message(
            lib::log::ds::class_t   classid
          , const std::string&  severity    
          , ds::messageid_t     message_id  = 0
          , const std::string&  message     = ""
          , lib::time::work::DateTime       time_stamp  = lib::time::work::DateTime::now()
          , const std::string&  appMnemonic = ""
        );

        virtual ~Message();
        Message& operator=(const Message& that);
        Message& operator=(const std::string& str);

        static void setDefaultApplicationMnemonic(const std::string& mnemonic);
        static std::string defaultApplicationMnemonic();

        void setTimeStamp(const time::work::DateTime& = time::work::DateTime::now());
        time::work::DateTime timeStamp() const;

        void setApplicationMnemonic(const std::string& app);
        std::string applicationMnemonic() const;

        bool setSeverityLevel(const std::string& str);
        void setSeverityLevel(ds::level_t level);
        ds::level_t severityLevel() const;

        void setClassID(lib::log::ds::class_t cls);
        lib::log::ds::class_t classID() const;

        void setMessageID(ds::messageid_t num);
        ds::messageid_t messageID() const;

        void setMessage(const std::string& msg);
        std::string message() const;

        enum PART_ENUM {
            TIME_STAMP      = 0x01
          , PID             = 0x02
          , MNEMONIC        = 0x04
          , FACILITY        = 0x08
          , SEVERITY        = 0x10
          , SEVERITY_FULL   = 0x20
          , MESSAGE_ID      = 0x40
          , MESSAGE_TEXT    = 0x80

          , INTERACTIVE     = SEVERITY_FULL | MESSAGE_TEXT
          , BATCH           = 0xFFFF & ~SEVERITY_FULL
          , ALL             = 0xFFFF & ~SEVERITY_FULL

        };
        typedef Flags<PART_ENUM> PARTS;

        std::string toString(PARTS part = ALL) const;

        int pid() const;

        bool worseThan(ds::level_t level) const;
        bool asBadOrWorseThan(ds::level_t level) const;

    protected:

    private:
        time::work::DateTime        m_TimeStamp;
        std::string     m_ApplicationMnemonic;
        ds::class_t     m_ClassID;
//        std::string     m_FacilityMnemonic;
        ds::level_t     m_SeverityLevel;
        ds::messageid_t m_MessageID;
        std::string     m_Message;
        int32_t         m_PID;

        static std::string  s_ApplicationMnemonic;

}; // class Message //

typedef boost::shared_ptr<Message> MessagePtr;
typedef boost::shared_ptr<const Message> ConstMessagePtr;

std::ostream& operator<< (std::ostream& out, const Message& msg);
std::istream& operator>> (std::istream& in, Message& msg);

//------------------------------------------------------------------------------
///@note    These two classes are tied enough together that they share a file.  
//------------------------------------------------------------------------------
class Messages : public std::vector<ConstMessagePtr>
{
    public:
        Messages();
        Messages(const Messages& msgs);

        void push_back(ConstMessagePtr msg);
        void push_back(const Messages& msg);
        void clear();

        inline void operator()(ConstMessagePtr msg) { push_back(msg); }
        inline void operator()(const Messages& msg) { push_back(msg); }

        ds::level_t severityLevel() const;
        bool worseThan(ds::level_t level) const;
        bool asBadOrWorseThan(ds::level_t level) const;

        int find(ds::messageid_t mesasgeid);
        int find(
            lib::log::ds::class_t   classid
          , ds::messageid_t         mesasgeid
        );

    private:

        ds::level_t   m_HighestSeverityLevel;
};

} // namespace log
} // namespace lib

#endif // #ifndef LIB_LOG_MESSAGE_H_FILE_GUARD

