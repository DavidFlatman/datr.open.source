//------------------------------------------------------------------------------
///@file lib_log_work_exception.h                                               
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_LOG_WORK_EXCEPTION_H_FILE_GUARD
#define LIB_LOG_WORK_EXCEPTION_H_FILE_GUARD

#include "lib_log_work_message.h"
#include "lib_macro.h"                  // CONCAT               
#include "lib_string.h"                 // lib::format          
#include "lib_time_work_datetime.h"

#include <stdexcept>

namespace lib {
namespace log {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Exception                                                        
///                                                                             
///@brief   An exception class for the DATR suite of tools.                     
///                                                                             
///@par  Why?                                                                   
///         The std::exception class is useful from a programmatic standpoint,  
///         but less useful from a hewmahn readable perspective.                
///         lib::log::work::Message is useful from a hewmahn readable           
///         perspective, but offers less insight to the program.                
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@warning There is effectively no checking to ensure that the number of       
///         parameters passed in to the constructor matches the number of       
///         parameters expected by the MESSAGE_FORMAT string.                   
///                                                                             
///@par Standard Exception Higharchy                                            
///         -   std::exception                                                  
///                 -   std::logic_error : public exception                     
///                         -   std::domain_error : public logic_error          
///                         -   std::invalid_argument : public logic_error      
///                         -   std::length_error : public logic_error          
///                         -   std::out_of_range : public logic_error          
///                 -   std::runtime_error                                      
///                         -   std::range_error : public runtime_error         
///                         -   std::overflow_error : public runtime_error      
///                         -   std::underflow_error : public runtime_error     
///                                                                             
///@par                                                                         
///         Yes, I know.  The above doesn't belong here ... but @e I find it    
///         useful.                                                             
///                                                                             
///@version 2020-09-21  DHF     Made open source                                
///                             Removed THROW.                                  
///                                                                             
///@version 2014-03-26  DHF     It is against pre-processor standards to put    
///                             a line continuation in with the macro           
///                             parameters.                                     
///                                                                             
///@version 2013-10-02  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename STD_EXCEPTION>
class Exception
    : public lib::log::work::Message
    , public STD_EXCEPTION
{
    public:
        Exception(const Exception& that) 
            : lib::log::work::Message(that)
            , STD_EXCEPTION(that)
        {
        }

        Exception(ConstMessagePtr msg)
            : lib::log::work::Message(*msg)
            , STD_EXCEPTION(msg->toString())
        {
        }

        Exception(
            lib::log::ds::class_t       classid
          , ds::level_t                 severity    
          , ds::messageid_t             message_id  = 0
          , const std::string&          message     = ""
          , lib::time::work::DateTime   time_stamp  
                                            = lib::time::work::DateTime::now()
          , const std::string&          app_mnemonic = ""
        ) : lib::log::work::Message(
                classid
              , severity
              , message_id
              , message
              , time_stamp
              , app_mnemonic
            )
          , STD_EXCEPTION(toString())
        {
        }

        Exception(
            lib::log::ds::class_t   classid
          , const std::string&  severity    
          , ds::messageid_t     message_id  = 0
          , const std::string&  message     = ""
          , lib::time::work::DateTime       time_stamp  = lib::time::work::DateTime::now()
          , const std::string&  app_mnemonic = ""
        ) : lib::log::work::Message(
                classid
              , severity
              , message_id
              , message
              , time_stamp
              , app_mnemonic
            )
          , STD_EXCEPTION(toString())
        {
        }

        virtual ~Exception() { }

        Exception& operator=(const Exception& that)
        {
            lib::log::work::Message::operator=(that);
            STD_EXCEPTION::operator=(that);
            return *this;
        }

}; // class Exception //

//------------------------------------------------------------------------------
///@brief   Create CustomExceptions that can be more readily caught (versus the 
///         typedef Exceptions that all map to the same base class...).         
//------------------------------------------------------------------------------
template <
    lib::log::ds::class_t       CLASS_ID
  , lib::log::ds::messageid_t   MSG_ID
  , const char**                MESSAGE_FORMAT
  , typename                    STD_EXCEPT = std::runtime_error
  , lib::log::ds::level_t       LEVEL = lib::log::ds::FATAL
>
class CustomException : public Exception<STD_EXCEPT>
{
    public:
        //----------------------------------------------------------------------
        ///@brief   Create the exception with no parameters to the message.     
        //----------------------------------------------------------------------
        CustomException(
            lib::time::work::DateTime       time_stamp = lib::time::work::DateTime::now()
          , const std::string&  app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                CLASS_ID
              , LEVEL
              , MSG_ID
              , MESSAGE_FORMAT 
              , time_stamp
              , app_mnemonic
            )
          , STD_EXCEPT(Exception<STD_EXCEPT>::toString())
        {
            static_assert(
                CLASS_ID != lib::log::ds::LIB_LOG_WORK_EXCEPTION
              , "Class ID should not equal lib::log::ds::LIB_LOG_WORK_EXCEPTION"
            );
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 1 parameter to the message.       
        //----------------------------------------------------------------------
        CustomException(
            const std::string& param1
          , lib::time::work::DateTime       time_stamp = lib::time::work::DateTime::now()
          , const std::string&  app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                  CLASS_ID
                , LEVEL
                , MSG_ID
                , lib::format(*MESSAGE_FORMAT, param1.c_str())
                , time_stamp
                , app_mnemonic
              )
        {
            static_assert(
                CLASS_ID != lib::log::ds::LIB_LOG_WORK_EXCEPTION
              , "Class ID should not equal lib::log::ds::LIB_LOG_WORK_EXCEPTION"
            );
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 2 parameters to the message.      
        //----------------------------------------------------------------------
        CustomException(
            const std::string& param1
          , const std::string& param2
          , lib::time::work::DateTime       time_stamp = lib::time::work::DateTime::now()
          , const std::string& app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                CLASS_ID
              , LEVEL
              , MSG_ID
              , lib::format(
                    *MESSAGE_FORMAT
                  , param1.c_str()
                  , param2.c_str()
                )
              , time_stamp
              , app_mnemonic
            )
        {
            static_assert(
                CLASS_ID != lib::log::ds::LIB_LOG_WORK_EXCEPTION
              , "Class ID should not equal lib::log::ds::LIB_LOG_WORK_EXCEPTION"
            );
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 3 parameters to the message.      
        //----------------------------------------------------------------------
        CustomException(
            const std::string& param1
          , const std::string& param2
          , const std::string& param3
          , lib::time::work::DateTime   time_stamp = lib::time::work::DateTime::now()
          , const std::string& app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                CLASS_ID
              , LEVEL
              , MSG_ID
              , lib::format(
                    *MESSAGE_FORMAT
                  , param1.c_str()
                  , param2.c_str()
                  , param3.c_str()
                )
              , time_stamp
              , app_mnemonic
            )
        {
            static_assert(
                CLASS_ID != lib::log::ds::LIB_LOG_WORK_EXCEPTION
              , "Class ID should not equal lib::log::ds::LIB_LOG_WORK_EXCEPTION"
            );
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 4 parameters to the message.      
        //----------------------------------------------------------------------
        CustomException(
            const std::string& param1
          , const std::string& param2
          , const std::string& param3
          , const std::string& param4
          , lib::time::work::DateTime   time_stamp = lib::time::work::DateTime::now()
          , const std::string& app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                CLASS_ID
              , LEVEL
              , MSG_ID
              , lib::format(
                    *MESSAGE_FORMAT
                  , param1.c_str()
                  , param2.c_str()
                  , param3.c_str()
                  , param4.c_str()
                )
              , time_stamp
              , app_mnemonic
            )
        {
            static_assert(
                CLASS_ID != lib::log::ds::LIB_LOG_WORK_EXCEPTION
              , "Class ID should not equal lib::log::ds::LIB_LOG_WORK_EXCEPTION"
            );
        }
        //----------------------------------------------------------------------
        ///@brief   Create the exception with no parameters to the message.     
        //----------------------------------------------------------------------
        CustomException(
            lib::log::ds::class_t   class_id
          , lib::time::work::DateTime       time_stamp = lib::time::work::DateTime::now()
          , const std::string&  app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                class_id
              , LEVEL
              , MSG_ID
              , MESSAGE_FORMAT 
              , time_stamp
              , app_mnemonic
            )
        {
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 1 parameter to the message.       
        //----------------------------------------------------------------------
        CustomException(
            lib::log::ds::class_t   class_id
          , const std::string& param1
          , lib::time::work::DateTime       time_stamp = lib::time::work::DateTime::now()
          , const std::string&  app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                  class_id
                , LEVEL
                , MSG_ID
                , lib::format(*MESSAGE_FORMAT, param1.c_str())
                , time_stamp
                , app_mnemonic
              )
        {
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 2 parameters to the message.      
        //----------------------------------------------------------------------
        CustomException(
            lib::log::ds::class_t   class_id
          , const std::string& param1
          , const std::string& param2
          , lib::time::work::DateTime       time_stamp = lib::time::work::DateTime::now()
          , const std::string& app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                class_id
              , LEVEL
              , MSG_ID
              , lib::format(
                    *MESSAGE_FORMAT
                  , param1.c_str()
                  , param2.c_str()
                )
              , time_stamp
              , app_mnemonic
            )
        {
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 3 parameters to the message.      
        //----------------------------------------------------------------------
        CustomException(
            lib::log::ds::class_t class_id
          , const std::string& param1
          , const std::string& param2
          , const std::string& param3
          , lib::time::work::DateTime   time_stamp = lib::time::work::DateTime::now()
          , const std::string& app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                class_id
              , LEVEL
              , MSG_ID
              , lib::format(
                    *MESSAGE_FORMAT
                  , param1.c_str()
                  , param2.c_str()
                  , param3.c_str()
                )
              , time_stamp
              , app_mnemonic
            )
        {
        }

        //----------------------------------------------------------------------
        ///@brief   Create the exception with 4 parameters to the message.      
        //----------------------------------------------------------------------
        CustomException(
            lib::log::ds::class_t class_id
          , const std::string& param1
          , const std::string& param2
          , const std::string& param3
          , const std::string& param4
          , lib::time::work::DateTime   time_stamp = lib::time::work::DateTime::now()
          , const std::string& app_mnemonic = ""
        ) : Exception<STD_EXCEPT>(
                class_id
              , LEVEL
              , MSG_ID
              , lib::format(
                    *MESSAGE_FORMAT
                  , param1.c_str()
                  , param2.c_str()
                  , param3.c_str()
                  , param4.c_str()
                )
              , time_stamp
              , app_mnemonic
            )
        {
        }

        virtual ~CustomException()  { }

}; // class CustomException //

//==============================================================================
//  I'm not sure if the following code deserves a medal or a one-way ticket to  
//  Hell.  The basic concept is to have the macro behave one way for the code   
//  including it and a different way fro lib_log_work_exception.cpp.            
//==============================================================================

#ifndef LIB_LOG_WORK_EXCEPTION_CPP
#define DATR_STANDARD_EXCEPTION(EXCEPTION_CLASS, MSG_ID, MSG_FORMAT, STD_EXCEPTION_TYPE) \
    extern const char* CONCAT(EXCEPTION_CLASS, _STR);                       \
    typedef lib::log::work::CustomException<                                \
        lib::log::ds::LIB_LOG_WORK_EXCEPTION                                \
      , MSG_ID                                                              \
      , &CONCAT(EXCEPTION_CLASS, _STR)                                      \
      , STD_EXCEPTION_TYPE                                                  \
    > EXCEPTION_CLASS
#else

#define DATR_STANDARD_EXCEPTION(EXCEPTION_CLASS, MSG_ID, MSG_FORMAT, STD_EXCEPTION_TYPE) \
    const char* CONCAT(EXCEPTION_CLASS, _STR)(MSG_FORMAT);
#endif

//==============================================================================
//  Some stand exception types.                                                 
//==============================================================================
DATR_STANDARD_EXCEPTION(
    FileNameNotSpecifiedException
  , lib::log::ds::messageid_t(ds::FILE_NAME_NOT_SPECIFIED)
  , "File name not specified."
  , std::runtime_error
);

DATR_STANDARD_EXCEPTION(
    FileOpenReadFailureException
  , lib::log::ds::messageid_t(ds::FILE_OPEN_READ_FAILURE)
  , "Failed to open %s for read."
  , std::runtime_error
);

DATR_STANDARD_EXCEPTION(
    FileOpenWriteFailureException
  , lib::log::ds::messageid_t(ds::FILE_OPEN_WRITE_FAILURE)
  , "Failed to open %s for write."
  , std::runtime_error
);

DATR_STANDARD_EXCEPTION(
    NotMyFileTypeException
  , ds::FILE_WRONG_TYPE
  , "File %s is not of type %s."
  , std::runtime_error
);

DATR_STANDARD_EXCEPTION(
    LogicError
  , ds::LOGIC_ERROR_MSG
  , "logic error: %s."
  , std::logic_error
);

//------------------------------------------------------------------------------
//  Do everyone a favor and DO NOT THROW AN EXCEPTION OF THIS TYPE.  This is a  
//  catch all.                                                                  
//------------------------------------------------------------------------------
DATR_STANDARD_EXCEPTION(
    UnknownException
  , ds::UNKNOWN_MESSAGE
  , "%s exception thrown:  %s"
  , std::logic_error
);

} // namespace work
} // namespace log
} // namespace lib

#endif // #ifndef LIB_LOG_WORK_EXCEPTION_H_FILE_GUARD

