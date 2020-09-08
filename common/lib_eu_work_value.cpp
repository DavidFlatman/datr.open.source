//------------------------------------------------------------------------------
///@file lib_eu_work_value.cpp                                                  
///                                                                             
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
///                                                                             
///@version 2020-08-27  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_eu_work_value.h"
#include "lib_bits_work.h"

#include <assert.h>
#include <string.h>                     // memcpy

namespace lib {
namespace eu {
namespace work {

static const uint32_t g_WordSequenceOrigin(1);
static const uint32_t g_BitsPerByte(8);
static const uint32_t g_BitsPerWord(16);
static const uint32_t g_BytesPerWord(g_BitsPerWord / g_BitsPerByte);

static const uint32_t g_IntegerSizeBits(sizeof(uint64_t) * g_BitsPerByte);

Value::Value(ConstMetaDataPtr metaData) : m_MetaData(metaData)
  , m_Double(0.0)
  , m_Unsigned(0)
  , m_Integer(0)
{
}


Value::~Value()
{
}

ConstMetaDataPtr Value::metaData() const { return m_MetaData; }

double      Value::toDouble() const    { return m_Double; }
void        Value::setDouble(double d) { setInternal(d); }

uint64_t    Value::toUnsigned() const      { return m_Unsigned; }
void        Value::setUnsigned(uint64_t u) { setInternal(u); }

uint64_t    Value::toInteger() const     { return m_Integer; }
void        Value::setInteger(int64_t i) { setInternal(i); }

std::string Value::toString(
    PARTS       parts 
  , uint32_t    total_size    
  , uint32_t    fraction_size 
)
{
    std::string result;

    return result;
}



//------------------------------------------------------------------------------
///@brief                                                                       
//------------------------------------------------------------------------------
void Value::setInternal(void const* p, size_t size)
{
    MetaData const& md(*m_MetaData);

    m_Double = m_Unsigned = m_Integer = 0;

    uint32_t word_index(0);
    uint32_t msb_offset(0);

    switch(md.dataType())
    {
        case DataType::Fixed:
            break;
        case DataType::String:
            break;

        case DataType::Integer:
            word_index = md.wordSequence() 
                       - g_WordSequenceOrigin
                       + (md.bitCount() - 1) / g_BitsPerWord
            ;
            msb_offset = (word_index * g_BitsPerWord) + md.lsb() - md.bitCount();
            if (md.isSigned())
            {
                lib::bits::work::getSigned(
                    m_Integer
                  , p
                  , msb_offset
                  , md.bitCount()
                );
                m_Double = m_Unsigned = m_Integer;
            } else {
                lib::bits::work::getUnsigned(
                    m_Unsigned
                  , p
                  , msb_offset
                  , md.bitCount()
                );
                m_Double = m_Integer  = m_Unsigned;
            }

            if (md.isSigned())
            {
                //--------------------------------------------------------------
                //  Get signed extension going ...                              
                //--------------------------------------------------------------
                m_Unsigned = (m_Unsigned << (g_IntegerSizeBits - md.bitCount()));
                memcpy(&m_Integer, &m_Unsigned, sizeof(m_Integer));

                m_Unsigned = m_Unsigned >> (g_IntegerSizeBits - md.bitCount());
                m_Integer  = m_Integer  >> (g_IntegerSizeBits - md.bitCount());
            }
            break;

        case DataType::Other:
            break;
        default:
            assert(0);
            break;
    }
}



} // namespace work //
} // namespace eu //
} // namespace lib //


