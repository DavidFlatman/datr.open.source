//------------------------------------------------------------------------------
///@file lib_atomic.h                                                           
///@par     Classification:  UNCLASSIFIED                                       
///                                                                             
///@brief   Macros for dealing with differences in C++/Boost's atomic functions.
///                                                                             
///@note    Once C++ x11 is sufficiently universally implemented this file      
///         should go away in favor of C++ x11's atomic methodology.            
///                                                                             
///@version 2014-06-18  DHF     Modified for use with Boost 1.49                
///                                                                             
///@version 2014-01-22  DHF     Modified atomic's version of ATOMIC_GET32 to    
///                             be compatible with the standard.                
///                                                                             
///@version 2013-10-29  DHF     Added ATOMIC_SET32                              
///                             Added ATOMIC_GET32                              
//------------------------------------------------------------------------------

#ifndef LIB_ATOMIC_H_FILE_GUARD
#define LIB_ATOMIC_H_FILE_GUARD

#include <boost/version.hpp>
#include <stdint.h>

//------------------------------------------------------------------------------
//  Boost 1.47 or before                                                        
//------------------------------------------------------------------------------
#if BOOST_VERSION <= 104700
#include <boost/interprocess/detail/atomic.hpp>
#define ATOMIC_INT32_T  boost::uint32_t
#define ATOMIC_INCREMENT(x) boost::interprocess::detail::atomic_inc32(&x)
#define ATOMIC_DECREMENT(x) boost::interprocess::detail::atomic_dec32(&x)
#define ATOMIC_SET32(x,y)   boost::interprocess::detail::atomic_write32(&x, y)
#define ATOMIC_GET32(x)     boost::interprocess::detail::atomic_read32(&x)

//------------------------------------------------------------------------------
//  Boost 1.48 or 1.49                                                          
//------------------------------------------------------------------------------
#elif BOOST_VERSION <= 104900
#include <boost/interprocess/detail/atomic.hpp>
#define ATOMIC_INT32_T  boost::uint32_t
#define ATOMIC_INCREMENT(x) boost::interprocess::ipcdetail::atomic_inc32(&x)
#define ATOMIC_DECREMENT(x) boost::interprocess::ipcdetail::atomic_dec32(&x)
#define ATOMIC_SET32(x,y)   boost::interprocess::ipcdetail::atomic_write32(&x, y)
#define ATOMIC_GET32(x)     boost::interprocess::ipcdetail::atomic_read32(&x)

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#else
#include <atomic>
#define ATOMIC_INT32_T  std::atomic<int32_t>
#define ATOMIC_INCREMENT(x) ++x
#define ATOMIC_DECREMENT(x) --x
#define ATOMIC_SET32(x,y)   x = y
#define ATOMIC_GET32(x)     ((int32_t)x)

#endif

#endif // #ifndef LIB_ATOMIC_H_FILE_GUARD
