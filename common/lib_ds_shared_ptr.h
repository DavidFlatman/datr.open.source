//------------------------------------------------------------------------------
///@file lib_ds_shared_ptr.h                                                    
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_DS_SHARED_PTR_H_FILE_GUARD
#define LIB_DS_SHARED_PTR_H_FILE_GUARD
#include <memory>
#include <stddef.h>              // std::nullptr_t

//#include <boost/shared_ptr.hpp>//temp will be removed when std:: is inserted
namespace lib {
namespace ds {
//------------------------------------------------------------------------------
///                                                                             
///@par Class: shared_ptr                                                       
///                                                                             
///@brief   A class created to replace <boost/shared_ptr.hpp> in order to give  
///         control over which shared pointer is being used in the code at a    
///         single location                                                     
///                                                                             
///@par Thread Safety: Special                                                  
///         The pointer itself is thread safe.  However, that does not mean     
///         the object pointed to is thread safe.  It means that the pointer    
///         reference count can be incremented and decrement across threads,    
///         but not that the object pointed to can be manipulated across        
///         threads.                                                            
///                                                                             
///         @code                                                               
///             void function(shared_ptr<std::string> p)                        
///             {                                                               
///             }                                                               
///             void thread_main_one(shared_ptr<std::string>& x)                
///             {                                                               
///             }                                                               
///             void thread_main_two(shared_ptr<std::string>& x)                
///             {                                                               
///                 //----------------------------------------------------------
///                 //  The call to function affects x's reference count.       
///                 //  This is okay.                                           
///                 //----------------------------------------------------------
///                 function(x);                                                
///                                                                             
///                 //----------------------------------------------------------
///                 //  Whether the call to replace is thread-safe or not is    
///                 //  independent of shared_ptr and is on std::string.        
///                 //----------------------------------------------------------
///                 x->replace('\t', ' ');                                      
///             }                                                               
///         @endcode                                                            
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2020-03-05  DHF     Added dynamic_pointer_cast                      
///                             Added const_pointer_cast                        
///                             Added std::shared_ptr constructors              
///                                                                             
///@version 2020-01-28  KCG     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

template <typename TYPE>
class shared_ptr : public std::shared_ptr<TYPE>
{
    public:
        //======================================================================
        //  Copied from the C++ std::shared_ptr.                                
        //======================================================================
        constexpr shared_ptr() noexcept 
            : std::shared_ptr<TYPE>() 
        { }

        shared_ptr(const shared_ptr&) noexcept = default;

        template<typename TYPE1>
        explicit shared_ptr(TYPE1* p) 
            : std::shared_ptr<TYPE>(p) 
        { }

        template<typename TYPE1, typename _Deleter>
        shared_ptr(TYPE1* p, _Deleter d) 
            : std::shared_ptr<TYPE>(p, d) 
        { }

        template<typename _Deleter>
        shared_ptr(nullptr_t p, _Deleter d)
            : std::shared_ptr<TYPE>(p, d) 
        { }

        template<typename TYPE1, typename _Deleter, typename _Alloc>
        shared_ptr(TYPE1* p, _Deleter d, _Alloc __a)
            : std::shared_ptr<TYPE>(p, d, std::move(__a)) 
        { }

        template<typename _Deleter, typename _Alloc>
        shared_ptr(nullptr_t p, _Deleter d, _Alloc __a)
            : std::shared_ptr<TYPE>(p, d, std::move(__a)) 
        { }

        template<typename TYPE1>
        shared_ptr(const shared_ptr<TYPE1>& r, TYPE* p) noexcept
            : std::shared_ptr<TYPE>(r, p) 
        { }

        template
        <
            typename TYPE1
          , typename = typename std::enable_if
                <std::is_convertible <TYPE1*, TYPE*>::value>::type
        >
        shared_ptr(const shared_ptr<TYPE1>& r) noexcept
            : std::shared_ptr<TYPE>(r) 
        { }

        shared_ptr(shared_ptr&& r) noexcept
            : std::shared_ptr<TYPE>(std::move(r)) 
        { }

        template
        <
            typename TYPE1
          , typename = typename std::enable_if
                <std::is_convertible<TYPE1*, TYPE*>::value>::type
        >
        shared_ptr(shared_ptr<TYPE1>&& r) noexcept
            : std::shared_ptr<TYPE>(std::move(r)) 
        { }

        template <typename TYPE1>
        explicit shared_ptr(const std::weak_ptr<TYPE1>& r)
            : std::shared_ptr<TYPE>(r) 
        { }


        template<typename TYPE1, typename _Del>
        shared_ptr(std::unique_ptr<TYPE1, _Del>&& r)
            : std::shared_ptr<TYPE>(std::move(r)) 
        { }

        constexpr shared_ptr(nullptr_t p) noexcept
            : std::shared_ptr<TYPE>(p) 
        { }

        shared_ptr& operator=(const shared_ptr&) noexcept = default;

        template<typename TYPE1>
        shared_ptr& operator=(const shared_ptr<TYPE1>& r) noexcept
        {
            this->std::shared_ptr<TYPE>::operator=(r);
            return *this;
        }

        shared_ptr& operator=(shared_ptr&& r) noexcept
        {
            this->std::shared_ptr<TYPE>::operator=(std::move(r));
            return *this;
        }

        template<class TYPE1> 
        shared_ptr& operator=(shared_ptr<TYPE1>&& r) noexcept
        {
            this->std::shared_ptr<TYPE>::operator=(std::move(r));
            return *this;
        }

