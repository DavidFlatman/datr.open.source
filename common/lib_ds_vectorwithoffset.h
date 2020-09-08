//------------------------------------------------------------------------------
///@file lib_ds_vectorwithoffset.h                                              
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
//------------------------------------------------------------------------------

#ifndef LIB_DS_VECTORWITHOFFSET_H_FILE_GUARD
#define LIB_DS_VECTORWITHOFFSET_H_FILE_GUARD

#include "lib_ds_offset.h"

#include <stdint.h>
#include <vector>
#include <stdexcept>

namespace lib {
namespace ds {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: VectorWithOffset                                                 
///                                                                             
///@brief   This class can be a replacement for the old "filechunk" class.      
///         It contains an ordered set (vector) of data and carries along       
///         the file offset of the data represented by the vector.              
///                                                                             
///@par Thread Safety:  class                                                   
///         This class is based on the std::vector which is only class          
///         thread safe.                                                        
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                             Removed boost::scoped_ptr                       
///                                                                             
///@version 2014-07-10  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
class VectorWithOffset 
    : public std::vector<TYPE>
    , public Offset
{
    public:
        //----------------------------------------------------------------------
        ///@param   fileOffset      The location in the file the data came from.
        ///@param   capacity        Capacity of vector before reallocatoin.     
        ///@param   size            Initial number of items in the vector.      
        //----------------------------------------------------------------------
        VectorWithOffset<TYPE>(
            uint64_t    fileoffset = -1
          , int         capacity = -1
          , int         size = 0
        ) : std::vector<TYPE>(size)
          , Offset(fileoffset)
        {
            if (capacity <= 0) {
                //  Ignore it.
            } else if (capacity < size) {
                throw std::range_error
                    ("Capacity must be greater or equal to size");
            } else {
                this->resize(size);
                this->reserve(capacity);
            }
        }
        VectorWithOffset<TYPE>(const VectorWithOffset& that)
            : std::vector<TYPE>(that)
            , Offset(that)
        {
            this->reserve(that.capacity());
        }

        virtual ~VectorWithOffset<TYPE>() { }

        VectorWithOffset<TYPE>& operator=(const VectorWithOffset<TYPE>& that)
        {
            this->std::vector<TYPE>::operator=(that);
            this->reserve(that.capacity());
            this->Offset::operator=(that);
            return *this;
        }

        uint64_t    fileOffset() const { return m_FileOffset; }
        void        setFileOffset(uint64_t offset) { m_FileOffset = offset; }

        uint64_t    capacityInBytes() const 
            { return this->capacity()*sizeof(TYPE); }
        uint64_t    sizeInBytes() const     
            { return this->size()    *sizeof(TYPE); }

        //----------------------------------------------------------------------
        ///@brief   Return the memory that contains the vector's data.          
        //----------------------------------------------------------------------
        void* memory() 
        { 
            //------------------------------------------------------------------
            //  this            Pointer to the current object.                  
            //  *this           Dereference the pointer to the current object,  
            //                  giving the current object.                      
            //  (*this)[0]      The object in the vector.                       
            //  &(*this)[0]     The address of the first object in the vector.  
            //------------------------------------------------------------------
            return &(*this)[0]; 
        }
        const void* memory() const 
        { 
            return &(*this)[0]; 
        }
    
    protected:

    private:

}; // class VectorWithOffset //

} // namespace ds
} // namespace lib

#endif // #ifndef LIB_DS_VECTORWITHOFFSET_H_FILE_GUARD

