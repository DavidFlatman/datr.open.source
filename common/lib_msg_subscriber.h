//------------------------------------------------------------------------------
///@file lib_msg_subscriber.h                                                   
///@brief Hold the definition of the lib::msg::Subscriber template & support    
///       classes.                                                              
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_MSG_SUBSCRIBER_H
#define LIB_MSG_SUBSCRIBER_H

//------------------------------------------------------------------------------
///@todo    We [read DHF] needs to look into why the warning overloaded-virtual 
///         is being issued.  I'm quasi assuming that there should be a way to  
///         get rid of it (maybe not though).                                   
//------------------------------------------------------------------------------
//#pragma clang diagnostic ignored "-Woverloaded-virtual"                       

#include "lib_ds_null.h"
#include "lib_ds_shared_ptr.h"
#include "lib_mp_work_queue.h"
#include "lib_mp_work_threadable.h"
#include "lib_msg_publishersubscriberbase.h"
#include "lib_msg_subscription.h"
#include "lib_msg_subscriptionbase.h"

#include <atomic>
#include <boost/signals2.hpp>
#include <stdint.h>
#include <string>

#include <stdio.h>

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///                                                                             
///@class Subscriber                                                            
///                                                                             
///@brief Base class to allow easy implementation of subscriber idiom.          
///                                                                             
///@par Template: Subscriber                                                    
///                                                                             
///@par Purpose:                                                                
///         The msg::Subscriber template is designed to do the drugery of       
///         "do we still have data" ... process it ... "do we still have data"  
///\n\n                                                                         
///         The design intent (but not a requirement) is that the class be used 
///         in a multi-threaded environment.  The operator()() is the boost     
///         thread's main routine; the method inbox is a slot to receive data   
///         to be processed (which is queued up for processing).                
///\n\n                                                                         
///         The first (much simpler) implementation allowed the subscriber to   
///         only subscribe to one type.  We then had a need to subscribe to two 
///         different types and [hopefully] process them in the order received. 
///                                                                             
///@par The Problem                                                             
///         We want our Subscriber in our Publisher / Subscriber world to be    
///         able to subscribe to multiple types of things at one time (and      
///         have them [basically] processed in the order received).             
///\n\n                                                                         
///         We had wanted to have a structure that contained a union of the     
///         various types that were subscribed to ... but a C++ union cannot    
///         contain items that have constructors (something about constructing  
///         on top of memory that other classes are constructing on -- wimps).  
///                                                                             
///@par Design                                                                  
///         In order to be useful, the Subscriber has to be template driven.    
///         But a std::queue can only hold one type of data ... but each        
///         subscription is of a different type.                                
///\n\n                                                                         
///         Since we can't have a queue of pointers to the Subscription because 
///         of the differing template types, we instead make a base class that  
///         isn't dependent on the template type but has a mechanism (pure      
///         virtual method) to bring back the template version.                 
///\n\n                                                                         
///         We then have our queue be a queue of pointers to the subscription   
///         base class (SubscriptionBase) stripping off the various template    
///         parameters -- making a single, consistent type.  We then execute    
///         the pure virtual method which gives us back templated type.         
///                                                                             
///@par Possible Usage:                                                         
///    @code                                                                    
///        #include "myrecord.h"                                                
///        #include "myproducer.h"                                              
///                                                                             
///        typedef boost::shared_ptr<MyRecord> MyRecordPtr;                     
///                                                                             
///        class MySubscriber : public lib::msg::Subscriber<MyRecordPtr>        
///        {                                                                    
///             public:                                                         
///                 void process(MyRecordPtr& x);                               
///        };                                                                   
///                                                                             
///        int main(int argc, char* argv[])                                     
///        {                                                                    
///            MyProducer p;                                                    
///            MySubscriber s;                                                  
///                                                                             
///            p.connect(s);                                                    
///                                                                             
///            boost::thread t1(boost::ref(s));                                 
///            boost::thread t2(boost::ref(p));                                 
///                                                                             
///            t1.join();                                                       
///            t2.join();                                                       
///        }                                                                    
///    @endcode                                                                 
///\n\n                                                                         
///         Note that the above code sample assumes that the publisher and      
///         subscriber each only operate on one type.  If more than one type is 
///         involved the the code should look like:                             
///\n\n                                                                         
///     @code                                                                   
///         int main(int argc, char* argv[])                                    
///         {                                                                   
///             lib::irig106::ch10::msg::FilePublisher publisher("file.ch10");  
///             lib::irig106::ch10::msg::ToPacketAndTMATS tmats;                
///             DebugPacket debug;                                              
///                                                                             
///             //--------------------------------------------------------------
///             // The publisher object only publishes one type (PacketPtr).    
///             // The tmats subscriber object only subscribes to one type.     
///             // Therefore we can use the simple connect.                     
///             //--------------------------------------------------------------
///             publisher.connect(tmats);                                       
///                                                                             
///             //--------------------------------------------------------------
///             // The tmats publisher object publishes two different types, so 
///             // we either have to call connect twice (specifying the type    
///             // each time) or we can use the SUBSCRIBE macro defined in      
///             // lib_msg_publisher.h                                          
///             //                                                              
///             // The debug object subscribes to two different types (both     
///             // coincidently published by tmats -- this is not a requirement)
///             //--------------------------------------------------------------
///             SUBSCRIBE(lib::irig106::ch10::work::PacketPtr, tmats, debug);   
///             SUBSCRIBE(lib::irig106::ch9::TMATSPtr, tmats, debug);           
///                                                                             
///             //--------------------------------------------------------------
///             //  For humor value we're starting the subscribers before the   
///             //  publishers.  If we start the publisher's first -- and there 
///             //  is no govenor on the subscriber queue -- then we can fill   
///             //  memory pretty quickly before the subscriber's thread is     
///             //  is even started to process the data.                        
///             //--------------------------------------------------------------
///             boost::thread t1(boost::ref(debug));                            
///             boost::thread t2(boost::ref(tmats));                            
///             boost::thread t3(boost::ref(publisher));                        
///                                                                             
///             t1.join();                                                      
///             t2.join();                                                      
///             t3.join();                                                      
///                                                                             
///         }                                                                   
///     @endcode                                                                
///                                                                             
///@par Thread Safety:  object                                                  
///         There is an implicit assumption that only one msg::Producer object  
///         will be feeding data to a msg::Subscriber object (at a time).  If   
///         this is a bad assumption, the publicationEnding code (at least) will
///         have to be modified.                                                
///                                                                             
///@version 2020-05-06  DHF     Removed lib_atomic.h in favor of <atomic>.      
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                             Replaced #include <boost/shared_ptr.hpp>        
///                                                                             
///@version 2019-02-20  DHF         -- v2.2.4                                   
///             -   Made the template parameters accessible to other code via   
///                 the using statements.                                       
///             -   Made the PublisherSubscriberBase public so that external    
///                 code can call endPublication in case the object's main      
///                 thread abnormally terminates.                               
///                                                                             
///@version 2016-11-14  -- DHF      -- v2.2.3                                   
///             -   Moved lib::mp::work::Queue from base class to member variable.
///             -   Added addToParentQueue.                                     
///             -   Accomodated changes to lib::msg::Subscription.              
///                                                                             
///@version 2016-11-01  -- DHF      --  v2.2.2                                  
///             -   Moved lib::msg::NULL* to lib::ds::NULL*                     
///                                                                             
///@version 2016-02-08  -- DHF      --  v2.2.1                                  
///             -   Changed the default governor (max_size) to 100.             
///                                                                             
///@version 2015-11-23  --  DHF/MNT --  v2.2.0                                  
///             -   Added abort method.                                         
///             -   Added calling of abort when the subscriber exits.           
///             -   Initialized the m_Stop member variable (only really needed  
///                 by unit test code).                                         
///                                                                             
///@version 2015-04-23  --  DHF     --  v2.1.0                                  
///             -   Removed heart/heart-monitor code.                           
///                                                                             
///@version 2014-06-16  -- DHF      --  v2.0.0                                  
///             -   Renamed "finalize" to "beforeEndThread" to clarify when it  
///                 was being called (i.e., after the publication ended, but    
///                 before the thread terminated).                              
///                                                                             
///@version 2014-05-21  --  DHF     --  v1.2.0                                  
///             -   Added next() in order to break up the processing loop and   
///                 allow test routines greater control.                        
///                                                                             
///@version 2014-03-13  --  DHF                                                 
///             -   Added more commentary.                                      
///                                                                             
///@version 2013-10-29  --  DHF     --  v1.1.0                                  
///             -   Added lib::mp::Heart as base.                               
///             -   Added beat within operator()().                             
///                                                                             
///@version 2013-05-08  --  DHF     --  v1.0.1                                  
///             -   Made use of the NULL_DEFAULT_TYPE_0 - 7 in lib_msg_null.h   
///                                                                             
///@version 2010-01-26  --  DHF     --  v1.0.0                                  
///             -   Changed outer namespace from tools to lib.                  
///                                                                             
///@version 2009-09-09  --  DHF     --  v0.0.1                                  
///             -   Updated template to allow simultaneous subscription to 9    
///                 different types.                                            
///                                                                             
///@version 2009-08-11  --  DHF     --  v0.0.1                                  
///             -   Class creation                                              
///                                                                             
//------------------------------------------------------------------------------
template <
    typename TYPE0
  , typename TYPE1 = ds::NULL_DEFAULT_TYPE_1
  , typename TYPE2 = ds::NULL_DEFAULT_TYPE_2
  , typename TYPE3 = ds::NULL_DEFAULT_TYPE_3
  , typename TYPE4 = ds::NULL_DEFAULT_TYPE_4
  , typename TYPE5 = ds::NULL_DEFAULT_TYPE_5
  , typename TYPE6 = ds::NULL_DEFAULT_TYPE_6
  , typename TYPE7 = ds::NULL_DEFAULT_TYPE_7
  , typename TYPE8 = ds::NULL_DEFAULT_TYPE_8
