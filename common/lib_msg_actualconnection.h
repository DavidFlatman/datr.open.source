//------------------------------------------------------------------------------
///@file lib_msg_actualconnection.h                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_MSG_ACTUALCONNECTION_H
#define LIB_MSG_ACTUALCONNECTION_H

#include "lib_ds_null.h"
#include "lib_msg_publication.h"
#include "lib_msg_subscription.h"

#include <stdint.h>

namespace lib {
namespace msg {

//------------------------------------------------------------------------------
///@brief   Ensure that neither the SUB_TYPE nor the PUB_TYPE are defaults and  
///         make connection.                                                    
///                                                                             
///         The default parameter types to Publisher and Subscriber are found   
///         in lib_msg_null.h and are based on lib::msg::NULL_BASE.             
///                                                                             
///@note    This is another template not expected to be called except by the    
///         publisher code.                                                     
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2016-11-01  DHF     lib::msg::NULL* moved to lib::ds::NULL*         
///                                                                             
///@version 2015-09-14  DHF     Changed return type from int to uint32_t.       
///                                                                             
//------------------------------------------------------------------------------
template<typename SUB_TYPE, typename PUB_TYPE>
uint32_t actualConnection(Subscription<SUB_TYPE>* s, Publication<PUB_TYPE>* p)
{
    uint32_t result(0);
    if (
        (!boost::is_base_and_derived<ds::NULL_BASE, SUB_TYPE>::value)
     && (!boost::is_base_and_derived<ds::NULL_BASE, PUB_TYPE>::value)
    )
    {
        p->connect(*s);
        result = 1;
    }
    return result;
}


} // namespace msg
} // namespace lib

#endif // #ifndef LIB_MSG_ACTUALCONNECTION_H
