//------------------------------------------------------------------------------
///@file lib_pod_work_vectorwithoffsetfilereader.h                              
///                                                                             
///@par Classification: UNCLASSIFIED, OPEN SOURCE                               
///                                                                             
///@warning Data to be read must be plain old data type                         
///         for this file reader to work properly                               
///                                                                             
//------------------------------------------------------------------------------

#ifndef LIB_POD_WORK_VECTORWITHOFFSETFILEREADER_H_FILE_GUARD
#define LIB_POD_WORK_VECTORWITHOFFSETFILEREADER_H_FILE_GUARD

#include "lib_work_file.h"
#include "lib_ds_vectorwithoffset.h"

#include "lib_ds_shared_ptr.h"

namespace lib {
namespace pod {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Template: VectorWithOffsetFileReader                                    
///                                                                             
///@warning Data to be read must be plain old data type                         
///         for this file writer to work properly                               
///                                                                             
///@brief   This is a template to read                                          
///         a vector with file offset object to file.                           
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     std::string filename = lib::config::unitTestDataFilePath                
///             ("irig106/ch8/clr/218gt123_20apr12_ch8.dat");                   
///     lib::pod::work::VectorWithOffsetFileReader<                             
///             lib::irig106::ch8::clr::ds::Word1x24> reader(filename);         
///     lib::ds::VectorWithOffset<                                              
///             lib::irig106::ch8::clr::ds::Word1x24> Word1x24Vector;           
///                                                                             
///     uint64_t result =  writer.read(Word1x24Vector);                         
///     std::cout<<"result: "<<result<<std::endl;                               
///                                                                             
///     writer.close();                                                         
///     @endcode                                                                
///                                                                             
///@tparam  TYPE    The TYPE of data held within the VectorWithOffset object.   
///                 The TYPE must be "plain old data": no pointers, no arrays.  
///                 Structures may be used if they only contain plain old data  
///                 and the size of the structure is equal to the sum of the    
///                 sizes of each structure member;                             
///                 ie., the compiler cannot add padding.                       
///                                                                             
///@version 2015-10-01  DHF     Made the base class a template parameter.       
///                             So that we can have other file readers besides  
///                             just lib::msg::File as the base.                
///                                                                             
///@version 2015-06-16  DMN     Added methods to support                        
///                             msg_vectorwithoffsetfilepublisher               
///                                                                             
///@version 2015-06-10  DHF     Removed reference to ch8 word.                  
///                                                                             
///@version 2015-06-10  DMN     Updated both read methods                       
///                                                                             
///@version 2014-10-08  DMN     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename TYPE, typename FILE_READER = lib::work::File>
class VectorWithOffsetFileReader : public FILE_READER
{

public:

    VectorWithOffsetFileReader(
        const std::string& path
      , uint64_t max_item_count = 32*1024 / sizeof(TYPE)
    )
        : FILE_READER(path, "rb")
        , m_MaximumItemCount(max_item_count)
    {
    }

    //--------------------------------------------------------------------------
    ///@brief   Read data into a vector up to m_MaximumItemCount                
    ///@param   items       The vector to read data into                        
    //--------------------------------------------------------------------------
    uint64_t read(lib::ds::VectorWithOffset<TYPE>& items)
    {
        items.setFileOffset(tellg());
        return FILE_READER::read(items, m_MaximumItemCount);
    }
    
    //--------------------------------------------------------------------------
    ///@brief   Read data into a vector up to sizeInBytes                       
    ///@param   items       The vector to read data into                        
    ///@param   sizeInBytes The size of bytes to read data into the vector      
    ///@note    Should be deprecated, but I will leave it                       
    ///         for backward compatible.                                        
    //--------------------------------------------------------------------------
    uint64_t read(lib::ds::VectorWithOffset<TYPE>& items, uint64_t sizeInBytes)
    {
        items.setFileOffset(tellg());
        return FILE_READER::read(items, sizeInBytes/sizeof(TYPE));
    }

    uint64_t get(lib::ds::shared_ptr<lib::ds::VectorWithOffset<TYPE> >& items)
    {
        lib::new_shared(items);
        return read(*items);
    }

    void setMaximumCountItem(uint64_t max_item_count){
        m_MaximumItemCount = max_item_count;
    }
    uint64_t maximumCountItem(){ return m_MaximumItemCount;}

    uint64_t tellg() const { return FILE_READER::tellg(); }

protected:

private:
    //----------------------------------------------------------------------
    //----------------------------------------------------------------------
    VectorWithOffsetFileReader(const VectorWithOffsetFileReader& that);
    VectorWithOffsetFileReader& operator=(const VectorWithOffsetFileReader& that);
    
    uint64_t m_MaximumItemCount;

};  // VectorWithOffsetFileReader //

} // namespace work //
} // namespace pod  //
} // namespace lib  //

#endif // #ifndef LIB_POD_WORK_VECTORWITHOFFSETFILEREADER_H_FILE_GUARD

