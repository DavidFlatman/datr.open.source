//------------------------------------------------------------------------------
///                                                                             
///@file lib_work_filetest.cpp                                                  
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                             Removed small-drive-test                        
///                                                                             
///@version 2017-10-11  MNT     Added test of tellg reset in open().            
///                                                                             
///@version 2016-02-17  DHF     Added OS checks for small drive.                
///                                                                             
///@version 2015-01-27  DHF     Added testing of open for write with overwrite  
///                             existing.                                       
///                                                                             
///@version 2015-01-13  DHF     Added actual testing (why this didn't exist     
///                             from days long gone is beyond me.)              
///                                                                             
///@version 2015-01-13  MTU     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
#include "lib_work_filetest.h"

#include "dev_test_work.h"
#include "lib_config_work_filepaths.h"
#include "lib_compiler_info.h"
#include "lib_file_work_info.h"
#include "lib_log_work_exception.h"
#include "lib_work_file.h"

#include <stdio.h>                  // remove   

#include "debug.h"

namespace lib {
namespace work {
namespace test {

//------------------------------------------------------------------------------
//  Register the test class for the dev_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::work::test::FileTest);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
FileTest::FileTest() : dev::test::work::Test("lib::work::File")
{
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    FileTest object to copy.                                    
//------------------------------------------------------------------------------
FileTest::FileTest(const FileTest& that) 
    : Test(that)
    , m_NormalPath(that.m_NormalPath)
    , m_SmallPath (that.m_SmallPath)
{

}

//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
FileTest::~FileTest()

{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
FileTest& FileTest::operator=(const FileTest& that)
{
    Test::operator=(that);
    m_NormalPath = that.m_NormalPath;
    m_SmallPath  = that.m_SmallPath ;
    return *this;
} // FileTest::operator=(const FileTest& that) //

//------------------------------------------------------------------------------
/// @brief Execute the various test for File.                                   
//------------------------------------------------------------------------------
void FileTest::runTest()
{
    //--------------------------------------------------------------------------
    //  We can't really put these in the constructor due to the order that      
    //  things are initialized.  Specifically, the TEST_REGISTER creates an     
    //  instance of FileTest before int main() is executed.  Unfortunately, it  
    //  also looks like it runs before the lib::config methods are ready to be  
    //  executed (which also require code to be run before int main).           
    //--------------------------------------------------------------------------
    m_NormalPath = lib::config::work::unitTestTemporaryFilePath("normal-file");

    File();
    append();
    open();
    close();
    flush();
    eof();
    is_open();
    path();
    size();
    tellg();
    seek();
    rewind();
    clear();
    bytesRead();
    bytesWritten();
    set_stdin();
    set_stderr();
    set_stdout();
    read();
    write();
    rewindPre();
    rewindPost();
    postSetInput();
    abort();
    isAbort();
    remove();

} // void FileTest::runTest() //

//------------------------------------------------------------------------------
/// @brief Tests for the File::File                                             
///                                                                             
//------------------------------------------------------------------------------
void FileTest::File()
{
    //--------------------------------------------------------------------------
    //  Create a file; write to it and read from it.                            
    //--------------------------------------------------------------------------
    {
        try { 
            ::remove(m_NormalPath.c_str());

            lib::work::File file(m_NormalPath, "wb");
            for (uint32_t i=0; i < 10; ++i) {
                TEST(file.write(&i, sizeof(i)) == sizeof(i));
            }
            file.close();
        } catch(const std::exception& e) {
            FAIL("exception thrown during write: " + std::string(e.what()));
        } catch(...) {
            FAIL("unknown exception thrown");
        }

        //----------------------------------------------------------------------
        // Try reading what we just wrote                                       
        //----------------------------------------------------------------------
        try { 
            lib::work::File input(m_NormalPath, "rb");
            for (uint32_t i=0; i < 10; ++i) {
                uint32_t j;
                TEST(input.read(&j, sizeof(j)) == sizeof(j));
                TEST(j == i);
            }
            input.close();
        } catch(const std::exception& e) {
            FAIL("exception thrown during read test: " + std::string(e.what()));
        } catch(...) {
            FAIL("unknown exception thrown");
        }

    }

    //--------------------------------------------------------------------------
    //  Fail to open a file.                                                    
    //--------------------------------------------------------------------------
    TEST_CONSTRUCTOR_EXPECT_EXCEPTION(
        lib::work::File("/you-cantwrite-here")
      , lib::log::work::Exception<std::runtime_error>
    );

    //--------------------------------------------------------------------------
    //  We want to test that for a failed to open for read.                     
    //--------------------------------------------------------------------------
    try {
        lib::work::File input("/not-legal.txt", "rb");
        FAIL("able to open /not-legal.txt");
    } catch(const lib::log::work::Exception<std::runtime_error>& e) {
        TEST(e.messageID() == lib::log::ds::FILE_OPEN_READ_FAILURE);
    } catch(...) {
        FAIL("wrong exception thrown");
    }

    //--------------------------------------------------------------------------
    //  We want to test that for a failed to open for write.                    
    //--------------------------------------------------------------------------
    try {
        lib::work::File input("/not-allowed.txt", "wb");
        FAIL("able to open /not-allowed.txt");
    } catch(const lib::log::work::Exception<std::runtime_error>& e) {
        TEST(e.messageID() == lib::log::ds::FILE_OPEN_WRITE_FAILURE);
    } catch(...) {
        FAIL("wrong exception thrown");
    }

} // void FileTest::File()

//------------------------------------------------------------------------------
/// @brief Tests for the File::append                                           
//------------------------------------------------------------------------------
void FileTest::append()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the append.                                     
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the append method.                              
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::open                                             
//------------------------------------------------------------------------------
void FileTest::open()
{
    std::string path(
        lib::config::work::unitTestTemporaryFilePath("lib_work_file.deleteme")
    );

    //--------------------------------------------------------------------------
    //  Try opening a file, clean up after opened.                              
    //--------------------------------------------------------------------------
    try {
        ::remove(path.c_str());
        lib::work::File file(path, "wb");
        file.write("Hello World", 12);
        file.close();
        TEST( lib::file::work::Info::exists(path));
        file.remove();
        TEST(!lib::file::work::Info::exists(path));

    } catch(...) {
        FAIL("unexpeted exception thrown");
    }

    //--------------------------------------------------------------------------
    //  Try opening a file for write that exists (expect exception).            
    //--------------------------------------------------------------------------
    try {
        ::remove(path.c_str());
        lib::work::File file(path, "wb");
        file.close();

        TEST_CONSTRUCTOR_EXPECT_EXCEPTION(
            lib::work::File foo(path,"wb")
          , lib::log::work::Exception<std::runtime_error>
        );
        ::remove(path.c_str());
    } catch(...) {
        FAIL("unexpeted exception thrown");
        ::remove(path.c_str());
    }

    //--------------------------------------------------------------------------
    //  Try opening a file for write that exists (expect exception).            
    //--------------------------------------------------------------------------
    try {
        ::remove(path.c_str());
        lib::work::File file(path, "wb");
        file.close();

        EXPECT_AN_EXCEPTION(
            file.open(path,"wb")
          , lib::log::work::Exception<std::runtime_error>
        );
        ::remove(path.c_str());
    } catch(...) {
        FAIL("unexpeted exception thrown");
        ::remove(path.c_str());
    }

    //--------------------------------------------------------------------------
    //  Try opening a file for write that exists but flag overwrite_existing.   
    //--------------------------------------------------------------------------
    try {
        ::remove(path.c_str());
        lib::work::File file(path, "wb");
        file.close();

        file.open(path,"wb", true);
        PASS("Opened existing file with overwrite_existing set to true.");
        file.close();
        ::remove(path.c_str());
    } catch(...) {
        FAIL("unexpeted exception thrown");
        ::remove(path.c_str());
    }

    //--------------------------------------------------------------------------
    //  Try opening a file, write to it, and close it. Open another file using  
    //  the same File instance. Ensure tellg is zero. Clean up.                 
    //--------------------------------------------------------------------------
    try {
        std::string path2(path + '2');
        ::remove(path.c_str());
        ::remove(path2.c_str());

        lib::work::File file(path, "wb");
        file.write("Hello World", 12);
        file.close();
        file.open(path2, "wb");

        TEST(file.tellg() == 0);

        file.close();
        ::remove(path.c_str());
        ::remove(path2.c_str());
    } catch(...) {
        FAIL("unexpeted exception thrown");
    }
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::close                                            
//------------------------------------------------------------------------------
void FileTest::close()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the close.                                      
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the close method.                               
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::flush                                            
//------------------------------------------------------------------------------
void FileTest::flush()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the flush.                                      
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the flush method.                               
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::eof                                              
//------------------------------------------------------------------------------
void FileTest::eof()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the eof.                                        
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the eof method.                                 
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::is_open                                          
//------------------------------------------------------------------------------
void FileTest::is_open()
{
    ::remove(m_NormalPath.c_str());
    lib::work::File output(m_NormalPath, "wb");
    TEST((output.is_open()));
    TEST((output.close(), !output.is_open()));

    lib::work::File input(m_NormalPath, "rb");
    TEST((input.is_open()));
    TEST((input.close(), !input.is_open()));

    TEST( lib::file::work::Info::exists(m_NormalPath));
    input.remove();
    TEST(!lib::file::work::Info::exists(m_NormalPath));
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::path                                             
//------------------------------------------------------------------------------
void FileTest::path()
{

}

//------------------------------------------------------------------------------
/// @brief Tests for the File::size                                             
//------------------------------------------------------------------------------
void FileTest::size()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the size.                                       
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the size method.                                
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::tellg                                            
//------------------------------------------------------------------------------
void FileTest::tellg()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the tellg.                                      
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the tellg method.                               
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::seek                                             
//------------------------------------------------------------------------------
void FileTest::seek()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the seek.                                       
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the seek method.                                
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::rewind                                           
//------------------------------------------------------------------------------
void FileTest::rewind()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the rewind.                                     
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the rewind method.                              
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::clear                                            
//------------------------------------------------------------------------------
void FileTest::clear()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the clear.                                      
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the clear method.                               
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::bytesRead                                        
//------------------------------------------------------------------------------
void FileTest::bytesRead()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the bytesRead.                                  
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the bytesRead method.                           
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::bytesWritten                                     
//------------------------------------------------------------------------------
void FileTest::bytesWritten()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the bytesWritten.                               
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the bytesWritten method.                        
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::set_stdin                                        
//------------------------------------------------------------------------------
void FileTest::set_stdin()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the set_stdin.                                  
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the set_stdin method.                           
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::set_stderr                                       
//------------------------------------------------------------------------------
void FileTest::set_stderr()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the set_stderr.                                 
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the set_stderr method.                          
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::set_stdout                                       
//------------------------------------------------------------------------------
void FileTest::set_stdout()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the set_stdout.                                 
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the set_stdout method.                          
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::read                                             
//------------------------------------------------------------------------------
void FileTest::read()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the read.                                       
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the read method.                                
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::write                                            
//------------------------------------------------------------------------------
void FileTest::write()
{
    //--------------------------------------------------------------------------
    //  Write until we run out of space on the device.                          
    //--------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::rewindPre                                        
//------------------------------------------------------------------------------
void FileTest::rewindPre()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the rewindPre.                                  
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the rewindPre method.                           
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::rewindPost                                       
//------------------------------------------------------------------------------
void FileTest::rewindPost()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the rewindPost.                                 
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the rewindPost method.                          
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::postSetInput                                     
//------------------------------------------------------------------------------
void FileTest::postSetInput()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the postSetInput.                               
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the postSetInput method.                        
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::abort                                            
//------------------------------------------------------------------------------
void FileTest::abort()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the abort.                                      
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the abort method.                               
}

//------------------------------------------------------------------------------
/// @brief Tests for the File::isAbort                                          
//------------------------------------------------------------------------------
void FileTest::isAbort()
{

    //--------------------------------------------------------------------------
    /// @test  Test to exercise the isAbort.                                    
    //--------------------------------------------------------------------------
    /// @todo  Add code to test the isAbort method.                             
}

void FileTest::remove()
{
    //--------------------------------------------------------------------------
    //  Remove is tested in other methods.                                      
    //--------------------------------------------------------------------------
}

} // namespace test 
} // namespace work
} // namespace lib

