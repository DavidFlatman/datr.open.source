//------------------------------------------------------------------------------
///@file lib_work_bitstreamtopacketconversionlab.tpp                            
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------


namespace lib {
namespace work {

//------------------------------------------------------------------------------
//  Default constructor.                                                        
//------------------------------------------------------------------------------
template <typename PACKET_TYPE, typename PACKET_FACTORY>
BitStreamToPacketConversionLab<PACKET_TYPE, PACKET_FACTORY>::
BitStreamToPacketConversionLab()
{
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename PACKET_TYPE, typename PACKET_FACTORY>
void BitStreamToPacketConversionLab<PACKET_TYPE, PACKET_FACTORY>::
put(const InputType input_item) 
{
    m_Bytes.push_back(input_item);
    process(false);
}

{
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename PACKET_TYPE, typename PACKET_FACTORY>
void BitStreamToPacketConversionLab<PACKET_TYPE, PACKET_FACTORY>::
fianlize()
{
    process(true);
}

{
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template <typename PACKET_TYPE, typename PACKET_FACTORY>
void BitStreamToPacketConversionLab<PACKET_TYPE, PACKET_FACTORY>::
process(bool last_time)
{
    
}

{
}


} // namespace work 
} // namespace lib 
