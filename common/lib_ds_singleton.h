//------------------------------------------------------------------------------
///@file lib_ds_singleton.h                                                   
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///@note after https://www.theimpossiblecode.com/blog/c11-generic-singleton-pattern/
/// and https://gist.github.com/sagi-z/c30f7f4aab63f0016a32040b023a86a9#file-singleton-h
///@note licensing from above site:
/// The MIT License
/// 
/// Copyright 2017 Sagi Zeevi (theimpossiblecode.com)
/// 
/// Permission is hereby granted, free of charge, to any person obtaining
/// a copy of this software and associated documentation files (the
/// "Software"), to deal in the Software without restriction, including
/// without limitation the rights to use, copy, modify, merge, publish,
/// distribute, sublicense, and/or sell copies of the Software, and to
/// permit persons to whom the Software is furnished to do so, subject to
/// the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included
/// in all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
/// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
/// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
/// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
/// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
/// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//------------------------------------------------------------------------------
#ifndef LIB_DS_SINGLETON_H
#define LIB_DS_SINGLETON_H

namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///
///@par Class: singleton
///
///@brief a simple singleton
///
///@par Purpose: contain a single instance of a class
///
///@par Thread Safety: object
///
///@par A simplified replacement for boost::singleton (no need for lease).
///         It's a basic singleton (one instance only). It's thread-safe using
///         C++ 11 magic static and lambdas as described in
/// https://www.theimpossiblecode.com/blog/c11-generic-singleton-pattern/.
///\n\n
///
///@par Extending This Class
///         This class is a slightly-trimmed version of the class described
///         at the web site above. It doesn't have the option to use an
///         external initializer when using a constructor with a parameter,
///         but relies purely on the contained class constructor.
///
///@par Using this class to hold a single instance of type T
/// 
///         Create an instance somewhere:
/// 
///             lib::ds::singleton<T> unique;
/// 
///         Use the instance as needed
/// 
///             unique->do_something();
/// 
///         At the absolute end of the singleton lifetime, use
/// 
///             lib::ds::singleton<T>::destroy();
/// 
///         Critical point: after destroy() is called, any attempts to
///         use the singleton will result in a nullptr dereference error.
/// 
///@version 2020-09-21  JRS     File Creation
//------------------------------------------------------------------------------
template <typename T>
class singleton
{
public:

	/// @brief modifiable access by pointer
	/// @return pointer to modifiable controlled instance of the object
	T* operator->()
	{
		return m_instance;
	}

	/// @brief non-modifiable access by pointer
	/// @return pointer to non-modifiable controlled instance of the object
	const T* operator->() const
	{
		return m_instance;
	}

	/// @brief retrieve the sole instance of the guarded object type.
	/// @note @instance() is not strictly needed for normal operations;
	/// the accessor operators above should be sufficient.
	static T* instance()
	{
		static bool static_init = []()->bool {
			m_instance = new T;
			return true;
		}();
		return m_instance;
	}

	/// @brief destroy the controlled instance.
	/// @note NOT thread safe!
	/// @note doesn't really do the right thing in that calling instance()
	/// after destroy() will not get a new or reconstituted instance, but
	/// will simply return nullptr.
	static void destroy()
	{
		T* p = nullptr;
		std::swap(p, m_instance);
		delete p;
	}

	/// @brief singleton constructor.
	/// @note implementation detail: uses c++11 magic statics to be thread-safe
	singleton()
	{
		static bool static_init = []()->bool {
			m_instance = new T;
			return true;
		}();
	}

	/// @brief delete copy constructor to make non-copyable
	singleton (const singleton&) = delete;

	/// @brief delete assignment operator to make non-copyable
	singleton& operator=(const singleton&) = delete;

private:
	/// the single instance
	static T* m_instance;
};

// definition of the static
template <typename T>
T* singleton<T>::m_instance;

// if there is a desire to make this fully boost-compatible,
// there needs to be a definition of a lease class here:
// template <typename T> struct lease : singleton<T> {};

}
};

#endif

