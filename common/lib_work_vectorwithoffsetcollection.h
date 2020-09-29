//------------------------------------------------------------------------------
///@file lib_work_vectorwithoffsetcollection.h                                  
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_WORK_VECTORWITHOFFSETCOLLECTION_H_FILE_GUARD
#define LIB_WORK_VECTORWITHOFFSETCOLLECTION_H_FILE_GUARD

#include "lib_ds_vectorwithoffset.h"
#include "lib_ds_shared_ptr.h"

#include <deque>
#include <stdexcept>
#include <stdint.h>

namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: VectorWithOffsetCollection                                       
///                                                                             
///@brief   Buffer a file's data stored in VectorWithOffsets for easy access.   
///                                                                             
///         Items are put into the vector with offset as they come from a file  
///         and allow the user to access the data via a file-offset.  When the  
///         programmer is done with a specific location of the file, it is      
///         freed (and any attempts to use data on or before that location are  
///         subject to crashing without warning).                               
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@warning It is assumed that the data will be added to the collection in the  
///         same order that the data was taken from the file.  Furthermore, it  
///         is assumed that all of the data from the file will pass through the 
///         collection.                                                         
///                                                                             
///@par Terminology:                                                            
///         offset      The term offset refers to a file @e byte offset into    
///                     the data.                                               
///         index       The term index refers to a index * sizeof(TYPE) byte    
///                     offset into the data.                                   
///\n\n                                                                         
///                     index * sizeof(TYPE) == offset                          
///                                                                             
///@par Why does this class exist?                                              
///         I need to subscribe to lib::ds::VectorWithOffset objects and then   
///         look for CH10 packets within the vectors.  The problem is that a    
///         packet may be partially contained in the 103rd vector received and  
///         the rest in the 104th vector received.  The "to ch10 conversion     
///         lab" can eitehr handle the problem of splitting the data between    
///         the vectors or we can abstract that problem into a separate calss.  
///         This class.                                                         
///\n\n                                                                         
///         With the VectorWithOffsetCollection<uint8_t>, we can have the CH10  
///         conversion lab do things like:                                      
///         @code                                                               
///             // check if we have a Little Endian sync pattern of 0xEB25      
///             if (collection[i] == 0x25 && collection[i+1] == 0xEB) {         
///                 // we have sync ...                                         
///             }                                                               
///         @endcode                                                            
///                                                                             
///         Without the VectorWithOffsetCollection, we have to constantly worry 
///         if 0x25 is in the current VectorWithOffset and the 0xEB in the next 
///         VectorWithOFfset.                                                   
///                                                                             
///         The next issue that this class handles i when to release the        
///         underlying VectorWithOffset objects.  The CH10 conversion lab does  
///         a:                                                                  
///         @code                                                               
///             collection.free(packet->m_FileOffset);                          
///         @endcode                                                            
///         knowing that it doesn't have to access any of the data before the   
///         last packet that is created.  The VectorwithOffsetCollection looks  
///         to see if it is holding any VectorWithOffset objects that only hold 
///         data from before the given value to free.  If so, the data within   
///         that VectorWithOffset will never be needed again and the vector can 
///         safely be removed from the queue (freeing the data/memory).         
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///             lib::pod::work::VectorWithOffsetFileReader<uint32_t> reader;    
///             reader.open("my_file.dat");                                     
///             lib::ds::shared_ptr<lib::ds::VectorWithOffset<uint32_t> > vec;  
///                                                                             
///             lib::work::VectorWithOffsetCollection<uint32_t> collection;     
///                                                                             
///             while (lib::new_shared(vec), reader.get(vec)) {                 
///                 collection.push_back(vec);                                  
///                 for (int index = minimum(); index < size(); ++index) {      
///                     if (collection[index] == 42) {                          
///                         collection.free(index);                             
///                     }                                                       
///                 }                                                           
///             }                                                               
///             reader.close();                                                 
///                                                                             
///     @endcode                                                                
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2015-12-09  DHF     Removed m_Constant from DataNode and added      
///                             bool constant() method instead (based on        
///                             review feedback from DMN).                      
///                                                                             
///@version 2015-11-23  DHF     Made the collection modifiable.                 
///                                                                             
///@version 2015-10-23  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
class VectorWithOffsetCollection
{
    public:
        typedef typename lib::ds::VectorWithOffset<TYPE>    Vector;
        typedef typename lib::ds::shared_ptr<Vector>        Pointer;
        typedef typename lib::ds::shared_ptr<const Vector>  ConstPointer;

        VectorWithOffsetCollection();
        virtual ~VectorWithOffsetCollection();

        TYPE& operator[](uint64_t index);
        const TYPE& operator[](uint64_t index) const;
        void free(uint64_t index);

        uint64_t minimum() const;
        uint64_t size() const;
        uint64_t available() const;
        bool empty() const;
        void clear();
        void push_back(Pointer item);
        void push_back(ConstPointer item);

        uint64_t memory_used() const;
        bool memcpy(void* dest, uint64_t index, uint64_t bytes) const;

    protected:

        virtual void vectorFreedEvent(ConstPointer p);

    private:
        struct DataNode
        {
            Pointer         m_Pointer;
            ConstPointer    m_ConstPointer;

            //------------------------------------------------------------------
            ///@brief   Return true if the DataNode holds constant only data.   
            //------------------------------------------------------------------
            inline bool constant() const
            {
                return m_Pointer.get() == NULL;
            }
        };

        typedef typename std::deque<DataNode>       Collection;
        typedef typename Collection::iterator       iterator;
        typedef typename Collection::const_iterator const_iterator;

        Collection  m_Data;
        uint64_t    m_LowestIndex;

        iterator       findNode(uint64_t index);
        const_iterator findNode(uint64_t index) const;

        uint64_t firstIndex(const_iterator p) const;
        uint64_t firstIndex(ConstPointer p) const;

        uint64_t lastIndex (const_iterator p) const;
        uint64_t lastIndex (ConstPointer p) const;

}; // class VectorWithOffsetCollection //

} // namespace work 
} // namespace lib  

#include "lib_work_vectorwithoffsetcollection.tpp"

#endif // #ifndef LIB_WORK_VECTORWITHOFFSETCOLLECTION_H_FILE_GUARD

