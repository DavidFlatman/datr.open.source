//------------------------------------------------------------------------------
///@file lib_work_packet.h                                                      
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_WORK_PACKET_H_FILE_GUARD
#define LIB_WORK_PACKET_H_FILE_GUARD

#include "lib_ds_standardunits.h"

#include <memory>
#include <stddef.h>
#include <stdint.h>


namespace lib {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Packet                                                           
///                                                                             
///@brief   Base class for "packets".                                           
///                                                                             
///         A packet is defined as having a header, a payload, and a trailer.   
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-09-15  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Packet
{
    public:
        Packet();
        Packet(Packet&& that) = default;
        Packet(const Packet& that) = delete;
        Packet& operator=(const Packet& that) = delete;
        virtual ~Packet();

        void setHeaderSize(
            size_t size
          , lib::ds::StandardUnits units = lib::ds::StandardUnits::BYTES
        )
        size_t headerSize(
            lib::ds::StandardUnits units = lib::ds::StandardUnits::BYTES
        );
    
    
    protected:


    private:
        struct Data;
        std::unique_ptr<Data> m_Data;

}; // class Packet //

using  PacketPtr = std::shared_ptr<Packet>;
using  ConstPacketPtr = std::shared_ptr<const Packet>;

} // namespace work
} // namespace lib


#endif // #ifndef LIB_WORK_PACKET_H_FILE_GUARD

