//------------------------------------------------------------------------------
///@file lib_work_bitstreamtopacketconversionlab.h                              
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_WORK_BITSTREAMTOPACKETCONVERSIONLAB_H_FILE_GUARD
#define LIB_WORK_BITSTREAMTOPACKETCONVERSIONLAB_H_FILE_GUARD

#include "lib_work_conversionlab.h"
#include "lib_work_vectorwithoffsetcollection.h"

#include <memory>


namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: BitStreamToPacketConversionLab                                   
///                                                                             
///@brief   Convert a bit stream into a packet.                                 
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@par By Pointer                                                              
///         If there is insufficient data at the end of an input item, the      
///         item is held until the next vector of data is obtained.             
///                                                                             
///@tparam  PACKET_TYPE     The output packet type; e.g.,                       
///                         lib::irig106::ch10::work::Packet.                   
///                                                                             
///@tparam  PACKET_FACTORY  A class that will given pointers to a byte          
///                         collection will return a pointer to PACKET_TYPE.    
///                         The class must have the following methods:          
///                             -   size_t minimumBytes()                       
///                                     Returns the minimum number of bytes     
///                                     to even be considered it's type.        
///                                     For irig106::ch10, this would return    
///                                     the number of bytes in the header.      
///                             -   size_t bytesNeeded(                         
///                                     std::vector<uint8_t>& const b           
///                                 )                                           
///                                     Returns the minimum number of bytes     
///                                     given miniumBytes() of bytes passed     
///                             -   STATUS  get(                                
///                                     shared_ptr<PACKET_TYPE>& p              
///                                   , std::vector<uint8_t> const& b           
///                                 )                                           
///                                 
///                                                                             
///@version 2020-09-15  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
template <typename PACKET_TYPE, typename PACKET_FACTORY>
class BitStreamToPacketConversionLab
    :   public lib::work::ConversionLabByPointer<
            lib::ds::VectorWithOffset
          , PACKET_TYPE
        >
{
    public:
        BitStreamToPacketConversionLab();
        BitStreamToPacketConversionLab(BitStreamToPacketConversionLab&& that) = default;
        BitStreamToPacketConversionLab(const BitStreamToPacketConversionLab& that) = delete;
        BitStreamToPacketConversionLab& operator=(const BitStreamToPacketConversionLab& that) = delete;
        virtual ~BitStreamToPacketConversionLab() = default;


        virtual void finalize() override;
        virtual void put(const InputType input_item) override;
    
    
    protected:
        virtual void process(bool last_time);


    private:
        lib::work::VectorWithOffsetCollection<uint8_t>  m_Bytes;

}; // class BitStreamToPacketConversionLab //

using  BitStreamToPacketConversionLabPtr = std::shared_ptr<BitStreamToPacketConversionLab>;
using  ConstBitStreamToPacketConversionLabPtr = std::shared_ptr<const BitStreamToPacketConversionLab>;

} // namespace work
} // namespace lib

#include "lib_work_bitstreamtopacketconversionlab.tpp"


#endif // #ifndef LIB_WORK_BITSTREAMTOPACKETCONVERSIONLAB_H_FILE_GUARD

