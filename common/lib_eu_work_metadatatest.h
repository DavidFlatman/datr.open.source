//------------------------------------------------------------------------------
///@file lib_eu_work_metadatatest.h                                             
//------------------------------------------------------------------------------
#ifndef LIB_EU_WORK_METADATATEST_H
#define LIB_EU_WORK_METADATATEST_H

#include "dev_test_work_test.h"

namespace lib {
namespace eu {
namespace work {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: MetaDataTest                                                     
///                                                                             
///@par Purpose:                                                                
///         The MetaDataTest class provides the regression test for the         
///         lib::eu::work::MetaData class.                                      
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-08-27  DHF     File creation via Make Test Utility             
///                                                                             
//------------------------------------------------------------------------------
class MetaDataTest : public dev::test::work::Test {
    public:
        MetaDataTest();
        MetaDataTest(const MetaDataTest& that);
        virtual ~MetaDataTest();
        MetaDataTest& operator=(const MetaDataTest& that);

    protected:
        void runTest();


    private:

}; //  class MetaDataTest : public dev::test::work::Test //

} // namespace test 
} // namespace work
} // namespace eu
} // namespace lib



#endif // #ifndef LIB_EU_WORK_METADATATEST_H //

