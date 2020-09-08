//------------------------------------------------------------------------------
///@file lib_log_work_messagefactory.cpp                                        
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Factory for generating Messages with defaults set.                  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2014-10-02  DHF     lib::DateTime moved to lib::time::work::DateTime
///                                                                             
///@version 2013-10-08  DHF     Refactored standard message values into         
///                             lib::log::ds.                                   
///                                                                             
///@version 2011-05-18  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_log_work_messagefactory.h"
#include "lib_string.h"
#include "lib_work_version.h"

namespace lib {
namespace log {
namespace work {

//------------------------------------------------------------------------------
///@brief   Construct the message factory.                                      
//------------------------------------------------------------------------------
MessageFactory::MessageFactory(ds::class_t classid) 
  : m_ClassID(classid)
{

} 

//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
MessageFactory::~MessageFactory()
{

} 

//------------------------------------------------------------------------------
///@brief   Generate a FATAL message.                                           
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fatal(
    ds::messageid_t id
  , const std::string& msg
) const
{
    return message(ds::FATAL, id, msg);

}

//------------------------------------------------------------------------------
///@brief   Generate a CRITICAL message.                                        
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::critical(
    ds::messageid_t id
  , const std::string& msg
) const
{
    return message(ds::CRITICAL, id, msg);

}

//------------------------------------------------------------------------------
///@brief   Generate a WARNING message.                                         
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::warning(
    ds::messageid_t id
  , const std::string& msg
) const
{
    return message(ds::WARNING, id, msg);

}

//------------------------------------------------------------------------------
///@brief   Generate an INFORMATIONAL message.                                  
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::informational(
    ds::messageid_t id
  , const std::string& msg
) const
{
    return message(ds::INFORMATIONAL, id, msg);

} 

//------------------------------------------------------------------------------
///@brief   Generate a VERBOSE message.                                         
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::verbose(
    ds::messageid_t id
  , const std::string& msg
) const
{
    return message(ds::VERBOSE, id, msg);

} 

//------------------------------------------------------------------------------
///@brief   Generate a DEBUG_LEVEL_1 message.                                   
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::debug(
    ds::messageid_t id
  , const std::string& msg
) const
{
    return message(ds::DEBUG_LEVEL_1, id, msg);

} 

//------------------------------------------------------------------------------
///@brief   Generate a message of the given level, id, and message.             
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::message(
    ds::level_t level
  , ds::messageid_t id
  , const std::string& msg
) const
{
    ConstMessagePtr ans(new Message(m_ClassID, level, id, msg));
    return ans;

}

//------------------------------------------------------------------------------
///@brief   Generated a standard application started message.                   
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::applicationStarted() const
{
    return verbose(ds::APPLICATION_STARTED, "application started");
}

//------------------------------------------------------------------------------
///@brief   Generated a standard application ending message.                    
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::applicationExitSuccess() const
{
    return verbose(
        ds::APPLICATION_EXIT_SUCCESS
      , "application ending with success"
    );
}

//------------------------------------------------------------------------------
///@brief   Generated a standard application ending message.                    
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::applicationExitFailure() const
{
    return warning(
        ds::APPLICATION_EXIT_FAILURE
      , "application exit with failure"
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard message containing the command line.            
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::commandLine(int argc, const char* argv[]) const
{
    std::string message;

    for (int a=0; a < argc; ++a) {
        message += "(" + format("%d", a) + ")\"" + std::string(argv[a]) + "\"";
    }

    return verbose(ds::COMMAND_LINE, message);
}

//------------------------------------------------------------------------------
///@brief   Generate a standard message containing the command line.            
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::commandLine(int argc, char* argv[]) const
{
    std::string message;

    for (int a=0; a < argc; ++a) {
        message += "(" + format("%d", a) + ")\"" + std::string(argv[a]) + "\"";
    }

    return verbose(ds::COMMAND_LINE, message);
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file name not standard warning message.         
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileNameNotStandard(
    const std::string& filename
) const
{
    return warning(
        ds::FILE_NAME_NOT_STANDARD
      , filename + " does not conform to file naming standards"
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file name not specified message.                
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileNameNotSpecified(
    const std::string& filetype
  , lib::log::ds::level_t severity
) const
{
    return message(
        severity
      , ds::FILE_NAME_NOT_SPECIFIED
      , filetype + " not specified."
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file open for read successfully message.        
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileOpenReadSuccess(
    const std::string& filename
) const
{
    return verbose(
        ds::FILE_OPEN_READ_SUCCESS
      , "successfully opened " + filename + " for read"
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file opened for read failed message.            
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileOpenReadFailure(
    const std::string& filename
) const
{
    return fatal(
        ds::FILE_OPEN_READ_FAILURE
      , "failed to open " + filename + " for read"
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file opened for write successfully message.     
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileOpenWriteSuccess(
    const std::string& filename
) const
{
    return verbose(
        ds::FILE_OPEN_WRITE_SUCCESS
      , "successfully opened " + filename + " for write"
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file opened for write failed message.           
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileOpenWriteFailure(
    const std::string& filename
) const
{
    return fatal(
        ds::FILE_OPEN_WRITE_FAILURE
      , "failed to open " + filename + " for write"
    );
}

//------------------------------------------------------------------------------
///@brief   Generate a standard file is of the wrong type message.              
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::fileWrongType(
    const std::string& filename
  , const std::string& expected_type
) const
{
    return fatal(
        ds::FILE_WRONG_TYPE
      , "file " + filename + " does not appear to be a " + expected_type
    );
}


//------------------------------------------------------------------------------
///@brief   Generated a standard application version message.                   
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::version() const
{
    return informational(ds::VERSION, "DATR " + lib::work::version());
}

//------------------------------------------------------------------------------
///@brief   Create a message indicating the first time in the file.             
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::timeFirst(
    const lib::time::work::DateTime& time
  , const std::string& filename
  , const std::string& comment
) const
{
    return informational(
        ds::TIME_FIRST
      , time.toString("%Y %H:%M:%S.%%6f")
      + " first time in " + filename + " " + comment
    );
}

//------------------------------------------------------------------------------
///@brief   Create a message indicating the first synchronized time in the file.
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::timeFirstSynced(
    const lib::time::work::DateTime& time
  , const std::string& filename
  , const std::string& comment
) const
{
    return informational(
        ds::TIME_FIRST_SYNC
      , time.toString("%Y %H:%M:%S.%%6f")
      + " first synchronized time in " + filename + " " + comment
    );
}

//------------------------------------------------------------------------------
///@brief   Create a message indicating the last time in the file.              
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::timeLast(
    const lib::time::work::DateTime& time
  , const std::string& filename
  , const std::string& comment
) const
{
    return informational(
        ds::TIME_LAST
      , time.toString("%Y %H:%M:%S.%%6f")
      + " last time in " + filename + " " + comment
    );
}

//------------------------------------------------------------------------------
///@brief   Create a message indicating the last synchronized time in the file. 
//------------------------------------------------------------------------------
ConstMessagePtr MessageFactory::timeLastSynced (
    const lib::time::work::DateTime& time
  , const std::string& filename
  , const std::string& comment
) const
{
    return informational(
        ds::TIME_LAST_SYNC
      , time.toString("%Y %H:%M:%S.%%6f")
      + " last synchronized time in " + filename + " " + comment
    );
}

} // namespace work //
} // namespace log //
} // namespace lib //

