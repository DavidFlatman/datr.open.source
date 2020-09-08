//------------------------------------------------------------------------------
///                                                                             
///@file lib_msg_publisherhelper.h                                              
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Holds helper routines for the lib_msg_publisher.h file.             
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-02-11  KCG     Changed boost::shared_ptr to lib::ds::shared_ptr
///                                                                             
///@version 2015-08-30  DHF     File creation.                                  
///                                                                             
//------------------------------------------------------------------------------
namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///@brief   Call MACRO for matrix cross between all Publication types and       
///         all Subscription types.                                             
//------------------------------------------------------------------------------
#define PUB_HELPER_ALL_PUB(MACRO, SAME_ONLY, D_ONLY)                       \
    PUB_HELPER_ALL_SUB(MACRO, TYPE0, 0, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE1, 1, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE2, 2, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE3, 3, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE4, 4, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE5, 5, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE6, 6, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE7, 7, SAME_ONLY, D_ONLY);                \
    PUB_HELPER_ALL_SUB(MACRO, TYPE8, 8, SAME_ONLY, D_ONLY);

//------------------------------------------------------------------------------
///@brief   Call the given MACRO with the given P_TYPE for each Subscription    
///         type.                                                               
//------------------------------------------------------------------------------
#define PUB_HELPER_ALL_SUB(MACRO, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY)      \
    MACRO(S0, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S1, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S2, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S3, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S4, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S5, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S6, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S7, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);              \
    MACRO(S8, P_TYPE, P_COUNT, SAME_ONLY, D_ONLY);
    

//------------------------------------------------------------------------------
///@macro PUB_HELPER_CONNECT_THEM                                               
///@brief   Make a connection between Subscription and Publication if all is    
///         compatible.                                                         
///                                                                             
///         This macro is expected to only be called by lib::msg::Publisher.    
///                                                                             
///@param S_TYPE    The type the Subscription subscribes to.                    
///@param P_TYPE    The type the Publication publishes.                         
///@param P_COUNT   The number of the Publication parameter, used to track if   
///                 the parameter is already subscribed to or not.              
///@param S_ONLY    True implies that only if S_TYPE and P_TYPE are the same    
///                 type, a connection is made.  False implies that a base      
///                 derived relationship can be connected to.                   
///@param D_ONLY    True if only derived classes (versus convertible) should be 
///                 connected to.  False means that convertible is also         
///                 acceptable.                                                 
///                                                                             
//------------------------------------------------------------------------------
#define PUB_HELPER_CONNECT_THEM(S_TYPE, P_TYPE, P_COUNT, S_ONLY, D_ONLY)    \
{                                                                           \
    Subscription<S_TYPE>* sub(dynamic_cast<Subscription<S_TYPE>*>(&s));     \
    Publication<P_TYPE>*  pub(dynamic_cast<Publication<P_TYPE>*>(this));    \
    if (                                                                    \
        pubUsed[P_COUNT] == 0                                               \
     && PublisherConnectionHelper<                                          \
            boost::is_same<S_TYPE, P_TYPE>::value                           \
          , boost::is_base_and_derived<S_TYPE, P_TYPE>::value               \
          , boost::is_convertible<lib::ds::shared_ptr<const P_TYPE>, lib::ds::shared_ptr<const S_TYPE> >::value                    \
          , S_TYPE                                                          \
          , P_TYPE                                                          \
        >::connect(sub, pub, S_ONLY, D_ONLY)                                \
    )                                                                       \
    {                                                                       \
        ++result;                                                           \
        ++pubUsed[P_COUNT];                                                 \
    }                                                                       \
}

#define PUB_HELPER_CONNECT_ALL                                              \
        PUB_HELPER_ALL_PUB(PUB_HELPER_CONNECT_THEM, true, true);            \
        PUB_HELPER_ALL_PUB(PUB_HELPER_CONNECT_THEM, false, true);           \
        PUB_HELPER_ALL_PUB(PUB_HELPER_CONNECT_THEM, false, false);
    

} // namespace msg
} // namespace lib 
