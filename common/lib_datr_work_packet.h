//------------------------------------------------------------------------------
///@file lib_datr_work_packet.h                                                 
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_DATR_WORK_PACKET_H_FILE_GUARD
#define LIB_DATR_WORK_PACKET_H_FILE_GUARD

#include <memory>

namespace lib {
namespace datr {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: Packet                                                           
///                                                                             
///@brief   The basis for DATR packets.  Allows for some generic routines.      
///                                                                             
///@par Thoughts
///         -   Want a base class for Packets in general so that we can have    
///             a generic 
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-09-21  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class Packet
{
    public:
        Packet();
        Packet(Packet&& that) = default;
        Packet(const Packet& that) = delete;
        Packet& operator=(const Packet& that) = delete;
        virtual ~Packet() = default;
    
    
    protected:


    private:
        struct Data;
        std::unique_ptr<Data> m_Data;

}; // class Packet //

using  PacketPtr = std::shared_ptr<Packet>;
using  ConstPacketPtr = std::shared_ptr<const Packet>;

} // namespace work
} // namespace datr
} // namespace lib


#endif // #ifndef LIB_DATR_WORK_PACKET_H_FILE_GUARD

