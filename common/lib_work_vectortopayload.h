//------------------------------------------------------------------------------
///@file lib_work_vectortopayload.h                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_WORK_VECTORTOPAYLOAD_H_FILE_GUARD
#define LIB_WORK_VECTORTOPAYLOAD_H_FILE_GUARD

#include "lib_ds_standardunits.h"
#include <memory>


namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: VectorToPayload                                                  
///                                                                             
///@brief   Convert a vector with offset to a payload based on a file           
///         configurtaion information.                                          
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@par Assumptions:                                                            
///         -   The data being passed in can be chunked up into "packets"       
///             where the packet has a length of the packet defined in the data.
///                                                                             
///@par Knowledge                                                               
///         -   This class knows about:                                         
///                 -   The number of file header bits.                         
///                 -   The offset into the packet for the packet length        
///                 -   The type of the packet length                           
///                         -   Bit length of the packet length                 
///                         -   Endianness of the packet length                 
///                         -   Units of the packet length                      
///                         -   Whether the packet length is all encompassing   
///                             of the data (e.g., IRIG-106, Chapter 10), or    
///                             if another offset is needed (e.g., FCP).        
///         -   This class hides:                                               
///         -   This class is agnostic about:                                   
///                 -   The size or meaning of the packet header outside of     
///                     packet length.                                          
///         -   Future Expansion                                                
///                 -   Packet sync pattern (e.g., 0xEB25)                      
///                 -   Checksums                                               
///                 -   Maximum packet size                                     
///                 -   Minimum packet size                                     
///                 -   Packet size multiple of (e.g., CH10 size mod 4 == 0)    
///                                                                             
///@version 2020-09-14  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class VectorToPayload
{
    public:
        VectorToPayload();
        VectorToPayload(VectorToPayload&& that) = default;
        VectorToPayload(const VectorToPayload& that) = delete;
        VectorToPayload& operator=(const VectorToPayload& that) = delete;
        virtual ~VectorToPayload() = default;

        void setFileHeaderSize(
            size_t headerSize
          , ds::StandardUnits units = ds::StanardUnits::Bytes
        )
        void set
    
    
    protected:


    private:
        struct Data;
        std::unique_ptr<Data> m_Data;

}; // class VectorToPayload //

using  VectorToPayloadPtr = std::shared_ptr<VectorToPayload>;
using  ConstVectorToPayloadPtr = std::shared_ptr<const VectorToPayload>;

} // namespace work
} // namespace lib


#endif // #ifndef LIB_WORK_VECTORTOPAYLOAD_H_FILE_GUARD

