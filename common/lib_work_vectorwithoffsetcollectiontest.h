//------------------------------------------------------------------------------
///@file lib_work_vectorwithoffsetcollectiontest.h                              
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_WORK_VECTORWITHOFFSETCOLLECTIONTEST_H
#define LIB_WORK_VECTORWITHOFFSETCOLLECTIONTEST_H

#include "dev_test_work_test.h"
#include <string>

namespace lib {
namespace work {
namespace test {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: VectorWithOffsetCollectionTest                                   
///                                                                             
///@par Purpose:                                                                
///         The VectorWithOffsetCollectionTest class provides the regression test for the
///         lib::work::VectorWithOffsetCollection class.                        
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2015-10-25  MTU     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class VectorWithOffsetCollectionTest : public dev::test::work::Test 
{
    public:
        VectorWithOffsetCollectionTest();
        VectorWithOffsetCollectionTest(const VectorWithOffsetCollectionTest& that);
        virtual ~VectorWithOffsetCollectionTest();
        VectorWithOffsetCollectionTest& operator=(const VectorWithOffsetCollectionTest& that);

    protected:
        void runTest();

        void VectorWithOffsetCollection();
        void operatorBrackets();
        void minimum();
        void size();
        void free();
        void memory_used();
        void push_back();
        void memcpy();
        void empty();
        void vectorFreedEvent();

    private:
        std::string m_FileName;

}; //  class VectorWithOffsetCollectionTest : public dev::Test //

} // namespace test 
} // namespace work
} // namespace lib



#endif // #ifndef LIB_WORK_VECTORWITHOFFSETCOLLECTIONTEST_H //

