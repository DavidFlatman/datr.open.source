//------------------------------------------------------------------------------
///@file lib_log_work_messagefactory.h                                          
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_LOG_MESSAGEFACTORY_H_FILE_GUARD
#define LIB_LOG_MESSAGEFACTORY_H_FILE_GUARD

#include "lib_time_work_datetime.h"
#include "lib_log_ds.h"
#include "lib_log_work_message.h"
#include <stdlib.h>                 // EXIT_SUCCESS 

namespace lib {
namespace log {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: MessageFactory                                                   
///                                                                             
///@brief   Factory for generating lib::log::Messages with defaults set.        
///                                                                             
///@par Thread Safety:  object                                                  
///                                                                             
///@note    In the initial implementation the factory is going to produce       
///         ConstMessagePtr.  If we find that unworkable, chagne it.            
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             lib::log::MessageFactory factory("command-line");               
///                                                                             
///             // ... snip ...                                                 
///             if (missing_required_parameter) {                               
///                 publish(                                                    
///                     factory.fatal(MSG_ID_MISSING, "missing param")          
///                 );                                                          
///             }                                                               
///                                                                             
///             // ... snip ...                                                 
///             if (unknown_parameter) {                                        
///                 publish(                                                    
///                     factory.warning(MSG_ID_UNKNOWN, "unknown parameter")    
///                 );                                                          
///             }                                                               
///         @endcode                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-10-02  DHF     lib::DateTime moved to lib::time::work::DateTime
///                                                                             
///@version 2013-10-08  DHF     Moved message id values into lib::log::ds.      
///                                                                             
///@version 2013-10-03  DHF     Removed deprecated facility.                    
///                                                                             
///@version 2011-06-08  DHF     Added command line.                             
///                                                                             
///@version 2011-05-18  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class MessageFactory
{
    public:
        MessageFactory(ds::class_t classid);
        inline MessageFactory(const MessageFactory& that) 
            : m_ClassID(that.m_ClassID)
            { }
        virtual ~MessageFactory();
        MessageFactory& operator=(const MessageFactory& that)
            { m_ClassID = that.m_ClassID; return *this; }

        ConstMessagePtr fatal(
            ds::messageid_t id
          , const std::string& msg
        ) const;

        ConstMessagePtr critical(
            ds::messageid_t id
          , const std::string& msg
        ) const;

        ConstMessagePtr warning(
            ds::messageid_t id
          , const std::string& msg
        ) const;

        ConstMessagePtr informational(
            ds::messageid_t id
          , const std::string& msg
        ) const;
        ConstMessagePtr  operator()(
            ds::messageid_t id, const std::string& msg
        ) const { return informational(id, msg); }
            
        ConstMessagePtr verbose(
            ds::messageid_t id
          , const std::string& msg
        ) const;

        ConstMessagePtr debug(
            ds::messageid_t id
          , const std::string& msg
        ) const;

        ConstMessagePtr message(
            ds::level_t level
          , ds::messageid_t id
          , const std::string& msg
        ) const;

        //----------------------------------------------------------------------
        //  Standard messages                                                   
        //----------------------------------------------------------------------
        ConstMessagePtr applicationStarted() const;
        ConstMessagePtr applicationExitSuccess() const;
        ConstMessagePtr applicationExitFailure() const;

        ConstMessagePtr commandLine(int argc, char* argv[]) const;
        ConstMessagePtr commandLine(int argc, const char* argv[]) const;

        ConstMessagePtr fileNameNotStandard(const std::string& filename) const;

        ConstMessagePtr fileNameNotSpecified(
            const std::string& filetype
          , lib::log::ds::level_t   severity = lib::log::ds::FATAL
        ) const;
        ConstMessagePtr fileOpenReadSuccess(const std::string& filename) const;
        ConstMessagePtr fileOpenReadFailure(const std::string& filename) const;
        ConstMessagePtr fileOpenWriteSuccess(const std::string& filename) const;
        ConstMessagePtr fileOpenWriteFailure(const std::string& filename) const;
        ConstMessagePtr fileWrongType(
            const std::string& filename
          , const std::string& expected_type
        ) const;

        ConstMessagePtr tailNumberUnknownWarning() const;
        ConstMessagePtr tailNumberUnknownFatal() const;

        ConstMessagePtr version() const;

        ConstMessagePtr timeFirst(
            const lib::time::work::DateTime& time
          , const std::string& filename
          , const std::string& comment = ""
        ) const;
        ConstMessagePtr timeFirstSynced(
            const lib::time::work::DateTime& time
          , const std::string& filename
          , const std::string& comment = ""
        ) const;
        ConstMessagePtr timeLast(
            const lib::time::work::DateTime& time
          , const std::string& filename
          , const std::string& comment = ""
        ) const;
        ConstMessagePtr timeLastSynced (
            const lib::time::work::DateTime& time
          , const std::string& filename
          , const std::string& comment = ""
        ) const;



    protected:

    private:
        //----------------------------------------------------------------------
        //  If you need 'em, code 'em.  But if you do, I will snicker at your   
        //  design.                                                             
        //----------------------------------------------------------------------

        lib::log::ds::class_t m_ClassID;

}; // class MessageFactory //

} // namespace work 
} // namespace log  
} // namespace lib  

#endif // #ifndef LIB_LOG_MESSAGEFACTORY_H_FILE_GUARD

