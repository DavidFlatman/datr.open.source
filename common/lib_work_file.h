//------------------------------------------------------------------------------
///@file lib_work_file.h                                                        
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_WORK_FILE_H_FILE_GUARD
#define LIB_WORK_FILE_H_FILE_GUARD

#include "lib_log_work_message.h"
#include "lib_log_work_exception.h"

#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>                      // SEEK_SET 
#include <string>
#include <sstream>
#include <vector>

namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: File                                                             
///                                                                             
///@brief   Base class for various data file reader/writer.                     
///                                                                             
///@par Thread Safety:  special                                                 
///         Treat it as "class" thread safe for best results.                   
///                                                                             
///@par Expected Usgae:                                                         
///         It is expected that the lib::work::File class will be the base      
///         class of a data specific file reader.  The derived class will       
///         [probably] implement a get method that parses out the data file     
///         and returns a lib::ds::shared_ptr to the data.                      
///@sa      lib::irig106::ch10::work::FileReader                                
///                                                                             
///@warning If you derived from lib::work::File and you expect to use openPre   
///         and openPost, you cannot call the constructor with a path.  Why?    
///         because your object's v-tables are still under construction and     
///         will [may?] not be found.  If you open the file via the constructor 
///         then the derived class' openPre and openPost will @e not be called. 
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                             Removed THROW.                                  
///                             Used std::unique_ptr                            
///                                                                             
///@version 2015-11-27  DHF     Moved the operator<< out of the class.  Since   
///                             they do not require any private access to       
///                             lib::work::File, they do not need to be declared
///                             friends.                                        
///                                                                             
///@version 2015-11-25  DMN     Added FilePtr, operator<<                       
///                                                                             
///@version 2015-06-10  DMN     Updated read(vector, max_count) method          
///                                                                             
///@version 2015-05-13  MNT     Added appendPre, appendPost, openPre, openPost  
///                             and so forth.  Code may have originated with    
///                             Magnus.                                         
///                                                                             
///@version 2015-03-31  DMN     Added writing a vector.                         
///                                                                             
///@version 2015-03-03  DHF     Added reading of a vector (due to DMN's request)
///                                                                             
///@version 2015-01-27  DHF     Added overwrite_existing boolean to constructor 
///                             and open method.                                
///                                                                             
///@version 2015-01-14  DHF     Added FileOpenWriteFiluareException to File and 
///                             open.                                           
///                                                                             
///@versoin 2015-01-13  DHF     Added errno, MSG_WRITE_ERROR, MSG_READ_ERROR,   
///                             and THROW on read and write.                    
///                                                                             
///@version 2015-01-06  DHF     Made read and write method virtual.             
///                                                                             
///@version 2014-03-31  DHF     Added virtual log message related methods.      
///                                                                             
///@version 2013-11-04  DHF     Added abort/isAbort                             
///                                                                             
///@version 2013-10-07  DHF     Added THROW to constructor.                     
///                             Added THROW to open.                            
///                                                                             
///@version 2013-02-06  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class File
{
    public:
        File(
            const std::string& path = ""
          , const std::string& mode = "rb"
          , bool overwrite_existing = false
        );
        virtual ~File();

        //----------------------------------------------------------------------
        //  Open modes                                                          
        //----------------------------------------------------------------------
        virtual bool append(const std::string& path);
        virtual bool appendPre();
        virtual void appendPost();

        virtual bool open(
            const std::string& path
          , const std::string& mode = "rb"
          , bool overwrite_existing = false
        );

        virtual bool openPre();
        virtual void openPost();
        
        virtual void close();
        virtual bool closePre();
        virtual void closePost();

        virtual void flush();
        virtual bool flushPre();
        virtual void flushPost();

        virtual bool eof() const;
        virtual bool is_open() const;
        virtual std::string path() const;

        virtual uint64_t size() const;
        virtual uint64_t tellg() const;
        virtual void seek(uint64_t offset, int direction = SEEK_SET);

        virtual void rewind();
        virtual bool rewindPre();
        virtual void rewindPost();

        virtual void clear();

        virtual uint64_t    bytesRead() const;
        virtual uint64_t    bytesWritten() const;

        void set_stdin();
        void set_stderr();
        void set_stdout();

        virtual uint64_t read (void* buffer, uint64_t bytes);

        template<typename TYPE>
        uint64_t read(std::vector<TYPE>& buffer, uint64_t max_count)
        {
            buffer.reserve(max_count);
            buffer.resize(max_count);
            uint64_t bytesRead = read(&buffer[0], buffer.size() * sizeof(TYPE));

            //------------------------------------------------------------------
            //  In case we read less than we asked for, resize to what we read. 
            //------------------------------------------------------------------
            buffer.resize(bytesRead / sizeof(TYPE));
            return bytesRead;
        }
        
        virtual uint64_t write(const void* buffer, uint64_t bytes);

        //----------------------------------------------------------------------
        ///@brief   Write a vector of data.                                     
        ///@limitation  TYPE needs to be plain old data (POD).                  
        //----------------------------------------------------------------------
        template <typename TYPE>
        uint64_t write(const std::vector<TYPE>& buffer)
        {
            if (buffer.size() == 0) return 0;
            return write(&buffer[0], buffer.size() * sizeof(TYPE));
        }

        virtual void postSetInput();

        virtual void abort();
        virtual bool isAbort() const;

        virtual bool remove();
        virtual bool removePre(); 
        virtual void removePost();

//        virtual int errno() const;

        static const lib::log::ds::messageid_t  MSG_WRITE_ERROR;
        static const lib::log::ds::messageid_t  MSG_READ_ERROR;


    protected:
        void checkForError(
            const char* func
          , lib::log::ds::messageid_t id
          , size_t actualBytes
          , size_t expectedBytes
        );

        virtual void writePost(const void* pointer, uint64_t bytes) { }
        virtual void readPost (const void* pointer, uint64_t bytes) { }

    private:
        //----------------------------------------------------------------------
        //  These aren't the routines you're looking for.                       
        //----------------------------------------------------------------------
        File(const File& that);
        File& operator=(const File& that);

        struct Data;
        std::unique_ptr<Data> m_Data;

}; // class File //

using FilePtr = lib::ds::shared_ptr<File>;

} // namespace work
} // namespace lib

//------------------------------------------------------------------------------
///@brief   Allow users to:  file << my_string                                  
//------------------------------------------------------------------------------
inline lib::work::File& operator<<(
    lib::work::File& out
  , const std::string& data
) 
{
    out.write(data.c_str(), data.size());
    return out;
}

//------------------------------------------------------------------------------
///@brief   Allow [nearly] anything to be written to the file.                  
//------------------------------------------------------------------------------
template <typename DATA_TYPE>
lib::work::File& operator<<(lib::work::File& out, const DATA_TYPE& data)
{
    std::ostringstream stream;
    stream << data;
    return out << stream.str();
}


#endif // #ifndef LIB_WORK_FILE_H_FILE_GUARD

