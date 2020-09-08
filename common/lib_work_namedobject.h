//------------------------------------------------------------------------------
///@file lib_work_namedobject.h                                                 
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------
#ifndef LIB_NAMEDOBJECT_H
#define LIB_NAMEDOBJECT_H

#include <string>

namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: NamedObject                                                      
///                                                                             
///@brief                                                                       
///                                                                             
///@par Purpose:                                                                
///                                                                             
///@par Thread Safety:  class/special                                           
///         The setName is not reentrant and is usually only set at             
///         construction time.                                                  
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2013-10-16  DHF     Added default constructor to allow the class to 
///                             be a virutal base class.                        
///                                                                             
///@version 2010-01-26  DHF     Changed outer namespace from tools to lib.      
///                                                                             
///@version 2009-11-23  DHF     File creation                                   
//------------------------------------------------------------------------------
class NamedObject
{
    public:
        NamedObject(const std::string& name = "anonymous") : m_Name(name) {}
        NamedObject(const NamedObject& that) : m_Name(that.m_Name) {}
        virtual ~NamedObject() {}

        NamedObject& operator=(const NamedObject& that) {
            m_Name = that.m_Name;
            return *this;
        }

        virtual void setName(const std::string& name) {m_Name = name;}
        virtual std::string name() const { return m_Name; }

    protected:

    private:
        std::string m_Name;

}; // class NamedObject //

} // namespace work //
} // namespace lib //

#endif // #ifndef LIB_NAMEDOBJECT_H //
