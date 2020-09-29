//------------------------------------------------------------------------------
///@file lib_file_info.h                                                        
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_FILE_WORK_INFO_H
#define LIB_FILE_WORK_INFO_H

#include <string>
#include <sys/stat.h>

namespace lib {

//------------------------------------------------------------------------------
/// Generic namespace for classes that relate to files and the file system.     
//------------------------------------------------------------------------------
namespace file {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@brief Wraps the system stat structure providing information on files.       
///                                                                             
///@par Class: Info                                                             
///                                                                             
///@par Purpose:                                                                
///         The lib::file::work::Info class is a wrapper around the system stat 
///         structure.                                                          
///                                                                             
///@par Thread Safety:  class                                                   
///         The lib::file::work::Info class is thread safe.  However, objects are
///         explicitly not thread safe.                                         
///                                                                             
///@par In Memory vs On Disk                                                    
///         The lib::file::work::Info class takes a snap shot of the file status
///         at object creation time and when refresh() is called.  If the file  
///         is modified after either of these events, the value of the object   
///         does not change.                                                    
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2019-10-09  DHF     Added extension method.                         
///                             Added root method.                              
///                                                                             
///@version 2013-10-01  DHF     Added static member exists.                     
///                                                                             
///@version 2007-04-16  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Info : public stat {
    public:
        //----------------------------------------------------------------------
        ///@brief Construct the Info object for the given file                  
        //----------------------------------------------------------------------
        Info(const std::string& path="");
        Info(const Info& that);
        virtual ~Info();

        Info& operator=(const Info& that);
        Info& operator=(const std::string& newpath);
        void refresh();

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        bool isDirectory() const;

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        const std::string& path() const;
              std::string  name() const;
              std::string  extension() const;
              std::string  root() const;

        //----------------------------------------------------------------------
        ///@brief ID of device containing file                                  
        //----------------------------------------------------------------------
        dev_t     device()  const;    

        //----------------------------------------------------------------------
        ///@brief inode number                                                  
        //----------------------------------------------------------------------
        ino_t     inode()   const;     

        //----------------------------------------------------------------------
        ///@brief Return the current protection of the file                     
        //----------------------------------------------------------------------
        mode_t    mode()    const;    

        //----------------------------------------------------------------------
        ///@brief Chagne the current protection of the file                     
        ///@return -1 = error (see errno); 0 = success                          
        //----------------------------------------------------------------------
        int mode(const mode_t&);

        #ifndef _WIN32
        //----------------------------------------------------------------------
        ///@brief number of hard links                                          
        //----------------------------------------------------------------------
        nlink_t   nlink()   const;   

        //----------------------------------------------------------------------
        ///@brief user ID of owner                                              
        //----------------------------------------------------------------------
        uid_t     uid()     const;     

        //----------------------------------------------------------------------
        ///@brief Change the user ID of owner                                   
        ///@return -1 = error (see errno); 0 = success                          
        //----------------------------------------------------------------------
        int uid(const uid_t&);

        //----------------------------------------------------------------------
        ///@brief group ID of owner                                             
        //----------------------------------------------------------------------
        gid_t     gid()     const;     

        //----------------------------------------------------------------------
        ///@brief Change the group ID of the owner                              
        ///@return -1 = error (see errno); 0 = success                          
        //----------------------------------------------------------------------
        int gid(const gid_t&);

        //----------------------------------------------------------------------
        ///@brief blocksize for filesystem I/O                                  
        //----------------------------------------------------------------------
        blksize_t blockSize() const; 

        //----------------------------------------------------------------------
        ///@brief number of blocks allocated                                    
        //----------------------------------------------------------------------
        blkcnt_t  blocks() const;  
        #endif

        //----------------------------------------------------------------------
        ///@brief device ID (if special file)                                   
        //----------------------------------------------------------------------
        dev_t     rdevice() const;    

        //----------------------------------------------------------------------
        ///@brief total size, in bytes                                          
        //----------------------------------------------------------------------
        off_t     size()    const;    

        //----------------------------------------------------------------------
        ///@brief time of last access                                           
        //----------------------------------------------------------------------
        time_t    accessTime() const;   

        //----------------------------------------------------------------------
        ///@brief Change the last access time of the file                       
        ///@return -1 = error (see errno); 0 = success                          
        //----------------------------------------------------------------------
        int      accessTime(const time_t&);

        //----------------------------------------------------------------------
        ///@brief time of last modification                                     
        //----------------------------------------------------------------------
        time_t    modifyTime() const;           

        //----------------------------------------------------------------------
        ///@brief Change the last modified time of the file                     
        ///@return -1 = error (see errno); 0 = success                          
        //----------------------------------------------------------------------
        int      modifyTime(const time_t&);

        //----------------------------------------------------------------------
        ///@brief time of last status change                                    
        //----------------------------------------------------------------------
        time_t    lastStatusChangeTime() const;  

        static bool exists(const std::string& path);

    protected:

    private:
        std::string m_Path;

}; // class Info //

//------------------------------------------------------------------------------
///@brief Less than for Info objecst; primary: dir < files; secondary: ASCII    
//------------------------------------------------------------------------------
bool operator<(const Info& left, const Info& right);

}  // namespace work //
}  // namespace file //
}  // namespace lib //

#endif // #ifndef LIB_FILE_WORK_INFO_H
