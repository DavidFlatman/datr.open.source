//------------------------------------------------------------------------------
///@file lib_ds_enum.h                                                          
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
//------------------------------------------------------------------------------

#ifndef LIB_DS_ENUM_H_FILE_GUARD
#define LIB_DS_ENUM_H_FILE_GUARD

#define SKIP_FOR_NOW

#include "lib_work_executebeforemain.h"
#include "lib_compiler_info.h"
#include "lib_string.h"                                 //----------------------

#include <algorithm>                                    //----------------------
#include <map>                                          // std::map             
#include <sstream>                                      // std::stringstream    
#include <stdint.h>                                     // uint64_t             
#include <stdlib.h>                                     //                      
#include <string>                                       // std::string          
                                                        //----------------------

//------------------------------------------------------------------------------
//  Windows has an issue with thread local.  The following makes this thread    
//  unsafe under Windows.  Current DATR usage would make this "okay", but far   
//  from ideal.  If Visual Studio starts to support the thread_local, this must 
//  be removed.                                                                 
//------------------------------------------------------------------------------
#ifndef IS_WINDOWS
#include <thread>                                       // thread_local         
#define thread_local static
#endif

//------------------------------------------------------------------------------
///@brief   Macro for defining smart enumerated types.                          
///                                                                             
///         This macro will define an enumerated type that has built in         
///         toString and fromString fucntionality.                              
///                                                                             
///@par Thread Safety:  special                                                 
///         Except for the @e setName method, all methods are reentrant         
///         (thread safe).  The setName method can cause a race condition.      
///                                                                             
///@par Macro?!?                                                                
///         Why a macro (versus a template)?  There are certain tricks that     
///         are being done (e.g., #__VA_ARGS__) that cannot be done if a        
///         template was employed.                                              
///                                                                             
///@par Expected Usage:  General                                                
///         @code                                                               
///             ENUM(traffic_light_t, RED = 1, YELLOW, GREEN);                  
///             traffic_light_t light(taffic_light_t::RED);                     
///             std::cout << "Light = " << light.toString() << std::endl;       
///                                                                             
///             for (auto l : traffic_light_t::values())                        
///             {                                                               
///                 std::cout << l << " is " << l.toString() << std::endl;      
///                 std::cout << l << " is " << (std::string) l << std::endl;   
///             }                                                               
///         @endcode                                                            
///                                                                             
///         With the DATR localization, we would expect the following output:   
///             1   is red                                                      
///             1   is red                                                      
///             2   is yellow                                                   
///             2   is yellow                                                   
///             3   is green                                                    
///             3   is green                                                    
///                                                                             
///@par Expected Usage:  switch statements                                      
///         If we want a switch statement to generate a warning (-Wall) about   
///         an unhandled condition, the value method needs to be used.  E.g.,   
///             @code                                                           
///                 ENUM(traffic_light_t, RED = 1, YELLOW, GREEN);              
///                 // ... snip ...                                             
///                                                                             
///                 void my_function(traffic_light_t x)                         
///                 {                                                           
///                     switch (x.value())                                      
///                     {                                                       
///                         case traffic_light_t::RED:                          
///                             // handle red                                   
///                             break;                                          
///                         case traffic_light_t::GREEN:                        
///                             // handle green                                 
///                             break;                                          
///                     }                                                       
///                 };                                                          
///             @endcode                                                        
///         When compiled with -Wall, the above code will generate a compiler   
///         warning that YELLOW is an unhandled condition.  If the switch       
///         was:                                                                
///             @code                                                           
///                 switch(x)                                                   
///                 {                                                           
///                     case traffic_light_t::RED:                              
///                         // handle red                                       
///                         break;                                              
///                     case traffic_light_t::GREEN:                            
///                         // handle green                                     
///                         break;                                              
///                 }                                                           
///             @endcode                                                        
///         Then no warning would be generated.                                 
///                                                                             
///@par Accessing Values                                                        
///         To code for the values, the "enumerated" type defined by the ENUM   
///         macro is very analogous to a C++ 11 enumerated class type.  In      
///         order to get to the individual values, they must be referenced by   
///         the type name.  From the example above:                             
///         @code                                                               
///             someFunction(traffic_light::RED);                               
///         @endcode                                                            
///                                                                             
///@par Local vs Global vs Internal                                             
///         Methods labeled "local" hold thread local values.  Methods labeled  
///         global are static variables and are shared across threads.  When    
///         possible, local is used.  Global is used when mappings have been    
///         changed.  The methods labeled "internal" decide if local or global  
///         is required.                                                        
///                                                                             
///@limitations                                                                 
///         -#  Two enumerated names with the same value will effectively       
///             suppress the earlier name(s).                                   
///         -#  The enumerated type must be declared outside of a method or     
///             function.  Under the covers (see code below) a new class is     
///             declared and defined for the enumerated type.  With C++ 11      
///             you cannot declare a class within either a method or a function.
///                                                                             
///@par setName                                                                 
///         Let's be honest, the setName feature may be a bad idea.  But, the   
///         coding conventions might require something like "TL_RED" and the    
///         user prefers to see "Red".                                          
///\n\n                                                                         
///         If the setName method is used, the following concerns should be     
///         addressed:                                                          
///             -#  @e setName is not reentrant (not thread-safe).              
///             -#  Performing a setName will break existing value-string       
///                 mappings.  For instance, the following code will fail:      
///                 @code                                                       
///                     ENUM(MyEnum, ZERO = 0, ONE, TWO, THREE, FOUR);          
///                     std::string s_zero(MyEnum::ZERO.toString());            
///                     MyEnum::setName(MyEnum::ZERO, "Zero");                  
///                                                                             
///                     //------------------------------------------------------
///                     //  The following code line will fail due to s_zero     
///                     //  holding "ZERO" and MyEnum now expecting "Zero".     
///                     //------------------------------------------------------
///                     MyEnum zero(MyEnum::fromString(s_Zero);                 
///                 @endcode                                                    
///             -#  All code has room for abuse, consider the following:        
///                 @code                                                       
///                     ENUM(MyEnum, ZERO = 0, ONE, TWO, THREE, FOUR);          
///                     MyEnum::setName(MyEnum::ONE, "ZERO");                   
///                 @endcode                                                    
///                                                                             
///@warning The DATR localization effectively makes the enumerated values       
///         lowercase for conversion purposes.  Consider:                       
///         @code                                                               
///             ENUM (red_t, red=1, Red=2, RED=3);                              
///         @endcode                                                            
///                                                                             
///         The DATR locationalization will make all of the strings "red".      
///                                                                             
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2020-02-24  DHF     Visual Studio has thread local issues.          
///                                                                             
///@version 2019-11-12  MNT     Resolved merge conflict.                        
///                                                                             
///@version 2019-10-08  DHF/MNT Added value() method.                           
///                                                                             
///@version 2019-08-15  DHF     Added inline throughout the ENUM class.         
///                                                                             
///@version 2019-07-24  DHF     Added constructor to allow set names.           
///                             Added ENUM_SET_NAME macro.                      
///                                                                             
///@version 2019-07-23  DHF     Added method level comments.                    
///                             Added operator==.                               
///                                                                             
///@version 2019-07-23  DHF     Added DATR localization of string names.        
///                                                                             
///@version 2019-07-19  DHF     Merged default constructor with uint64_t        
///                             constructor.                                    
///                                                                             
///@version 2019-01-17  DHF     File creation based on concepts taken from      
///                             the web.                                        
///                                                                             
//------------------------------------------------------------------------------
#define ENUM(NAME, ...)                                                     \
    class NAME                                                              \
    {                                                                       \
        public:                                                             \
            inline NAME(uint64_t x = 0) : m_Value(x) { }                    \
            /*-----------------------------------------------------------*/ \
            /*!@brief This really should only be used via ENUM_SET_NAME. */ \
            /*-----------------------------------------------------------*/ \
            inline NAME(uint64_t v, const std::string& n) : m_Value(v)      \
            {                                                               \
                setName(v, n);                                              \
            }                                                               \
                                                                            \
            inline explicit NAME(const std::string& s)                      \
              : m_Value(fromString(s)) {}                                   \
                                                                            \
            inline ~NAME() = default;                                       \
                                                                            \
            inline NAME& operator=(const NAME& x) = default;                \
            inline NAME& operator=(const std::string& x)                    \
            {                                                               \
                *this = NAME(x);                                            \
                return *this;                                               \
            }                                                               \
                                                                            \
            enum ENUM_NAME {__VA_ARGS__};                                   \
                                                                            \
            inline ENUM_NAME value() const { return ENUM_NAME(m_Value); }   \
                                                                            \
            inline operator uint64_t() const { return m_Value; }            \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief Return a std::string holding the name of the value.*/ \
            /*-----------------------------------------------------------*/ \
            inline std::string toString(void) const                         \
            {                                                               \
                return toString(m_Value);                                   \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief Return a std::string holding name of given value.  */ \
            /*-----------------------------------------------------------*/ \
            inline static std::string toString(uint64_t value)              \
            {                                                               \
                auto i = valueName().find(value);                           \
                if (i == valueName().end())                                 \
                {                                                           \
                    throw std::range_error("invalid " #NAME " value");      \
                }                                                           \
                return i->second;                                           \
            }                                                               \
            /*-----------------------------------------------------------*/ \
            /*!@brief Return the name of this value.                     */ \
            /*-----------------------------------------------------------*/ \
            inline explicit operator std::string() const                    \
            {                                                               \
                return toString(m_Value);                                   \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return true if this variable is less than that.  */ \
            /*-----------------------------------------------------------*/ \
            inline bool operator<(const NAME n) const                       \
            {                                                               \
                return m_Value < n.m_Value;                                 \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Convert from a string to an enum element.        */ \
            /*-----------------------------------------------------------*/ \
            inline static NAME fromString(const std::string& s)             \
            {                                                               \
                for (auto n : values())                                     \
                {                                                           \
                    if (toString(n) == s) return n;                         \
                }                                                           \
                throw std::range_error(s + " not a member of " + #NAME);    \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return all of the enumerated values.             */ \
            /*-----------------------------------------------------------*/ \
            inline static std::vector<NAME> const& values()                 \
            {                                                               \
                return internalValues();                                    \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return all of the names of all of values.        */ \
            /*!@warning Array subscript may not be the enumerated value. */ \
            /*-----------------------------------------------------------*/ \
            inline static std::vector<std::string> const& names()           \
            {                                                               \
                return internalNames();                                     \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Change the name for the given value.             */ \
            /*!@warning This code is not reentrant.                      */ \
            /*-----------------------------------------------------------*/ \
            inline static void setName(                                     \
                uint64_t value                                              \
              , const std::string& newName                                  \
            )                                                               \
            {                                                               \
                globalValueName()[value] = newName;                         \
                internalNames().clear();                                    \
                internalValues().clear();                                   \
            }                                                               \
                                                                            \
                                                                            \
        private:                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Convert the string value to the local standard.  */ \
            /*-----------------------------------------------------------*/ \
            inline static std::string localize(const std::string& s)        \
            {                                                               \
                std::string result(s);                                      \
                for (auto& c : result)                                      \
                {                                                           \
                    if ('A' <= c && c <= 'Z') c = c ^ 32;                   \
                    else if (c == '_') c = '-';                             \
                }                                                           \
                return result;                                              \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return the thread local value/name map.          */ \
            /*-----------------------------------------------------------*/ \
            inline static std::map<uint64_t, std::string>& localValueName() \
            {                                                               \
                thread_local std::map<uint64_t, std::string> value_name;    \
                if (value_name.empty())                                     \
                {                                                           \
                    std::string va_args(#__VA_ARGS__);                      \
                    std::replace(va_args.begin(), va_args.end(), '=', ' '); \
                                                                            \
                    std::stringstream stream(va_args);                      \
                    std::vector<std::string> strings;                       \
                    for (std::string s; getline(stream, s, ','); )          \
                    {                                                       \
                        strings.push_back(localize(s));                     \
                    }                                                       \
                                                                            \
                    uint64_t i(0);                                          \
                    for (auto& s : strings)                                 \
                    {                                                       \
                        std::string name;                                   \
                        std::string value;                                  \
                                                                            \
                        std::stringstream stream(s);                        \
                        stream >> name >> value;                            \
                                                                            \
                        if (!value.empty()) i = lib::toUnsigned(value.c_str());        \
                                                                            \
                        value_name[i++] = name;                             \
                    }                                                       \
                }                                                           \
                return value_name;                                          \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return the application global value/name map.    */ \
            /*-----------------------------------------------------------*/ \
            inline static std::map<uint64_t, std::string>& globalValueName()\
            {                                                               \
                /*-------------------------------------------------------*/ \
                /* Note:    Application global values are defined static,*/ \
                /*          not thread_local.                            */ \
                /*-------------------------------------------------------*/ \
                static std::map<uint64_t, std::string> value_name;          \
                if (value_name.empty()) value_name = localValueName();      \
                return value_name;                                          \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief Return the value/name map that should be used.     */ \
            /*-----------------------------------------------------------*/ \
            inline static std::map<uint64_t, std::string>& valueName()      \
            {                                                               \
                return globalValueName().empty()                            \
                    ?  localValueName()                                     \
                    :  globalValueName()                                    \
                ;                                                           \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return the names of all of the values.           */ \
            /*-----------------------------------------------------------*/ \
            inline static std::vector<std::string>& internalNames()         \
            {                                                               \
                thread_local std::vector<std::string> names;                \
                if (names.empty())                                          \
                {                                                           \
                    for (auto i : internalValues())                         \
                    {                                                       \
                        names.push_back(toString(i));                       \
                    }                                                       \
                }                                                           \
                return names;                                               \
            }                                                               \
                                                                            \
            /*-----------------------------------------------------------*/ \
            /*!@brief   Return the values valid enum values.             */ \
            /*-----------------------------------------------------------*/ \
            inline static std::vector<NAME>& internalValues()               \
            {                                                               \
                thread_local std::vector<NAME> values;                      \
                if (values.empty())                                         \
                {                                                           \
                    for (auto i : valueName()) values.push_back(i.first);   \
                }                                                           \
                return values;                                              \
            }                                                               \
                                                                            \
            uint64_t m_Value;                                               \
                                                                            \
    };


//------------------------------------------------------------------------------
///@brief   Set the string name of an enumerated type at global initialization  
///         time.                                                               
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///         //------------------------------------------------------------------
///         //  If the default ENUM casing behavior is used:                    
///         //------------------------------------------------------------------
///         ENUM(E, GREEN, YELLOW, RED);                                        
///         void my_function()                                                  
///         {                                                                   
///             assert(E::toString(E::GREEN)  == "green");                      
///             assert(E::toString(E::YELLOW) == "yellow");                     
///             assert(E::toString(E::RED)    == "red");                        
///         }                                                                   
///                                                                             
///         //------------------------------------------------------------------
///         //  If ENUM_SET_NAME is used:                                       
///         //------------------------------------------------------------------
///         ENUM(N, ONE, TWO, THREE);                                           
///         ENUM_SET_NAME(N, ONE,   "uno");                                     
///         ENUM_SET_NAME(N, TWO,   "dose");                                    
///         ENUM_SET_NAME(N, THREE, "tres");                                    
///                                                                             
///         void my_function()                                                  
///         {                                                                   
///             assert(N::toString(N::ONE)   == "uno");                         
///             assert(N::toString(N::TWO)   == "dose");                        
///             assert(N::toString(N::THREE) == "tres");                        
///         }                                                                   
///     @endcode                                                                
///                                                                             
///@par Why EXECUTE_BEFORE_MAIN                                                 
///         -#  There are no order dependencies on other code being executed    
///             first.                                                          
///         -#  Even if the routine is called multiples times (once per         
///             compilation module) there is no harm created other than         
///             a very small performance hit at application start up.           
///         -#  Combine the prior comments and this can be safely placed in     
///             include files.                                                  
///                                                                             
//------------------------------------------------------------------------------
#define ENUM_SET_NAME(NAME, V, S)                                              \
    EXECUTE_BEFORE_MAIN(NAME::setName(NAME::V, S));

#endif // #ifndef LIB_DS_ENUM_H_FILE_GUARD

