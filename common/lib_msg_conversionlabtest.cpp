//------------------------------------------------------------------------------
///@file lib_msg_conversionlabtest.cpp                                          
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Changed typedef to using                        
///                             Replaced #include lib_boost.h                   
///                                                                             
///@version 2014-08-07  MTU         File creation                               
//------------------------------------------------------------------------------
#include "lib_msg_conversionlabtest.h"

#include "dev_test_work.h"
#include "dev_test_work_test.h"
//#include "lib_boost.h"                  // lib::new_shared
#include "lib_config_work_filepaths.h"
#include "lib_ds_shared_ptr.h"
#include "lib_mp_work_threadablecollection.h"
#include "lib_msg_conversionlab.h"
#include "lib_work_conversionlab.h"

#include "debug.h"

namespace lib {
namespace msg {
namespace test {

using DoublePtr = lib::ds::shared_ptr<double>;
using ConstDoublePtr = lib::ds::shared_ptr<const double>;

using IntegerPtr = lib::ds::shared_ptr<int>;
using ConstIntegerPtr = lib::ds::shared_ptr<const int>;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace work {
class TestByPointer
    : public lib::work::ConversionLabByPointer<double, int>
{
    public:
        void finalize() 
        {
            IntegerPtr i;
            lib::new_shared(i, 3); push_back(i);
            lib::new_shared(i, 2); push_back(i);
            lib::new_shared(i, 1); push_back(i);
            lib::new_shared(i, 0); push_back(i);
        }

        void put(ConstDoublePtr d)
        {
            IntegerPtr i;
            lib::new_shared(i, *d);
            push_back(i);
        }
};

//------------------------------------------------------------------------------
//  We want the ByReference to produce different results than ByPointer, so     
//  ByReference multiplies the value by 2 (and ByPointer does not).             
//------------------------------------------------------------------------------
class TestByReference
    : public lib::work::ConversionLabByReference<double, int>
{
    public:
        void put(const double& d)
        {
            IntegerPtr i;
            lib::new_shared(i, d * 2);  
            push_back(i);
        }
};
} // namespace work

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
using TestByPointer = lib::msg::ConversionLab<work::TestByPointer, double, int>;

using TestByPointerPtr = lib::ds::shared_ptr<TestByPointer>;

using TestByReference = lib::msg::ConversionLab<work::TestByReference, double, int>;

using TestByReferencePtr = lib::ds::shared_ptr<TestByReference>;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class TestSubscriber
    : public lib::msg::Subscriber<int>
    , public std::vector<int>
{
    public:
        void process(ConstIntegerPtr& i) { push_back(*i); }

        size_t size() const { return std::vector<int>::size(); }
};
using TestSubscriberPtr = lib::ds::shared_ptr<TestSubscriber>;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class TestPublisher
    : public lib::msg::Publisher<double>
    , public lib::mp::work::Threadable
{
    public:
        void operator()()
        {
            DoublePtr d;

            lib::new_shared(d, 10); publish(d);
            lib::new_shared(d, 20); publish(d);
            lib::new_shared(d, 30); publish(d);
            lib::new_shared(d, 40); publish(d);

            endPublication();
        }
};
using TestPublisherPtr = lib::ds::shared_ptr<TestPublisher>;

//------------------------------------------------------------------------------
//  Register the test class for the dev_test_work_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::msg::test::ConversionLabTest);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
ConversionLabTest::ConversionLabTest() : Test("lib::msg::ConversionLab")
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    ConversionLabTest object to copy.                           
//------------------------------------------------------------------------------
ConversionLabTest::ConversionLabTest(const ConversionLabTest& that) : Test(that)
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
ConversionLabTest::~ConversionLabTest()

{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
ConversionLabTest& ConversionLabTest::operator=(const ConversionLabTest& that)
{
    Test::operator=(that);
    return *this;
} // ConversionLabTest::operator=(const ConversionLabTest& that) //

//------------------------------------------------------------------------------
/// @brief Execute the various test for ConversionLab.                          
//------------------------------------------------------------------------------
void ConversionLabTest::runTest()
{
    DoublePtr   d;
    lib::new_shared(d);

    //==========================================================================
    //  Since all of the methods are interrelated, we're going to test them all 
    //  in one routine.                                                         
    //==========================================================================

    //--------------------------------------------------------------------------
    //  Test ConversionByPointer                                                
    //--------------------------------------------------------------------------
    {

        lib::mp::work::ThreadableCollection threads;

        TestPublisherPtr publisher;
        lib::new_shared(publisher);
        threads.push_back(publisher);

        TestByPointerPtr byPointer;
        lib::new_shared(byPointer);
        threads.push_back(byPointer);

        TestSubscriberPtr subscriber;
        lib::new_shared(subscriber);
        threads.push_back(subscriber);

        publisher->connect(byPointer);
        byPointer->connect(subscriber);

        threads.startAll();

        threads.joinAll();

        TEST(subscriber->size() == 8);  // 4 + 4 of finalize    
        TEST(subscriber->at(0) == 10);
        TEST(subscriber->at(1) == 20);
        TEST(subscriber->at(2) == 30);
        TEST(subscriber->at(3) == 40);

    }

    //--------------------------------------------------------------------------
    //  Test ConversionByReference                                              
    //--------------------------------------------------------------------------
    {
        lib::mp::work::ThreadableCollection threads;

        TestPublisherPtr publisher;
        lib::new_shared(publisher);
        threads.push_back(publisher);

        TestByReferencePtr byReference;
        lib::new_shared(byReference);
        threads.push_back(byReference);

        TestSubscriberPtr subscriber;
        lib::new_shared(subscriber);
        threads.push_back(subscriber);

        publisher->connect(byReference);
        byReference->connect(subscriber);

        threads.startAll();

        threads.joinAll();

        TEST(subscriber->size() == 4);
        TEST(subscriber->at(0) == 20);
        TEST(subscriber->at(1) == 40);
        TEST(subscriber->at(2) == 60);
        TEST(subscriber->at(3) == 80);

    }
} // void ConversionLabTest::runTest() //

} // namespace test 
} // namespace msg
} // namespace lib

