//------------------------------------------------------------------------------
///@file lib_file_work_info.cpp                                                 
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@brief Contains the implementation code for the lib::file::work::Info class  
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2019-10-09  DHF     Added extension and root methods.               
///                                                                             
///@version 2013-10-01  DHF     Added static member exists.                     
///                                                                             
///@version 2007-04-16  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/stat.h>

#ifdef IS_VISUAL_STUDIO
#include <io.h> // _chmod
#include <sys/utime.h>
#else
#include <unistd.h>
#include <utime.h>
#endif

#include <string.h>                     //  memcpy                              

#include "lib_file_work_info.h"

namespace lib {
namespace file {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Info::Info(
    const std::string& filePath
) : m_Path(filePath)
{
    if (filePath != "") {
        refresh();
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Info::Info(
    const Info& that
) : stat(that)
  , m_Path(that.m_Path)
{
    st_dev = that.st_dev;               // ID of device containing file         
    st_ino = that.st_ino;               // inode number                         
    st_mode = that.st_mode;             // protection                           
    st_uid = that.st_uid;               // user ID of owner                     
    st_gid = that.st_gid;               // group ID of owner                    
    st_rdev = that.st_rdev;             // device ID (if special file)          
    st_size = that.st_size;             // total size, in bytes                 
    st_atime = that.st_atime;           // time of last access                  
    st_mtime = that.st_mtime;           // time of last modification            
    st_ctime = that.st_ctime;           // time of last status change           

    #ifndef _WIN32
    st_blksize = that.st_blksize;       // blocksize for filesystem I/O         
    st_blocks = that.st_blocks;         // number of blocks allocated           
    st_nlink = that.st_nlink;           // number of hard links                 
    #endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Info::~Info() 
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Info& Info::operator=(const Info& that) 
{
    stat::operator=(that);
    return *this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Info& Info::operator=(const std::string& newpath)
{
    m_Path = newpath;
    if (newpath != "") {
        refresh();
    }
    return *this;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Info::refresh()
{
#ifdef IS_VISUAL_STUDIO
    struct _stat* s(dynamic_cast<struct _stat*>(this));
    int status = ::_stat(m_Path.c_str(), s);
    if (status != 0) {memset(s, 0, sizeof(struct _stat));}
#else
    int status = ::stat(m_Path.c_str(), this);
    if (status != 0) {
        memset(static_cast<stat*>(this), 0, sizeof(stat));
    }
#endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool Info::isDirectory() const
{
    #ifdef _WIN32
    #define __S_IFDIR S_IFDIR
    #endif
    return (st_mode & __S_IFDIR) != 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const std::string& Info::path() const
{
    return m_Path;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string Info::name() const
{
    for (size_t x = m_Path.size(); x > 0; ) {
        --x;
        if (m_Path[x] == '/' || m_Path[x] == '\\') {
            return m_Path.substr(x+1, m_Path.size());
        }
    }

    return m_Path;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string Info::extension() const
{
    std::string n(name());
    std::string result;

    size_t last_dot = n.rfind(".");

    if (last_dot != std::string::npos)
    {
        result = n.substr(last_dot+1);
    }

    return result;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string Info::root() const
{
    std::string n(name());
    std::string result;

    size_t last_dot = n.rfind(".");

    if (last_dot != std::string::npos)
    {
        result = n.substr(0, last_dot);
    }

    return result;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool operator<(const Info& left, const Info& right)
{
    if (left.isDirectory() && !right.isDirectory()) {
        return true;
    } else if (right.isDirectory() && !left.isDirectory()) {
        return false;
    } else {
        return left.path() < right.path();
    }
}

//------------------------------------------------------------------------------
//  Read accessor functions.                                                    
//------------------------------------------------------------------------------
dev_t       Info::device()                  const { return st_dev; }
ino_t       Info::inode()                   const { return st_ino; }
mode_t      Info::mode()                    const { return st_mode;}
dev_t       Info::rdevice()                 const { return st_rdev;}
off_t       Info::size()                    const { return st_size;}
time_t      Info::accessTime()              const { return st_atime;}
time_t      Info::modifyTime()              const { return st_mtime;}
time_t      Info::lastStatusChangeTime()    const { return st_ctime;}

#ifndef _WIN32
nlink_t     Info::nlink()                   const { return st_nlink;}
uid_t       Info::uid()                     const { return st_uid;}
gid_t       Info::gid()                     const { return st_gid;}
blksize_t   Info::blockSize()               const { return st_blksize;}
blkcnt_t    Info::blocks()                  const { return st_blocks;}
#endif

//------------------------------------------------------------------------------
//  Write accessor functions                                                    
//------------------------------------------------------------------------------
int Info::mode(const mode_t& m)
{
#ifdef IS_VISUAL_STUDIO
    int result = _chmod(m_Path.c_str(), m);
#else
    int result =  chmod(m_Path.c_str(), m);
#endif

    refresh();
    return result;
}

#ifndef _WIN32
int Info::uid(const uid_t& u)
{
    int result =  chown(m_Path.c_str(), u, -1);
    refresh();
    return result;
}

int Info::gid(const gid_t& g)
{
    int result =  chown(m_Path.c_str(), -1, g);
    refresh();
    return result;
}
#endif

int Info::accessTime(const time_t& a)
{
    utimbuf b;
    b.actime = a;
    b.modtime = st_mtime;
    int result =  utime(m_Path.c_str(), &b);
    refresh();
    return result;
}

int Info::modifyTime(const time_t& m)
{
    utimbuf b;
    b.actime = st_atime;
    b.modtime = m;
    int result = utime(m_Path.c_str(), &b);
    refresh();
    return result;
}

bool Info::exists(const std::string& path)
{
    //--------------------------------------------------------------------------
    //  I'm allowed one Microsoft slam per comment.  Consider the gratutious    
    //  renaming of standard functions slammed.                                 
    //
    //  AGB - I'm now officially allowing you two per comment.  Microsoft has
    //  earned it.
    //--------------------------------------------------------------------------
    #ifdef IS_VISUAL_STUDIO
        //struct _stat s;
        //int status = ::_stat(path.c_str(), &s);
        struct __stat64 s;
        int status = ::__stat64(path.c_str(), &s);
    #else
        struct stat s;
        int status = ::stat(path.c_str(), &s);
    #endif

    return status == 0;

}

} // namespace work 
} // namespace file
} // namespace lib

