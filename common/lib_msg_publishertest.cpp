//------------------------------------------------------------------------------
///@file lib_msg_publishertest.cpp                                              
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@author  Make Test Utility       MTU     Utility by DHF                      
///                                                                             
///@version 2020-09-23  JRS     cleanup for some warnings.                      
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Changed typedef to using                        
///                             Replaced #include lib_boost.h                   
///                                                                             
///@version 2015-08-30  MTU         File creation                               
//------------------------------------------------------------------------------
#include "lib_msg_publishertest.h"
#include "dev_test_work.h"
#include "dev_test_work_test.h"
#include "lib_config_work_filepaths.h"
#include "lib_msg_publisher.h"

//#include "lib_boost.h"
#include "lib_ds_shared_ptr.h"
#include "lib_ds_vectorwithoffset.h"
#include "lib_mp_work_threadablecollection.h"
#include <vector>

#include "debug.h"

namespace lib {
namespace msg {
namespace test {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int fillVector(std::vector<std::string>& vec, int offset = 0)
{
   if (offset >= 0 && vec.size() < 5) vec.push_back("George Washington");
   if (offset >= 1 && vec.size() < 5) vec.push_back("John Adams");
   if (offset >= 2 && vec.size() < 5) vec.push_back("Thomas Jefferson");
   if (offset >= 3 && vec.size() < 5) vec.push_back("James Madison");
   if (offset >= 4 && vec.size() < 5) vec.push_back("James Monroe");
   if (offset >= 5 && vec.size() < 5) vec.push_back("John Quincy Adams");
   if (offset >= 6 && vec.size() < 5) vec.push_back("Andrew Jackson");
   if (offset >= 7 && vec.size() < 5) vec.push_back("Martin Van Buren");
   if (offset >= 8 && vec.size() < 5) vec.push_back("William Henry Harrison");
   if (offset >= 9 && vec.size() < 5) vec.push_back("John Tyler");
   if (offset >= 10 && vec.size() < 5) vec.push_back("James K. Pok");
   if (offset >= 11 && vec.size() < 5) vec.push_back("Zachary Taylor");
   if (offset >= 12 && vec.size() < 5) vec.push_back("Milliard Fillmore");
   if (offset >= 13 && vec.size() < 5) vec.push_back("Franklin Pierce");
   if (offset >= 14 && vec.size() < 5) vec.push_back("James Buchanan");
   if (offset >= 15 && vec.size() < 5) vec.push_back("Abraham Lincoln");
   if (offset >= 16 && vec.size() < 5) vec.push_back("Andrew Johnson");
   if (offset >= 17 && vec.size() < 5) vec.push_back("Ulysses S. Grant");
   if (offset >= 18 && vec.size() < 5) vec.push_back("Rutherford B. Hayes");
   if (offset >= 19 && vec.size() < 5) vec.push_back("James A. Garfield");
   if (offset >= 20 && vec.size() < 5) vec.push_back("Chester A Arthur");
   if (offset >= 21 && vec.size() < 5) vec.push_back("Grover Cleveland");
   if (offset >= 22 && vec.size() < 5) vec.push_back("Benjamin Harrison");
   if (offset >= 23 && vec.size() < 5) vec.push_back("Grover Cleveland");
   if (offset >= 24 && vec.size() < 5) vec.push_back("William McKinley");
   if (offset >= 25 && vec.size() < 5) vec.push_back("Theodore Roosevelt");
   if (offset >= 26 && vec.size() < 5) vec.push_back("William Howard Taft");
   if (offset >= 27 && vec.size() < 5) vec.push_back("Woodrow Wilson");
   if (offset >= 28 && vec.size() < 5) vec.push_back("Warren G. Harding");
   if (offset >= 29 && vec.size() < 5) vec.push_back("Calvin Coolidge");
   if (offset >= 30 && vec.size() < 5) vec.push_back("Herbert Hoover");
   if (offset >= 31 && vec.size() < 5) vec.push_back("Franklin D. Roosevelt");
   if (offset >= 32 && vec.size() < 5) vec.push_back("Harry S Truman");
   if (offset >= 33 && vec.size() < 5) vec.push_back("Dwight D. Eisenhower");
   if (offset >= 34 && vec.size() < 5) vec.push_back("John F. Kennedy");
   if (offset >= 35 && vec.size() < 5) vec.push_back("Lyndon B. Johnson");
   if (offset >= 36 && vec.size() < 5) vec.push_back("Richard M. Nixon");
   if (offset >= 37 && vec.size() < 5) vec.push_back("Gerald Ford");
   if (offset >= 38 && vec.size() < 5) vec.push_back("James Carter");
   if (offset >= 39 && vec.size() < 5) vec.push_back("Ronald Reagan");
   if (offset >= 40 && vec.size() < 5) vec.push_back("George H. W. Bush");
   if (offset >= 41 && vec.size() < 5) vec.push_back("William J. Clinton");
   if (offset >= 42 && vec.size() < 5) vec.push_back("George W. Bush");
   if (offset >= 43 && vec.size() < 5) vec.push_back("Barack H. Obama");

