//------------------------------------------------------------------------------
///@file lib_work_file.cpp                                                      
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
///                                                                             
///@brief   Base class for various data file reader/writer.                     
///                                                                             
///@warning If you derived from lib::work::File and you expect to use openPre   
///         and openPost, you cannot call the constructor with a path.  Why?    
///         Because your object's v-tables are still under construction and     
///         will [may?] not be found.  If you open the file via the constructor 
///         then the derived class' openPre and openPost will @e not be called. 
///                                                                             
///@version 2020-09-21  DHF     Made opens source.                              
///                             Removed THROW.                                  
///                             Updated size of strncpy.                        
///                                                                             
///@version 2017-10-16  MNT     Merged 2017-08-22 (SEV) and 2017-10-11 (MNT).   
///                                                                             
///@version 2017-10-11  MNT     Added Data::initialize() and calls it in open.  
///                                                                             
///@version 2017-08-22  SEV     Updated fsize method for VS 2015.               
///                                                                             
///@version 2015-04-30  MNT     Added pre and post methods.                     
///                                                                             
///@version 2015-03-31  DMN     Added remove method.                            
///                                                                             
///@version 2014-06-18  AGB     Filled in some of the Windows-specific code.    
///                                                                             
///@version 2014-03-31  DHF     Added logMessage queue methods.                 
///                                                                             
///@version 2014-02-24  DHF     Fixed bug for MinGW regrading file size.        
///                             Visual Studio may still have the same issue.    
///                                                                             
///@version 2013-10-07  DHF     Added THROW to constructor.                     
///                             Added THROW to open.                            
///                                                                             
///@version 2013-02-06  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_work_file.h"
#include "lib_compiler_info.h"
#include "lib_file_work_info.h"
#include "lib_log_work_messagefactory.h"
#include "lib_string.h"


#include <assert.h>
#include <deque>

#ifdef IS_MINGW
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdlib.h>
    #include <time.h>
    #include <unistd.h>
#endif

#ifdef IS_LINUX
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdlib.h>
    #include <time.h>
#endif

#ifdef IS_VISUAL_STUDIO
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <stdlib.h>
    #include <time.h>
#endif


namespace lib {
namespace work {

//------------------------------------------------------------------------------
///@brief State data for the File class.                                        
//------------------------------------------------------------------------------
struct File::Data
{
    FILE*           m_File;
    std::string     m_Path;

    uint64_t        m_TellG;
    uint64_t        m_Size;

    uint64_t        m_BytesRead;
    uint64_t        m_BytesWritten;

    volatile bool   m_Abort;

    int             m_ErrorNumber;

    void initialize() {
        m_File = NULL;
        m_Path.erase();
        m_TellG = 0;
        m_Size = 0;
        m_BytesRead = 0;
        m_BytesWritten = 0;
        m_Abort = false;
        m_ErrorNumber = 0;
    }

