//------------------------------------------------------------------------------
///@file lib_work_filetest.h                                                    
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_WORK_FILETEST_H
#define LIB_WORK_FILETEST_H

#include "dev_test_work_test.h"
#include <string>

namespace lib {
namespace work {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: FileTest                                                         
///                                                                             
///@par Purpose:                                                                
///         The FileTest class provides the regression test for the             
///         lib::work::File class.                                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2015-01-13  MTU     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class FileTest : public dev::test::work::Test {
    public:
        FileTest();
        FileTest(const FileTest& that);
        virtual ~FileTest();
        FileTest& operator=(const FileTest& that);

    protected:
        void runTest();

        void File();
        void append();
        void open();
        void close();
        void flush();
        void eof();
        void is_open();
        void path();
        void size();
        void tellg();
        void seek();
        void rewind();
        void clear();
        void bytesRead();
        void bytesWritten();
        void set_stdin();
        void set_stderr();
        void set_stdout();
        void read();
        void write();
        void rewindPre();
        void rewindPost();
        void postSetInput();
        void abort();
        void isAbort();
        void remove();

    private:
        std::string m_NormalPath;
        std::string m_SmallPath;

}; //  class FileTest : public dev::Test //

} // namespace test 
} // namespace work
} // namespace lib

#endif // #ifndef LIB_WORK_FILETEST_H //

