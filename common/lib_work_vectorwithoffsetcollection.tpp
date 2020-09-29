//------------------------------------------------------------------------------
///@file lib_work_vectorwithoffsetcollection.tpp                                
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#include <deque>
#include <string.h>                         // memcpy   

namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: VectorWithOffsetCollection                                       
///                                                                             
///@brief   Buffer a file's data stored in VectorWithOffsets for easy access.   
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@version 2020-09-21  DHF     Made open source.                               
///                                                                             
///@version 2015-12-09  DHF     Used DataNode::constant() instead of            
///                             DataNote::m_Constant based on review feedback   
///                             from DMN.                                       
///                                                                             
///@version 2015-11-23  DHF     Made the collection modifiable.                 
///                                                                             
///@version 2015-10-23  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename TYPE>
VectorWithOffsetCollection<TYPE>::VectorWithOffsetCollection()
    : m_LowestIndex(0)
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename TYPE>
VectorWithOffsetCollection<TYPE>::~VectorWithOffsetCollection()
{
}

//------------------------------------------------------------------------------
///@brief   Return the indexTH element of the file so the person can change it. 
//------------------------------------------------------------------------------
template <typename TYPE>
TYPE& VectorWithOffsetCollection<TYPE>::operator[](uint64_t index)
{
    if (index < m_LowestIndex) {
        throw std::range_error("access attempt of freed data");

    } else if (index >= size()) {
        throw std::range_error("access attempt beyond end of vector");
    }

    iterator node = findNode(index);
    if (node->constant()) {
        lib::new_shared(node->m_Pointer, *node->m_ConstPointer);
    }

    assert(node != m_Data.end());

    return (*(node->m_Pointer))[index-node->m_Pointer->m_FileOffset /sizeof(TYPE)];
}

//------------------------------------------------------------------------------
///@brief   Return the indexTH element of the file.                             
//------------------------------------------------------------------------------
template <typename TYPE>
const TYPE& VectorWithOffsetCollection<TYPE>::operator[](uint64_t index) const
{
    if (index < m_LowestIndex) {
        throw std::range_error("access attempt of freed data");

    } else if (index >= size()) {
        throw std::range_error("access attempt beyond end of vector");
    }
    const_iterator node = findNode(index);

    assert(node != m_Data.end());

    return (*(node->m_Pointer))[index-node->m_Pointer->m_FileOffset /sizeof(TYPE)];
}

//------------------------------------------------------------------------------
///@brief   User will no longer need index and below; free uneeded memory.      
//------------------------------------------------------------------------------
template <typename TYPE>
void VectorWithOffsetCollection<TYPE>::free(uint64_t index)
{
    if (index < m_LowestIndex) return;
    m_LowestIndex = index + 1;

    const_iterator i(m_Data.begin());

    bool done = false;
    while (i != m_Data.end() && !done)
    {
        if (lastIndex(i) <= m_LowestIndex) {
            if (i->constant()) {
                vectorFreedEvent(i->m_ConstPointer);
            } else {
                vectorFreedEvent(i->m_Pointer);
            }
            m_Data.pop_front();
            i = m_Data.begin();
        } else {
            done = true;
        }
    }

}

//------------------------------------------------------------------------------
///@brief   Return the minimum valid index.  This should be the highest free    
///         value + 1 (if there's anything in the collection).                  
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::minimum() const
{
    uint64_t result(uint64_t(-1));

    if (!m_Data.empty())
    {
        result = firstIndex(m_Data.begin());
        if (result < m_LowestIndex) result = m_LowestIndex;
    }

    return result;

}

//------------------------------------------------------------------------------
///@brief   Return the last allowed index.                                      
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::size() const
{
    uint64_t result(0);
    typename Collection::const_reverse_iterator last(m_Data.rbegin());

    if (last != m_Data.rend()) 
    {
        result = lastIndex(last->m_ConstPointer);
    }
    return result;
}

//------------------------------------------------------------------------------
///@brief   Return the number of elements available in the collection.          
///                                                                             
///         This method differs from size() in that size is the last allowed    
///         index (+1) and ignores the minimum (what has been freed.            
///         This method accounts for the no longer available freed ...          
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::available() const
{
    return empty() ? 0 : size() - minimum();
}

//------------------------------------------------------------------------------
///@brief   Return true if there is no data available.                          
//------------------------------------------------------------------------------
template <typename TYPE>
bool VectorWithOffsetCollection<TYPE>::empty() const
{
    return m_Data.empty();
}

//------------------------------------------------------------------------------
///@brief   Clear the contents of the vector.                                   
///                                                                             
///@todo    Okay, what happens to the free index?!?  Should it go back to       
///         0 or should it continue where it left off?                          
//------------------------------------------------------------------------------
template <typename TYPE>
void VectorWithOffsetCollection<TYPE>::clear()
{
    const_iterator i;
    for (i = m_Data.begin(); i != m_Data.end(); ++i) {
        vectorFreedEvent(i->ConstPointer);
    }

    m_Data.clear();
}

//------------------------------------------------------------------------------
///@brief   Add a non constant VectorWithOffset onto the collection.            
//------------------------------------------------------------------------------
template <typename TYPE>
void VectorWithOffsetCollection<TYPE>::push_back(Pointer item)
{
    DataNode node;
    node.m_Pointer      = item;
    node.m_ConstPointer = item;
    m_Data.push_back(node);
}