    Data() { initialize(); }
};

const lib::log::ds::messageid_t  lib::work::File::MSG_WRITE_ERROR       (1);
const lib::log::ds::messageid_t  lib::work::File::MSG_READ_ERROR        (2);

//------------------------------------------------------------------------------
///@brief   Return the number of bytes in a given file.                         
//------------------------------------------------------------------------------
static uint64_t fsize(FILE* fp)
{
    #pragma message "WARNING:  Some of this code really needs to change"
    #ifdef IS_MINGW
        struct ::_stati64 s;

        if(::_fstati64(fp->_file, &s) == -1) {
            perror("_fstat64");
        }
        return s.st_size;
    #endif

    #ifdef IS_VISUAL_STUDIO_OBE_QUESTION_MARK
        __stat64 s;
        _fstat64(fileno(fp), &s);
        return s.st_size;
    #endif

    #ifdef IS_VISUAL_STUDIO_PRE_2015
        struct ::_stati64 s;

        if(::_fstati64(fp->_file, &s) == -1) {
            perror("_fstat64");
        }
        return s.st_size;
    #endif

    //--------------------------------------------------------------------------
    //Changed fp->_file to _fileno(fp): VS 2015 no longer allows use of _file.  
    //--------------------------------------------------------------------------
    #ifdef IS_VISUAL_STUDIO
        struct ::__stat64 s;

        if (::_fstat64(_fileno(fp), &s) == -1)
        {
            perror("__fstat64");
        }
        return s.st_size;
    #endif

    #ifdef IS_LINUX
        struct stat s;
        static_assert(sizeof(s.st_size) == 8, "sizeof(s.st_size) == 0");
        if (fstat(fileno(fp), &s) == -1) {
            perror("fstat");
        }
        return s.st_size;
    #endif

}

//------------------------------------------------------------------------------
///@brief   Replace "ftell" with 64-bit version.                                
//------------------------------------------------------------------------------
static uint64_t ftell(FILE* file)
{
    #pragma message "WARNING: This code MUST change once MinGW supports 64-bit files."
    fpos_t pos;
    fgetpos(file, &pos);

    return *((uint64_t*)(&pos));
}

//------------------------------------------------------------------------------
///@brief   Replace "fseek" with 64-bit version.                                
//------------------------------------------------------------------------------
static int fseek(FILE* file, int64_t offset, int whence)
{
    #pragma message "WARNING: This code MUST change once MinGW supports 64-bit files."
    switch(whence) {
        case SEEK_CUR:
            offset += lib::work::ftell(file);
            break;

        case SEEK_END:
            offset = lib::work::fsize(file) - offset;
            break;

        case SEEK_SET:
            break;

        default:
            break;

    }

    return fsetpos(file, (fpos_t*)(&offset));

}

//------------------------------------------------------------------------------
///@brief   Create the File, opening the specified file (for mode).             
///                                                                             
///@warning If @e path is passed in and the file is opened then the derived     
///         classes openPre and openPost will @e not be called.  This is        
///         because the v-tables are not fully initialized at this point and    
///         therefore the polymorphed openPre/openPost will not be found.       
///                                                                             
//------------------------------------------------------------------------------
File::File(
    const std::string& path
  , const std::string& mode
  , bool overwrite_existing
) : m_Data(new Data)
{
    if (path != "") open(path, mode, overwrite_existing);
}

//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
File::~File()
{
    close();

} // File::~File

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
bool File::open(
    const std::string& path
  , const std::string& mode
  , bool overwrite_existing
) 
{
    close();

    if (openPre()) {

        //----------------------------------------------------------------------
        //  If we're trying to open for write, make sure either the file        
        //  doesn't exist or we're supposed to just overwrite it.               
        //----------------------------------------------------------------------
        if (
            !overwrite_existing 
          && mode.find("w") != mode.npos
          && lib::file::work::Info::exists(path)
        ) {
            throw lib::log::work::Exception<std::runtime_error>(
                lib::log::ds::LIB_WORK_FILE
              , lib::log::ds::FATAL
              , lib::log::ds::FILE_OPEN_WRITE_FAILURE
              , "File " + path + " already exists."
            );
        }

        //----------------------------------------------------------------------
        //  Need to reinitialize the m_Data object since we're [potentially]    
        //  opening another file after closing a previous one.                  
        //----------------------------------------------------------------------
        m_Data->initialize();

        m_Data->m_Path = path;
        m_Data->m_File = fopen(path.c_str(), mode.c_str());

        if (m_Data->m_File != NULL) {
            //------------------------------------------------------------------
            //  We're good.  No exceptions to be thrown.                        
            //------------------------------------------------------------------

        } else if (mode.find("w") != mode.npos) {
            throw lib::log::work::FileOpenWriteFailureException(
                lib::log::ds::LIB_WORK_FILE
              , path
            );
        } else if (mode.find("r") != mode.npos) {
            throw lib::log::work::FileOpenReadFailureException(
                lib::log::ds::LIB_WORK_FILE
              , path
            );
        }

        postSetInput();

        //----------------------------------------------------------------------
        //  Set the m_Data->m_Size of the file.                                 
        //----------------------------------------------------------------------
        if (m_Data->m_File != NULL) 
        {
            m_Data->m_Size = lib::work::fsize(m_Data->m_File);
        }

        openPost();
    }
    return m_Data->m_File != NULL;
}

bool File::openPre() { return true; }
void File::openPost() { }

//------------------------------------------------------------------------------
///@brief   Open the file for append.                                           
//------------------------------------------------------------------------------
bool File::append(const std::string& path)
{
    bool result(appendPre());

    if (result) result = open(path, "ab");

    if (result) {
        m_Data->m_TellG = size();
        appendPost();
    }

    return result;

}

bool File::appendPre() { return true; }
void File::appendPost() { }

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::close()
{
    if (!closePre()) return;

    if (m_Data->m_File) {
        fclose(m_Data->m_File);
        m_Data->m_File = NULL;

        closePost();
    }
}

bool File::closePre() { return true; }
void File::closePost() { }

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::flush()
{
    if (!flushPre()) return;
    assert(m_Data->m_File);
    fflush(m_Data->m_File);

    flushPost();
}

bool File::flushPre() { return true; }
void File::flushPost() { }

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
bool File::eof() const
{
    return m_Data->m_File == NULL || feof(m_Data->m_File);
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
bool File::is_open() const
{
    return m_Data->m_File != NULL;
} 

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
std::string File::path() const
{
    return m_Data->m_Path;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
uint64_t File::size() const
{
    return m_Data->m_Size;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
uint64_t File::tellg() const
{
    return m_Data->m_TellG;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::seek(uint64_t offset, int direction)
{
    switch(direction)
    {
        case SEEK_CUR:
            m_Data->m_TellG += offset;
            break;
        case SEEK_END:
            m_Data->m_TellG = size() - offset;
            if (m_Data->m_TellG < 0) m_Data->m_TellG = 0;
            break;
        case SEEK_SET:
        default:
            m_Data->m_TellG = offset;
            break;
    }
    lib::work::fseek(m_Data->m_File, m_Data->m_TellG, SEEK_SET);

} // void File::seek(uint64_t offset, int direction) //

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::rewind()
{
    if (!rewindPre()) return;

    if (m_Data->m_File != NULL) {
        seek(0);
        rewindPost();
    }
}

bool File::rewindPre() { return true; }
void File::rewindPost() { }

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::clear()
{
    //--------------------------------------------------------------------------
    //  This may be relict of a bygone era ... it was supposed to clear eof and 
    //  any error flags.                                                        
    //--------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
uint64_t  File::bytesRead() const
{
    return m_Data->m_BytesRead;

} 

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
uint64_t  File::bytesWritten() const
{
    return m_Data->m_BytesWritten;
} 

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::set_stdin()
{
    close();
    m_Data->m_File = stdin;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::set_stderr()
{
    close();
    m_Data->m_File = stderr;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::set_stdout()
{
    close();
    m_Data->m_File = stdout;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
uint64_t File::read (void* buffer, uint64_t bytes)
{
    size_t result = fread(buffer, 1, size_t(bytes), m_Data->m_File);
    checkForError("read", MSG_READ_ERROR, result, bytes);

    m_Data->m_TellG += result;
    m_Data->m_BytesRead += result;

    readPost(buffer, bytes);

    return result;
}

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
uint64_t File::write(const void* buffer, uint64_t bytes)
{
    int64_t bytesWritten = fwrite(buffer, 1, bytes, m_Data->m_File);
    checkForError("write", MSG_WRITE_ERROR, bytesWritten, bytes);

    m_Data->m_TellG += bytesWritten;
    m_Data->m_BytesWritten += bytesWritten;

    writePost(buffer, bytes);

    return bytesWritten;
}

//------------------------------------------------------------------------------
///@brief   Remove/delete the file.                                             
//------------------------------------------------------------------------------
bool File::remove()
{
    bool result = false;
    if (removePre()) {
        result = 
            lib::file::work::Info::exists(path()) && std::remove(path().c_str()) == 0;
        removePost();
    }
    return result;
}

bool File::removePre() { return true; }
void File::removePost() { }

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void File::checkForError(
    const char* func
  , lib::log::ds::messageid_t id
  , size_t actualBytes
  , size_t expectedBytes
) 
{
    if (actualBytes != expectedBytes && ferror(m_Data->m_File)) {
        //----------------------------------------------------------------------
        //  Save off the error number and get the system messasge.              
        //----------------------------------------------------------------------
        m_Data->m_ErrorNumber = errno;
        char error_buffer[1024];

        //----------------------------------------------------------------------
        //  Note:   We'd prefer to use strerror_r, but it isn't supported by    
        //          Microsoft and doesn't seem to be working under Linux.       
        //----------------------------------------------------------------------
        strncpy(
            error_buffer
          , strerror(m_Data->m_ErrorNumber)
          , sizeof(error_buffer)-1
        );

        std::string message = lib::format(
            "%s error %d on file %s: %s"
          , func
          , m_Data->m_ErrorNumber
          , m_Data->m_Path.c_str()
          , error_buffer
        );

        throw lib::log::work::Exception<std::runtime_error>(
            lib::log::ds::LIB_WORK_FILE
          , lib::log::ds::FATAL
          , id
          , message
        );
    }
}

void File::postSetInput()
{
}

//------------------------------------------------------------------------------
///@brief   Inform the underlying file reader that it should stop.              
///@warning abort is not something that is recoverable.  Once "aborted" the     
///         object cannot be resumed/restarted.                                 
//------------------------------------------------------------------------------
void File::abort() { m_Data->m_Abort = true; }

//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
bool File::isAbort() const { return m_Data->m_Abort; }

} // namespace work //
} // namespace lib //