   return static_cast<int>(vec.size());
}

//------------------------------------------------------------------------------
class VectorPublisher
    : public lib::mp::work::Threadable
    , public lib::msg::Publisher<std::vector<std::string> >
{
    public:
        void operator()()
        {
            for (int i=0; i < 40; ++i) {
                lib::ds::shared_ptr<std::vector<std::string> > item;
                lib::new_shared(item);
                fillVector(*item);
                publish(item);
            }
            endPublication();
        }
};
using VectorPublisherPtr = lib::ds::shared_ptr<VectorPublisher>;

//------------------------------------------------------------------------------
class VectorWithOffsetPublisher
    : public lib::mp::work::Threadable
    , public lib::msg::Publisher<lib::ds::VectorWithOffset<std::string> >
{
    public:
        void operator()()
        {
            for (int i=0; i < 40; ++i) {
                lib::ds::shared_ptr<lib::ds::VectorWithOffset<std::string> > item;
                lib::new_shared(item);
                item->setFileOffset(i * 100);
                fillVector(*item);
                publish(item);
            }
            endPublication();
        }
};
using VectorWithOffsetPublisherPtr = lib::ds::shared_ptr<VectorWithOffsetPublisher>;

//------------------------------------------------------------------------------
class BothPublisher
    : public lib::mp::work::Threadable
    , public lib::msg::Publisher<
            lib::ds::VectorWithOffset<std::string> 
          , std::vector<std::string>
        >
{
    public:
        void operator()()
        {
            for (int i=0; i < 40; ++i) {
                lib::ds::shared_ptr<lib::ds::VectorWithOffset<std::string> > item;
                lib::new_shared(item);
                item->setFileOffset(i * 100);
                fillVector(*item);
                publish(item);

                lib::ds::shared_ptr<std::vector<std::string> > sv;
                lib::new_shared(sv);
                fillVector(*sv);
                publish(sv);
            }
            endPublication();
        }
};
using BothPublisherPtr = lib::ds::shared_ptr<BothPublisher>;


//------------------------------------------------------------------------------
class VectorSubscriber
    : public lib::msg::Subscriber<std::vector<std::string> >
{
    public:
        VectorSubscriber() : m_StdCount(0) { }
        void process(lib::ds::shared_ptr<const std::vector<std::string> >& v)
        {
            ++m_StdCount;
        }

        int m_StdCount;
        
};
using VectorSubscriberPtr = lib::ds::shared_ptr<VectorSubscriber>;


//------------------------------------------------------------------------------
class VectorWithOffsetSubscriber
    : public lib::msg::Subscriber<lib::ds::VectorWithOffset<std::string> >
{
    public:
        VectorWithOffsetSubscriber() : m_LibCount(0) { }
        void process(
            lib::ds::shared_ptr<const lib::ds::VectorWithOffset<std::string> >& v
        )
        {
            ++m_LibCount;
        }

        int m_LibCount;
        
};
using VectorWithOffsetSubscriberPtr = lib::ds::shared_ptr<VectorWithOffsetSubscriber>;

//------------------------------------------------------------------------------
class BothSubscriber
    : public lib::msg::Subscriber<
        std::vector<std::string>
      , lib::ds::VectorWithOffset<std::string> 
    >
{
    public:
        BothSubscriber() : m_LibCount(0), m_StdCount(0) { }

        void process(
            lib::ds::shared_ptr<const lib::ds::VectorWithOffset<std::string> >& v
        )
        {
            ++m_LibCount;
        }
        void process(
            lib::ds::shared_ptr<const std::vector<std::string> >& v
        )
        {
            ++m_StdCount;
        }

        int m_LibCount;
        int m_StdCount;
        
};
using BothSubscriberPtr = lib::ds::shared_ptr<BothSubscriber>;




//------------------------------------------------------------------------------
//  Register the test class for the dev_test_work_test_classes.                           
//------------------------------------------------------------------------------
TEST_REGISTER(lib::msg::test::PublisherTest);

//------------------------------------------------------------------------------
/// @brief Default constructor                                                  
//------------------------------------------------------------------------------
PublisherTest::PublisherTest() : Test("lib::msg::Publisher")
{
    //  This space intentionally left mostly blank.                             
}

//------------------------------------------------------------------------------
/// @brief Copy constructor                                                     
/// @param  that    PublisherTest object to copy.                               
//------------------------------------------------------------------------------
PublisherTest::PublisherTest(const PublisherTest& that) : Test(that)
{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Destructor                                                           
//------------------------------------------------------------------------------
PublisherTest::~PublisherTest()

{
    //  This space intentionally left mostly blank.                             
}


//------------------------------------------------------------------------------
/// @brief Assignment operator.                                                 
//------------------------------------------------------------------------------
PublisherTest& PublisherTest::operator=(const PublisherTest& that)
{
    Test::operator=(that);
    return *this;
} // PublisherTest::operator=(const PublisherTest& that) //


//------------------------------------------------------------------------------
/// @brief Execute the various test for Publisher.                              
//------------------------------------------------------------------------------
void PublisherTest::runTest()
{
    Publisher();
    publish();
    connect();
    endPublication();
    subscriptionCount();

} // void PublisherTest::runTest() //


//------------------------------------------------------------------------------
/// @brief Tests for the Publisher::Publisher                                   
//------------------------------------------------------------------------------
void PublisherTest::Publisher()
{
    //--------------------------------------------------------------------------
    //  The constructor is effectively tested in the other test methods.        
    //--------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
/// @brief Tests for the Publisher::publish                                     
///                                                                             
///         There are two data types being published/subscribed-to: std::vector 
///         and lib::ds::VectorWithOffset.  The latter is derived from the      
///         former.                                                             
//------------------------------------------------------------------------------
void PublisherTest::publish()
{
    //--------------------------------------------------------------------------
    //  [vector-type][function] where                                           
    //      vector:                                                             
    //          s = std::vector (s = std)                                       
    //          l = lib::ds::VectorWithOffset   (l = lib)                       
    //          b = both vectors (b = both)                                     
    //      function:                                                           
    //          p = publisher                                                   
    //          s = subscriber                                                  
    //--------------------------------------------------------------------------
    VectorPublisherPtr sp;
    VectorWithOffsetPublisherPtr lp;
    BothPublisherPtr bp;

    VectorSubscriberPtr ss;
    VectorWithOffsetSubscriberPtr ls;
    BothSubscriberPtr bs;

    //--------------------------------------------------------------------------
    //  Test std::vector publisher to std::vector subscriber.  Same type being  
    //  used,  they should connect with nothing special being done.             
    //--------------------------------------------------------------------------
    {
        lib::new_shared(sp);
        lib::new_shared(ss);

        sp->connect(ss);

        TEST(sp->subscriptionCount() == 1);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(sp);
        threads.push_back(ss);

        threads.startAll();
        threads.joinAll();

        TEST(ss->m_StdCount == 40);
    }

    //--------------------------------------------------------------------------
    //  Test lib::ds::VectorWithOffset publisher to std::vector subscriber.     
    //  With the "allowDerived" set to true, std::vector subscriber should      
    //  receive okay.                                                           
    //--------------------------------------------------------------------------
    {
        lib::new_shared(lp);
        lib::new_shared(ss);

        lp->connect(ss, true);

        TEST(lp->subscriptionCount() == 1);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(lp);
        threads.push_back(ss);

        threads.startAll();
        threads.joinAll();

        TEST(ss->m_StdCount == 40);
    }

    //--------------------------------------------------------------------------
    //  Same as above, but this time the connect says derived aren't allowed;   
    //  therefore none of the published items should be received.               
    //--------------------------------------------------------------------------
    {
        lib::new_shared(lp);
        lib::new_shared(ss);

        try {
            lp->connect(ss);
        } catch(...) {
            // no common type
        }

        TEST(lp->subscriptionCount() == 0);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(lp);
        threads.push_back(ss);

        threads.startAll();
        threads.joinAll();

        TEST(ss->m_StdCount == 0);
    }

    //--------------------------------------------------------------------------
    //  Exact same as before, but explicitly don't allow derived types.         
    //--------------------------------------------------------------------------
    {
        lib::new_shared(lp);
        lib::new_shared(ss);

        try {
            lp->connect(ss, false);
        } catch(...) {
            // no common type
        }

        TEST(lp->subscriptionCount() == 0);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(lp);
        threads.push_back(ss);

        threads.startAll();
        threads.joinAll();

        TEST(ss->m_StdCount == 0);
    }

    //--------------------------------------------------------------------------
    //  std::vector publisher with "universal" subscriber.                      
    //--------------------------------------------------------------------------
    {
        lib::new_shared(sp);
        lib::new_shared(bs);

        sp->connect(bs);

        TEST(sp->subscriptionCount() == 1);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(sp);
        threads.push_back(bs);

        threads.startAll();
        threads.joinAll();

        TEST(bs->m_StdCount == 40);
        TEST(bs->m_LibCount == 0);
    }

    //--------------------------------------------------------------------------
    //  lib::ds::Vector publisher with universal subscriber.  Note that even    
    //  though both "process" methods can receive the lib::ds::Vector, it should
    //  only be connected to the lib:: version (most compatible).               
    //--------------------------------------------------------------------------
    {
        lib::new_shared(lp);
        lib::new_shared(bs);

        lp->connect(bs);

        TEST(lp->subscriptionCount() == 1);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(lp);
        threads.push_back(bs);

        threads.startAll();
        threads.joinAll();

        TEST(bs->m_StdCount == 0);
        TEST(bs->m_LibCount == 40);
    }
    //--------------------------------------------------------------------------
    //  Do the same thing, but this time tell the connect to allow derived      
    //  types.  It should still only connect with the same-type.                
    //--------------------------------------------------------------------------
    {
        lib::new_shared(lp);
        lib::new_shared(bs);

        lp->connect(bs);

        TEST(lp->subscriptionCount() == 1);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(lp);
        threads.push_back(bs);

        threads.startAll();
        threads.joinAll();

        TEST(bs->m_StdCount == 0);
        TEST(bs->m_LibCount == 40);
    }

    //--------------------------------------------------------------------------
    //  Both types of publishers to the universal subscriber.                   
    //--------------------------------------------------------------------------
    {
        lib::new_shared(lp);
        lib::new_shared(sp);
        lib::new_shared(bs);

        lp->connect(bs);
        sp->connect(bs);

        TEST(lp->subscriptionCount() == 1);
        TEST(sp->subscriptionCount() == 1);

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(sp);
        threads.push_back(lp);
        threads.push_back(bs);

        threads.startAll();
        threads.joinAll();

        TEST(bs->m_StdCount == 40);
        TEST(bs->m_LibCount == 40);
    }

    //--------------------------------------------------------------------------
    //  Both publisher to all types of subscribers.  Only the both subscriber   
    //  will process both.                                                      
    //--------------------------------------------------------------------------
    {
        lib::new_shared(bp);
        lib::new_shared(ss);
        lib::new_shared(ls);
        lib::new_shared(bs);

        bp->connect(ss);
        TEST(bp->subscriptionCount() == 1);

        bp->connect(ls);
        TEST(bp->subscriptionCount() == 2);

        bp->connect(bs);
        TEST(bp->subscriptionCount() == 4); // both types makes +2

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(bp);
        threads.push_back(ss);
        threads.push_back(ls);
        threads.push_back(bs);

        threads.startAll();
        threads.joinAll();

        TEST(ss->m_StdCount == 40);
        TEST(ls->m_LibCount == 40);

        TEST(bs->m_LibCount == 40);
        TEST(bs->m_StdCount == 40);

    }

    //--------------------------------------------------------------------------
    //  Same as above, but allow derived ...                                    
    //--------------------------------------------------------------------------
    {
        lib::new_shared(bp);
        lib::new_shared(ss);
        lib::new_shared(ls);
        lib::new_shared(bs);

        bp->connect(ss, true);
        TEST(bp->subscriptionCount() == 2); // both types to std::vector

        bp->connect(ls, true);
        TEST(bp->subscriptionCount() == 3); // only one type to lib::vector

        bp->connect(bs, true);
        TEST(bp->subscriptionCount() == 5); // both types; ony once

        lib::mp::work::ThreadableCollection threads;
        threads.push_back(bp);
        threads.push_back(ss);
        threads.push_back(ls);
        threads.push_back(bs);

        threads.startAll();
        threads.joinAll();

        TEST(ss->m_StdCount == 80);

        TEST(ls->m_LibCount == 40);

        TEST(bs->m_LibCount == 40);
        TEST(bs->m_StdCount == 40);

    }
}
                                                                                

//------------------------------------------------------------------------------
/// @brief Tests for the Publisher::connect                                     
//------------------------------------------------------------------------------
void PublisherTest::connect()
{
    //--------------------------------------------------------------------------
    //  pub-sub the same.                                                       
    //--------------------------------------------------------------------------
    try {
        VectorPublisher  pub;
        VectorSubscriber sub;
        pub.connect(sub);
        PASS("connection made");
    } catch(...) {
        FAIL("exception thrown unexpectedly");
    }

    //--------------------------------------------------------------------------
    //  pub-sub the same.                                                       
    //--------------------------------------------------------------------------
    try {
        VectorWithOffsetPublisher  pub;
        VectorWithOffsetSubscriber sub;
        pub.connect(sub);
        PASS("connection made");
    } catch(...) {
        FAIL("exception thrown unexpectedly");
    }

    //--------------------------------------------------------------------------
    //  pub-sub the different                                                   
    //--------------------------------------------------------------------------
    try {
        VectorPublisher  pub;
        VectorWithOffsetSubscriber sub;
        pub.connect(sub);
        FAIL("unexpected connection made");
    } catch(const VectorPublisher::ConnectionError& e) {
        PASS(std::string("expected exception thrown ") + e.what());
    } catch(...) {
        FAIL("unknown exception thrown");
    }

    //--------------------------------------------------------------------------
    //  sub-type derived from pub-type; throw because must be same              
    //--------------------------------------------------------------------------
    try {
        VectorWithOffsetPublisher  pub;
        VectorSubscriber sub;
        pub.connect(sub);
        FAIL("unexpected connection made");
    } catch(const VectorWithOffsetPublisher::ConnectionError& e) {
        PASS(std::string("expected exception thrown ") + e.what());
    } catch(...) {
        FAIL("unknown exception thrown");
    }

    //--------------------------------------------------------------------------
    //  sub-type derived from pub-type; works because may be derived            
    //--------------------------------------------------------------------------
    try {
        VectorWithOffsetPublisher  pub;
        VectorSubscriber sub;
        pub.connect(sub, true);
        PASS("connection made");
    } catch(const VectorWithOffsetPublisher::ConnectionError& e) {
        FAIL(std::string("unexpected exception thrown ") + e.what());
    } catch(...) {
        FAIL("unknown exception thrown");
    }

}


//------------------------------------------------------------------------------
/// @brief Tests for the Publisher::endPublication                              
//------------------------------------------------------------------------------
void PublisherTest::endPublication()
{
    //--------------------------------------------------------------------------
    //  endPublication is effectively tested in the publish method.  If we hang 
    //  then endPublication isn't working.  8^}                                 
    //--------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
/// @brief Tests for the Publisher::subscriptionCount                           
//------------------------------------------------------------------------------
void PublisherTest::subscriptionCount()
{
    VectorWithOffsetPublisher pub;
    TEST(pub.subscriptionCount() == 0);
    
    VectorSubscriber sub_v;
    try {
        pub.connect(sub_v);
    } catch(...) {
        //----------------------------------------------------------------------
        // we expect an exception ... and the subscription count not to change  
        //----------------------------------------------------------------------
    }
    TEST(pub.subscriptionCount() == 0);
    
    pub.connect(sub_v, true);
    TEST(pub.subscriptionCount() == 1);
    
    VectorWithOffsetSubscriber sub_vwo;
    pub.connect(sub_vwo);
    TEST(pub.subscriptionCount() == 2);

    BothPublisher bp;
    BothSubscriber bs;

    bp.connect(bs);
    TEST(bp.subscriptionCount() == 2);  // both types
}


} // namespace test                                                             
} // namespace msg                                                              
} // namespace lib                                                              


