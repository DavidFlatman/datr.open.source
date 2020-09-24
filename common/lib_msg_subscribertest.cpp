//------------------------------------------------------------------------------
///\file lib_msg_subscribertest.cpp                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#include "lib_msg_subscribertest.h"
#include "dev_test_work.h"
#include "dev_test_work_test.h"
#include "lib_msg_subscriber.h"
#include "lib_msg_publisher.h"

#include "lib_mp_work_threadable.h"
#include "lib_mp_work_threadablecollection.h"

#include "debug.h"


namespace lib {
namespace msg {
namespace test {

TEST_REGISTER(lib::msg::test::SubscriberTest);

//------------------------------------------------------------------------------
///                                                                             
///@par Class: SubscriberTest                                                   
///                                                                             
///@author                                                                      
///         Make Test Utility           MTU         Utility by DHF              
///                                                                             
///@version 2020-09-24  JRS     updated with automated C++ 11 recommendations.  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-09-09  MTU     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Method:  SubscriberTest                                                     
//------------------------------------------------------------------------------
SubscriberTest::SubscriberTest() : Test("lib::msg::Subscriber") {}
SubscriberTest::SubscriberTest(const SubscriberTest& that) = default;

//------------------------------------------------------------------------------
//  Method:  ~SubscriberTest                                                    
//------------------------------------------------------------------------------
SubscriberTest::~SubscriberTest() = default;

//------------------------------------------------------------------------------
//  Method:  operator=                                                          
//------------------------------------------------------------------------------
SubscriberTest& SubscriberTest::operator=(const SubscriberTest& that)
{
    dev::test::work::Test::operator=(that);
    return *this;
} // SubscriberTest::operator=(const SubscriberTest& that) //


class MySubscriberTest
    : public lib::msg::Subscriber<int, double>
    , dev::test::work::TestProxy
{
    public:
        MySubscriberTest(SubscriberTest* t)
            : dev::test::work::TestProxy(t)
            , m_CountInteger(0)
            , m_CountDouble(0)
        {
        }

        void process(lib::ds::shared_ptr<const int>& i) override
        {
            switch (m_CountInteger)
            {
                case 0: TEST(*i == 0); break;
                case 1: TEST(*i == 2); break;
                case 2: TEST(*i == 4); break;
                case 3: TEST(*i == 6); break;
                case 4: TEST(*i == 8); break;
                default:
                    FAIL(
                        lib::format(
                            "Unexpected integer %d/%d", *i, m_CountInteger
                        )
                    );
                    break;
            }
            ++m_CountInteger;
        }

        void process(lib::ds::shared_ptr<const double>& i) override
        {
            switch (m_CountDouble)
            {
                case 0: TEST(*i == 0.0); break;
                case 1: TEST(*i == 0.5); break;
                case 2: TEST(*i == 1.0); break;
                case 3: TEST(*i == 1.5); break;
                case 4: TEST(*i == 2.0); break;
                default:
                    FAIL(
                        lib::format(
                            "Unexpected integer %f/%d", *i, m_CountInteger
                        )
                    );
                    break;
            }
            ++m_CountDouble;
        }
        int countInteger() { return m_CountInteger; }

        int countDouble() { return m_CountDouble; }

    private:
        int                 m_CountInteger;
        int                 m_CountDouble;
};
using MySubscriberTestPtr = lib::ds::shared_ptr<MySubscriberTest>;

class MyPublisherTest
    : public lib::msg::Publisher<int, double>
    , public lib::mp::work::Threadable
{
    public:
        void operator()() override
        {
            for (int i=0; i < 5; ++i)
            {
                lib::ds::shared_ptr<int>    p_int(new int(i * 2));
                lib::ds::shared_ptr<double> p_dbl(new double(i * 0.5));

                publish(p_int);
                publish(p_dbl);
            }
            endPublication();
        }
};
using MyPublisherTestPtr = lib::ds::shared_ptr<MyPublisherTest>;

//------------------------------------------------------------------------------
//  Method:  runTest                                                            
//------------------------------------------------------------------------------
void SubscriberTest::runTest() 
{
    lib::mp::work::ThreadableCollection threads;

    MySubscriberTestPtr sub;
    MyPublisherTestPtr  pub;

    lib::new_shared(pub);
    lib::new_shared(sub, this);

    pub >> sub;

    TEST(sub->countInteger() == 0);
    TEST(sub->countDouble() == 0);

    threads.push_back(pub);
    threads.push_back(sub);

    threads.startAll();
    threads.joinAll();

    TEST(sub->countInteger() == 5);
    TEST(sub->countDouble() == 5);


} // SubscriberTest::runTest //

} // namespace test //
} // namespace msg //
} // namespace lib //
