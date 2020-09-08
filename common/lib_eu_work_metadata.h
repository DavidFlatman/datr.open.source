//------------------------------------------------------------------------------
///@file lib_eu_work_metadata.h                                                 
///@par  Classification:  UNCLASSIFIED, OPEN SOURCE                             
//------------------------------------------------------------------------------

#ifndef LIB_EU_WORK_METADATA_H_FILE_GUARD
#define LIB_EU_WORK_METADATA_H_FILE_GUARD

#include "lib_eu_work_datatype.h"

#include <memory>
#include <stdint.h>
#include <string>


namespace lib {
namespace eu {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: MetaData                                                         
///                                                                             
///@brief   MetaData about an engineering unit value.                           
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@par Expected Usage:                                                         
///     @code                                                                   
///     @endcode                                                                
///                                                                             
///@version 2020-08-27  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------
class MetaData
{
    public:
        MetaData(
            std::string const&      name
          , std::string const&      units
          , uint32_t                word_sequence
          , uint32_t                bit_count
          , uint32_t                least_significant_bit
          , double                  scale
          , double                  most_significant_value
          , double                  range_low
          , double                  range_high
          , bool                    is_signed
          , DataType                data_type
        );
        MetaData(MetaData&& that) = default;
        MetaData(const MetaData& that) = default;
        MetaData& operator=(const MetaData& that) = default;
        virtual ~MetaData() = default;

        uint32_t wordSequence() const        
            { return m_WordSequence; }

        uint32_t lsb() const
            {   return m_LeastSignificantBit; }

        double msbValue() const 
            {   return m_MostSignificantBitValue; }


        uint32_t bitCount()       const { return m_BitCount; }

        double scale()            const { return m_Scale; }

        std::string const& units()const {   return m_Units; }

        double rangeLow()         const { return m_RangeLow; }

        double rangeHigh()        const { return m_RangeHigh; }

        std::string const& name() const { return m_Name; }

        bool isSigned()           const { return m_IsSigned; }

        DataType dataType()       const { return m_DataType; }


    protected:


    private:
        uint32_t            m_WordSequence;
        uint32_t            m_BitCount;
        uint32_t            m_LeastSignificantBit;
        double              m_Scale;
        double              m_MostSignificantBitValue;
        std::string         m_Units;
        double              m_RangeLow;
        double              m_RangeHigh;

        std::string         m_Name;

        bool                m_IsSigned;
        DataType            m_DataType;
    

}; // class MetaData //

using  MetaDataPtr = std::shared_ptr<MetaData>;
using  ConstMetaDataPtr = std::shared_ptr<const MetaData>;

} // namespace work
} // namespace eu
} // namespace lib


#endif // #ifndef LIB_EU_WORK_METADATA_H_FILE_GUARD

