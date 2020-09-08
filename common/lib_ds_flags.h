//------------------------------------------------------------------------------
///                                                                             
///@file lib_ds_flags.h                                                         
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_DS_FLAGS_H_FILE_GUARD
#define LIB_DS_FLAGS_H_FILE_GUARD

#include <stdint.h>

namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///@par Class:  Flag                                                            
///@biref   A single element of Flags.                                          
///@version 2018-08-03  DHF     Changed m_Value from int to uint64_t.           
//------------------------------------------------------------------------------
template <typename INTEGER>
class Flag
{
    public:
        inline Flag(INTEGER v) : m_Value(v) {}
        inline operator INTEGER() const { return m_Value; }
    private:
        INTEGER m_Value;

}; // class Flag

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Flags                                                            
///                                                                             
///@brief   Make handling flags defined as enumerated types easier to handle.   
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///         @code                                                               
///             class Message {                                                 
///                 public:                                                     
///                     enum PARTS_ENUM {                                       
///                         TIME_STAMP  = 0x01                                  
///                       , MNEMONIC    = 0x02                                  
///                       , FACILITY    = 0x04                                  
///                       , SEVERITY    = 0x08                                  
///                       , ID          = 0x10                                  
///                       , TEXT        = 0x20                                  
///                       , ALL         = 0xff                                  
///                     };                                                      
///                     typedef lib::Flags<PARTS_ENUM> PARTS;                   
///                                                                             
///                     void write(PARTS part = ALL);                           
///             };                                                              
///                                                                             
///             //  snip                                                        
///                                                                             
///             Message msg;                                                    
///             populateMessage(msg);                                           
///             msg.write(Message::TIME_STAMP | Message::TEXT);                 
///                                                                             
///         @endcode                                                            
///         Without using lib::Flags, the above code would have to look like:   
///         @code                                                               
///             Message msg;                                                    
///             populateMessage(msg);                                           
///             msg.write(                                                      
///                 Message::PARTS_ENUM(Message::TIME_STAMP | Message::TEXT)    
///             );                                                              
///         @endcode                                                            
///                                                                             
///@version 2014-03-25  DHF     Changed for (;;); to for (;;) { } for clang.    
///                                                                             
///@version 2011-03-17  DHF         File creation                               
///                                                                             
//------------------------------------------------------------------------------
template <typename ENUM_TYPE, typename INTEGER = uint64_t>
class Flags
{
    public:
        Flags(const Flags& that) : m_V(that.m_V) {}
        Flags(ENUM_TYPE e) : m_V((INTEGER) e) {}
        Flags() : m_V(0) {}
        Flags(Flag<INTEGER> f) : m_V(f) {}
        Flags(INTEGER f) : m_V(f) {}

        operator INTEGER() const { return m_V; }

        Flags& operator=(const Flags& that) { m_V = that.m_V;   return *this;}
        Flags& operator&=(INTEGER  mask)    { m_V &= mask;      return *this;}
        Flags& operator|=(Flags that)       { m_V |= that.m_V;  return *this;}
        Flags& operator|=(ENUM_TYPE e)      { m_V |= e;         return *this;}
        Flags& operator^=(Flags that)       { m_V ^= that.m_V;  return *this;}
        Flags& operator^=(ENUM_TYPE e)      { m_V ^= e;         return *this;}

        ENUM_TYPE operator&(INTEGER  m)    const {return ENUM_TYPE(m_V & m);}
        ENUM_TYPE operator&(ENUM_TYPE e)   const {return ENUM_TYPE(m_V & e);}

        Flags operator|(const Flags& f)const { Flags r(m_V | f.m_V);  return r;}
        Flags operator|(ENUM_TYPE e)   const { Flags r(m_V | e);      return r;}

        Flags operator^(const Flags& f)const { Flags r(m_V ^ f.m_V);  return r;}
        Flags operator^(ENUM_TYPE e)   const { Flags r(m_V ^ e);      return r;}

        Flags operator~() const { Flags r(~m_V); return r;}
        bool  operator!() const { return !m_V;}

        bool test(ENUM_TYPE e) const 
            { return (m_V & e) == e && (e != 0 || m_V == int(e)); }
        bool test(INTEGER e) const 
            { return (m_V & e) == e && (e != 0 || m_V == INTEGER(e)); }

        static INTEGER toIndex(ENUM_TYPE f)
            { INTEGER i = 0; for (INTEGER v(f); v; v >>= 1, ++i) {}  return i; }

        INTEGER toIndex()
            { INTEGER i = 0; for (INTEGER v(m_V); v; v >>= 1, ++i) {}  return i; }

    protected:

    private:
        INTEGER m_V;

}; // class Flags //

} // namespace ds
} // namespace lib

#endif // #ifndef LIB_DS_FLAGS_H_FILE_GUARD

