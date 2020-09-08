
//------------------------------------------------------------------------------
///@file lib_eu_work_metadata.cpp                                               
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
///                                                                             
///@version 2020-08-27  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_eu_work_metadata.h"

namespace lib {
namespace eu {
namespace work {


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
MetaData::MetaData( 
    std::string const& name 
  , std::string const& units 
  , uint32_t word_sequence 
  , uint32_t bit_count 
  , uint32_t least_significant_bit 
  , double scale 
  , double most_significant_value 
  , double range_low 
  , double range_high
  , bool        is_signed
  , DataType    data_type
) : m_WordSequence(word_sequence)
  , m_BitCount(bit_count)
  , m_LeastSignificantBit(least_significant_bit)
  , m_Scale(scale)
  , m_MostSignificantBitValue(most_significant_value)
  , m_Units(units)
  , m_RangeLow(range_low)
  , m_RangeHigh(range_high)
  , m_Name(name)
  , m_IsSigned(is_signed)
  , m_DataType(data_type)
{
}


} // namespace work //
} // namespace eu //
} // namespace lib //