//------------------------------------------------------------------------------
///@brief   Add a constant VectorWithOffset onto the collection.                
//------------------------------------------------------------------------------
template <typename TYPE>
void VectorWithOffsetCollection<TYPE>::push_back(ConstPointer item)
{
    DataNode node;
    node.m_ConstPointer = item;
    m_Data.push_back(node);
}

//------------------------------------------------------------------------------
///@brief   Return the number of bytes stored in the buffers.                   
///                                                                             
///@note    The result is a partial lie.  The purpose is to know if we're       
///         ballooning or actually releasing memory when a free occurs.  The    
///         result ignores the fact that we might have the same data in         
///         multiple locations.                                                 
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::memory_used() const
{
    uint64_t result(0);

    for (const_iterator i = m_Data.begin(); i != m_Data.end(); ++i)
    {
        result += i->m_ConstPointer->size() * sizeof(TYPE);
    }

    return result;
}

//------------------------------------------------------------------------------
///@brief   If the memory requested is still held, copy it into the destination.
///@par Why?                                                                    
///         It is very possible that the memory requested will span multiple    
///         VectorWithOffset objects...                                         
///@param   dest    Pointer to the destination.                                 
///@param   index   Index to the beginning of the data that is requested        
///@param   byteLen Length of data, in bytes, to be returned.                   
///@result  true = There was enough memory and it was copied.                   
///         false = There was not enough memory and it was not copied.          
//------------------------------------------------------------------------------
template <typename TYPE>
bool VectorWithOffsetCollection<TYPE>:: memcpy(
    void* dest
  , uint64_t index
  , uint64_t bytes
) const
{
    bool result = 
        !empty()
      && (index >= minimum())
      && ((size() - index) * sizeof(TYPE) >= bytes)
    ;

    if (result)
    {
        const_iterator i = findNode(index);

        uint8_t* dest_bytes((uint8_t*) dest);
        uint64_t dest_index(0);

        //----------------------------------------------------------------------
        //  Convert index from the file index to the vector index.              
        //----------------------------------------------------------------------
        index -= i->m_ConstPointer->m_FileOffset / sizeof(TYPE);

        while (bytes > 0) 
        {
            uint64_t count = (i->m_ConstPointer->size() - index) * sizeof(TYPE);
            if (count > bytes) count = bytes;
            ::memcpy(
                &dest_bytes[dest_index]
              , &((*(i->m_ConstPointer))[index])
              , count
            ); 
            bytes       -= count;
            dest_index  += count;
            index        = 0;
            ++i;
        }
    }

    return result;

} // bool VectorWithOffsetCollection<TYPE>:: memcpy(...)

//------------------------------------------------------------------------------
///@brief   Return the index of the first element +1 in the given vector.       
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::firstIndex(ConstPointer p) const
{
    return p->m_FileOffset / sizeof(TYPE) ;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::firstIndex(const_iterator p) const
{
    return firstIndex(p->m_ConstPointer);
}

//------------------------------------------------------------------------------
///@brief   Return the index of the last element +1 in the given vector.        
///                                                                             
///@par Why +1?                                                                 
///     @code                                                                   
///         for (int index = vwoc.firstIndex(p); index < vwoc.lastIndex(p); ++i)
///     @endcode                                                                
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::lastIndex (ConstPointer p) const
{
    return firstIndex(p) + p->size();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename TYPE>
uint64_t VectorWithOffsetCollection<TYPE>::lastIndex (const_iterator p) const
{
    return lastIndex(p->m_ConstPointer);
}

//------------------------------------------------------------------------------
///@brief   Allow a derived class to know when a given VectorWithOffset is no   
///         longer needed.                                                      
///                                                                             
///@par Why ConstPointer versus Pointer?                                        
///         We want to avoid creating Pointer objects if at all possible        
///         since they may be expensive to create (forces a copy).              
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE>
void VectorWithOffsetCollection<TYPE>::vectorFreedEvent(ConstPointer p)
{
}

//------------------------------------------------------------------------------
///@brief   Find and return an iterator to the node containing the data for     
///         the given index.                                                    
//------------------------------------------------------------------------------
template <typename TYPE>
typename VectorWithOffsetCollection<TYPE>::iterator
VectorWithOffsetCollection<TYPE>::findNode(uint64_t index) 
{
    iterator result(m_Data.begin());

    bool found(false);
    while (result != m_Data.end() && !found)
    {
        found = firstIndex(result->m_ConstPointer) <= index
          &&    lastIndex (result->m_ConstPointer) > index
        ;

        if (!found) ++result;
    }
    return result;
}

//------------------------------------------------------------------------------
///@brief   Find and return an iterator to the node containing the data for     
///         the given index.                                                    
//------------------------------------------------------------------------------
template <typename TYPE>
typename VectorWithOffsetCollection<TYPE>::const_iterator
VectorWithOffsetCollection<TYPE>::findNode(uint64_t index) const
{
    const_iterator result(m_Data.begin());

    bool found(false);
    while (result != m_Data.end() && !found)
    {
        found = firstIndex(result->m_ConstPointer) <= index
          &&    lastIndex (result->m_ConstPointer) > index
        ;

        if (!found) ++result;
    }
    return result;
}

} // namespace work 
} // namespace lib  