>
class Subscriber 
    : public Subscription<TYPE0>
    , public Subscription<TYPE1>
    , public Subscription<TYPE2>
    , public Subscription<TYPE3>
    , public Subscription<TYPE4>
    , public Subscription<TYPE5>
    , public Subscription<TYPE6>
    , public Subscription<TYPE7>
    , public Subscription<TYPE8>
    , public lib::mp::work::Threadable
    , virtual public PublisherSubscriberBase
{
    public:
        //----------------------------------------------------------------------
        //  Give non-templated code access to the template parameters.          
        //----------------------------------------------------------------------
        using Type0 = TYPE0;
        using Type1 = TYPE1;
        using Type2 = TYPE2;
        using Type3 = TYPE3;
        using Type4 = TYPE4;
        using Type5 = TYPE5;
        using Type6 = TYPE6;
        using Type7 = TYPE7;
        using Type8 = TYPE8;

        //----------------------------------------------------------------------
        ///@brief Construct the Subscriber object.                              
        ///@param max_size  The maximum size the subscription should receive    
        ///                 before hanging the publishing thread.               
        ///                 The default 0 imposes no artifical limit on the     
        ///                 number of items that can be in the queue (if you    
        ///                 end up compute bound, this could be a very bad      
        ///                 thing).                                             
        ///\n\n                                                                 
        ///                 A reasonable value for the govenor (max size) is    
        ///                 based on a number of factors including the size of  
        ///                 the data types subscribed to (large types, smaller  
        ///                 max_size).                                          
        ///\n\n                                                                 
        ///                 If the max_size is too small, too much time will    
        ///                 be spent in thread context switching (not good      
        ///                 either).                                            
        //----------------------------------------------------------------------
        Subscriber(size_t max_size = 100) 
            : lib::mp::work::Threadable("anonymous")
            , m_PublicationCount(0)
            , m_Stop(false)
            , m_Queue(max_size)
            {
                setSubscriber();
            }

        Subscriber(const std::string& name, size_t max_size = 100)
            : lib::mp::work::Threadable(name)
            , m_PublicationCount(0)
            , m_Stop(false)
            , m_Queue(max_size)
            {
                setSubscriber();
            }

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        virtual ~Subscriber() 
        {
        }

        //----------------------------------------------------------------------
        ///@brief Called when a Publisher has terminated publishing; used to    
        ///       wake up the subscriber if the subscriber has hung up waiting  
        ///       for more data to process.                                     
        //----------------------------------------------------------------------
        virtual void publicationEnding()
        {
            --m_PublicationCount;

            if (m_PublicationCount <= 0) {
                m_Queue.setInterrupt(true);
            }
        }

        //----------------------------------------------------------------------
        ///@brief Once started (think thread main) keeps working until queue is 
        ///       empty and publicationEnding (or stopProcessing returns true)  
        ///       has been called.                                              
        //----------------------------------------------------------------------
        virtual void operator()() override
        {
            try {
                m_Stop = false;
                m_Stage = Stage::stage0;
                m_DoEndPublication = true;

                if (!initialize()) {
                    return;
                }

                while (!stopProcessing()) 
                {
                    next();
                }

                cleanUp();
            } catch(...) {
                cleanUp();
                throw;
            }
        }

        //----------------------------------------------------------------------
        ///@brief   Process the next item in the input queue.                   
        //----------------------------------------------------------------------
        virtual void next()
        {
            SubscriptionBase* item = NULL;
            if (m_Queue.pop(item)) {
                //beat();                                                       
                item->processQueueItem();

            }
        }

        //----------------------------------------------------------------------
        ///@par Design Decision:                                                
        ///         Nobody is really going to be subscribing to objects of      
        ///         NULL_DEFAULT_TYPE_n.  So, we implement the process methods  
        ///         for the default values of the template parameters then the  
        ///         user of the template isn't harassed by "you didn't         
        ///         implement this."  At the same time, we're *not* using TYPE1 
        ///         because if the user of the template actually passed in a    
        ///         parameter, then we want them to be harassed about any missing
        ///         'process' methods that they may have forgotten to implement.
        //----------------------------------------------------------------------
        virtual void process(
            lib::ds::shared_ptr<const TYPE0>& item
        ) = 0;
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_1>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_2>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_3>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_4>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_5>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_6>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_7>& item
        ) {}
        virtual void process(
            lib::ds::shared_ptr<const ds::NULL_DEFAULT_TYPE_8>& item
        ) {}

        void setGovernor(size_t s) { m_Queue.setGovernor(s); }
        size_t governor() const { return m_Queue.governor(); }

        //----------------------------------------------------------------------
        ///@brief Make the operator()()'s while statement easier to write.      
        ///@return true = There's nothing left in our queue to process and our  
        ///        publishers have quit.                                        
        //----------------------------------------------------------------------
        virtual bool stopProcessing()
        {
            if (m_Stop) return true;

            if (m_Queue.empty() && m_PublicationCount == 0) {
                return true;
            }
            if (isPublisher() && subscriptionCount() == 0) {
                return true;
            }

            return false;

        }

        //----------------------------------------------------------------------
        ///@brief   Give the sub class running in the thread context a chance to
        ///         abort the thread before we get started by overriding the    
        ///         initialize function.                                         
        ///                                                                     
        ///         Credit where credit is due, this concept was taken from     
        ///         Kirk Wilson's egrims work at ECR.                           
        ///                                                                     
        ///@return  true    = start the processing loop                         
        ///         false   = abort the thread                                  
        //----------------------------------------------------------------------
        virtual bool initialize() { return true; }

        //----------------------------------------------------------------------
        ///@brief  Give the sub class a chance to finalize while still operating
        ///        in the thread context.                                       
        //----------------------------------------------------------------------
        virtual void beforeEndThread() { }

        //----------------------------------------------------------------------
        ///@brief   Set the flag to terminate processing.                       
        //----------------------------------------------------------------------
        void stop(bool value = true) {m_Stop = value;}

        int publicationCount() const {return m_PublicationCount; }


    protected:
        //----------------------------------------------------------------------
        ///@brief   Give the derived class a chance to do something before      
        ///         calling endPublication.                                     
        ///@par     What's endPublication?                                      
        ///             If the subscriber is also a publisher, endPublication   
        ///             is used to let this object's subscriber's know that     
        ///             it is terminating (so that they shut down when they     
        ///             run out of data publishers).                            
        ///\n\n                                                                 
        ///             endPublication is declared virtual in the base class    
        ///             PublisherSubscriberBase.                                
        ///@result  true tells the thread to call endPublication.  false implies
        ///         that the derived class handled the functionality.           
        //----------------------------------------------------------------------
        virtual bool beforeEndPublication() { return true; }

        //----------------------------------------------------------------------
        ///@brief   Called after the endPublication to give the derived class   
        ///         a chance to clean up after endPublication is called.        
        ///@note    This is only called if beforeEndPublication returns true.   
        //----------------------------------------------------------------------
        virtual void afterEndPublication() {}

        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        void disconnectPublishers()
        {
            std::vector<boost::signals2::connection>::iterator c;
            for (c = m_Connections.begin(); c != m_Connections.end(); ++c) {
                c->disconnect();
            }
            
        }

        //----------------------------------------------------------------------
        ///@brief   Added item to the master queue for needing to be  executed  
        ///         next.                                                       
        //----------------------------------------------------------------------
        virtual void addToParentQueue(SubscriptionBase* item) override
        {
            m_Queue.push(item);
        }

    private:
        //----------------------------------------------------------------------
        //  If you need these, you're probably doing something wrong -- or      
        //  you've gone in a direction that I didn't anticipate.  Think about   
        //  it and if you really do need them, implement them (and it would be  
        //  nice if you explained your reasoning so that I could learn from it.)
        //----------------------------------------------------------------------
        Subscriber(const Subscriber& that);
        Subscriber& operator=(const Subscriber& that);

        //----------------------------------------------------------------------
        ///@brief The number of subscriptions that this object is subscribing to
        //----------------------------------------------------------------------
        volatile std::atomic<int32_t>   m_PublicationCount;

        //----------------------------------------------------------------------
        ///@brief Increases the number of subscriptions that this object is     
        ///       subscribing to; used to know when to terminate.               
        ///@note  See ramblings in SubscriptionBase and PublisherBase.          
        //----------------------------------------------------------------------
        void incrementPublicationCount(
            boost::signals2::connection& connection1
          , boost::signals2::connection& connection2
        )
        {
            ++m_PublicationCount;
            m_Connections.push_back(connection1);
            m_Connections.push_back(connection2);
        }

        //----------------------------------------------------------------------
        ///@brief   Flag used to indicate if we should terminate.               
        //----------------------------------------------------------------------
        volatile bool m_Stop;

        std::vector<boost::signals2::connection>    m_Connections;
                                                                                
        //----------------------------------------------------------------------
        ///@brief   Queue holding the subscription bases (in order) that they   
        ///         should be executed.                                         
        ///                                                                     
        ///         lib::mp::work::Queue is thread-safe.  This allows multiple  
        ///         publishers to insert items into the queue while the         
        ///         Subscription thread is pulling itesm out of the queue.      
        ///                                                                     
        ///         The queue is of type SubscriptionBase* in order to track    
        ///         which Subscription<TYPE> should be next in the order of     
        ///         execution.  The SubscriptionBase* allows us to effectively  
        ///         have a queue with multiple types (since the templated       
        ///         Subscription<TYPE> has a base of SubscriptionBase>.         
        //----------------------------------------------------------------------
        lib::mp::work::Queue<SubscriptionBase*>           m_Queue;

        enum class Stage 
        {
            stage0, stage1, stage2, stage3, stage4, stage5, stage6
        } m_Stage;

        //----------------------------------------------------------------------
        ///@brief   True if the endPublication should be executed; initialized  
        ///         in operator()().                                            
        //----------------------------------------------------------------------
        bool m_DoEndPublication;

        //----------------------------------------------------------------------
        ///@brief   Called after the all of the thread processing is complete;  
        ///         executed in the thread's context.                           
        ///                                                                     
        ///         What we would like to do is:                                
        ///         @code                                                       
        ///             if (beforeEndPublication()) {                           
        ///                 endPublication();                                   
        ///                 afterEndPublication();                              
        ///             }                                                       
        ///                                                                     
        ///             disconnectPublishers();                                 
        ///                                                                     
        ///             //------------------------------------------------------
        ///             //  Clear out the underlying input queue and free any   
        ///             //  governed publishers.                                
        ///             //------------------------------------------------------
        ///             m_Queue.abort();                                        
        ///                                                                     
        ///             beforeEndThread();                                      
        ///         @endcode                                                    
        ///                                                                     
        ///         The problem is that several of the above calls can          
        ///         potentially throw exceptions, but the remainder of the      
        ///         cleanUp routine still needs to be exectued -- and the       
        ///         exception thrown preserved and eventually re-thrown.        
        ///                                                                     
        ///         We're half-way assuming that there is a wrapper around      
        ///         this Subscriber that will convert the exception into a      
        ///         log message, but this class is agnostic to that potential.  
        ///                                                                     
        ///         In order to allow completion of the cleanUp routine and     
        ///         preserve the exception thrown, we make the cleanUp routine  
        ///         [potentially] recursive by having the exception handler     
        ///         call cleanUp.  The m_Stage variable tracks where in the     
        ///         cleanUp process we are at, and the m_DoEndPublication       
        ///         tracks if we should call endPublication.                    
        ///                                                                     
        ///         The m_Stage and m_DoEndPublication need to be member        
        ///         variables (versus function variables) in case an exception  
        ///         is thrown between the time that they are set and the time   
        ///         that they are needed since when they are needed may be in   
        ///         a different instance of cleanUp then when they are set.     
        ///                                                                     
        //----------------------------------------------------------------------
        void cleanUp()
        {
            try {
                if (m_Stage == Stage::stage0)
                {
                    m_Stage = Stage::stage1;
                    m_DoEndPublication = beforeEndPublication();
                }

                if (m_DoEndPublication && m_Stage == Stage::stage1)
                {
                    m_Stage = Stage::stage2;
                    endPublication();
                }

                if (m_DoEndPublication && m_Stage == Stage::stage2)
                {
                    m_Stage = Stage::stage3;
                    afterEndPublication();
                }

                if (m_Stage == Stage::stage3 || !m_DoEndPublication)
                {
                    m_Stage = Stage::stage4;
                    disconnectPublishers();
                }


                //--------------------------------------------------------------
                //  Clear out the underlying input queue and free any governed  
                //  publishers.                                                 
                //--------------------------------------------------------------
                if (m_Stage == Stage::stage4)
                {
                    m_Stage = Stage::stage5;
                    m_Queue.abort();
                }

                if (m_Stage == Stage::stage5)
                {
                    m_Stage = Stage::stage6;
                    beforeEndThread();
                }

            } catch (...) {
                cleanUp();
                throw;
            }

        }


}; // class Subscriber                                                          

}  // namespace msg //                                                          
}  // namespace lib //                                                          

#endif // #ifndef LIB_MSG_SUBSCRIBER_H                                          