        template<typename TYPE1, typename _Del>
        shared_ptr& operator=(std::unique_ptr<TYPE1, _Del>&& r)
        {
            this->std::shared_ptr<TYPE>::operator=(std::move(r));
            return *this;
        }

        //======================================================================
        //  DATR extensions                                                     
        //======================================================================
        shared_ptr(const std::shared_ptr<TYPE>& t) noexcept 
            : std::shared_ptr<TYPE>(t)
        {
        }

        //----------------------------------------------------------------------
        ///@brief   Allow for testing the pointer in an if statement.           
        ///                                                                     
        ///@par Expected Usage                                                  
        ///         @code                                                       
        ///             shared_ptr<T> p;                                        
        ///             get(p);                                                 
        ///             int count(0);                                           
        ///                                                                     
        ///             if (p)  { ++count; }                                    
        ///             if (p.get() != nullptr) { assert(count == 1); }         
        ///                                                                     
        ///         @endcode                                                    
        //----------------------------------------------------------------------
        inline operator bool() const 
        { 
            return std::shared_ptr<TYPE>::get() != nullptr; 
        }


    protected:


    private:


}; // class shared_ptr : public std::shared_ptr<T>



template<typename TYPE, typename TYPE1>
inline shared_ptr<TYPE> dynamic_pointer_cast(
    const shared_ptr<TYPE1>& r
) noexcept
{
    if (TYPE* p = dynamic_cast<TYPE*>(r.get()))
    {
        return shared_ptr<TYPE>(r, p);
    }
    return shared_ptr<TYPE>();
}

template<typename TYPE, typename TYPE1>
inline shared_ptr<TYPE> const_pointer_cast(
    const shared_ptr<TYPE1>& r
) noexcept
{ 
    return shared_ptr<TYPE>(r, const_cast<TYPE*>(r.get())); 
}



} // namespace ds

template <typename TYPE>
lib::ds::shared_ptr<TYPE>  new_shared(lib::ds::shared_ptr<TYPE>& p)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE);
    return p;
}

template <typename TYPE, typename T1>
lib::ds::shared_ptr<TYPE> new_shared(lib::ds::shared_ptr<TYPE>& p, T1&& p1)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE(p1));
    return p;
}

template <typename TYPE, typename T1, typename T2>
lib::ds::shared_ptr<TYPE> new_shared(lib::ds::shared_ptr<TYPE>& p, T1&& p1, T2&& p2)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2));
    return p;
}

template <typename TYPE, typename T1, typename T2, typename T3>
lib::ds::shared_ptr<TYPE> new_shared(lib::ds::shared_ptr<TYPE>& p, T1&& p1, T2&& p2, T3&& p3)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3));
    return p;
}

template <typename TYPE, typename T1, typename T2, typename T3, typename T4>
lib::ds::shared_ptr<TYPE> new_shared(lib::ds::shared_ptr<TYPE>& p, T1&& p1, T2&& p2, T3&& p3, T4&& p4)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3, p4));
    return p;
}

template <typename TYPE, typename T1, typename T2, typename T3, typename T4, typename T5>
lib::ds::shared_ptr<TYPE> new_shared(lib::ds::shared_ptr<TYPE>& p, T1&& p1, T2&& p2, T3&& p3, T4&& p4, T5&& p5)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3, p4, p5));
    return p;
}

template <typename TYPE, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
lib::ds::shared_ptr<TYPE> new_shared(lib::ds::shared_ptr<TYPE>& p, T1&& p1, T2&& p2, T3&& p3, T4&& p4, T5&& p5, T6&& p6)
{
    p = lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3, p4, p5, p6));
    return p;
}


template <typename TYPE>
lib::ds::shared_ptr<TYPE> make_shared()
{
    return lib::ds::shared_ptr<TYPE>(new TYPE);
}

template <typename TYPE, typename T1>
lib::ds::shared_ptr<TYPE> make_shared(T1&& p1)
{
    return lib::ds::shared_ptr<TYPE>(new TYPE(p1));
}

template <typename TYPE, typename T1, typename T2>
lib::ds::shared_ptr<TYPE> make_shared(T1&& p1 , T2&& p2)
{
    return lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2));
}

template <typename TYPE, typename T1, typename T2, typename T3>
lib::ds::shared_ptr<TYPE> make_shared(T1&& p1 , T2&& p2, T3&& p3)
{
    return lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3));
}

template <typename TYPE, typename T1, typename T2, typename T3, typename T4>
lib::ds::shared_ptr<TYPE> make_shared(T1&& p1 , T2&& p2, T3&& p3, T4&& p4)
{
    return lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3, p4));
}

template <typename TYPE, typename T1, typename T2, typename T3, typename T4, typename T5>
lib::ds::shared_ptr<TYPE> make_shared(T1&& p1 , T2&& p2, T3&& p3, T4&& p4, T5&& p5)
{
    return lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3, p4, p5));
}

template <typename TYPE, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
lib::ds::shared_ptr<TYPE> make_shared(T1&& p1 , T2&& p2, T3&& p3, T4&& p4, T5&& p5, T6&& p6)
{
    return lib::ds::shared_ptr<TYPE>(new TYPE(p1, p2, p3, p4, p5, p6));
}

//------------------------------------------------------------------------------
//  Set a shared pointer back to empty.                                   
//------------------------------------------------------------------------------
template<class T> 
void set_to_null(lib::ds::shared_ptr<T>& p)
{
    p = lib::ds::shared_ptr<T>();
}

} // namespace lib
#endif  //#ifdef LIB_DS_SHARED_PTR_H_FILE_GUARD
