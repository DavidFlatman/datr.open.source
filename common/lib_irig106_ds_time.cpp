//------------------------------------------------------------------------------
///@file lib_irig106_ds_time.cpp
///@par Classification:  UNCLASSIFIED, OPEN SOURCE
//------------------------------------------------------------------------------
#include "lib_irig106_ds_time.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace lib {
namespace irig106 {
namespace ds {

//------------------------------------------------------------------------------
///@version 2012-10-12      AGB     Original incarnation
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///@brief Outputs a text representation of a time encoding scheme name to an 
///     ostream
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, TIME_ENCODING_SCHEME e)
{
    switch (e)
    {
        case TES_CH4_BINARY:    return out << "tes-ch4-binary";
        case TES_CH4_BCD:       return out << "tes-ch4-bcd";
        case TES_CH10_DAY:      return out << "tes-ch10-day";
        case TES_CH10_DMY:      return out << "tes-ch10-dmy";
        case TES_CH10_RELATIVE: return out << "tes-ch10-relative";
        case TES_CH10_IEEE1588: return out << "tes-ch10-ieee1588";
        default:                return out << "undefined";
    }
}

//------------------------------------------------------------------------------
///@brief Converts a text representation of a time encoding scheme name to an
///         enum value
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& in, TIME_ENCODING_SCHEME& e)
{
    std::string x;
    in >> x;
    boost::algorithm::to_lower(x);
    boost::algorithm::replace_all(x, "-", "_");

    if      (x == "tes_ch4_binary")     {e = TES_CH4_BINARY;}
    else if (x == "tes_ch4_bcd")        {e = TES_CH4_BCD;}
    else if (x == "tes_ch10_day")       {e = TES_CH10_DAY;}
    else if (x == "tes_ch10_dmy")       {e = TES_CH10_DMY;}
    else if (x == "tes_ch10_relative")  {e = TES_CH10_RELATIVE;}
    else if (x == "tes_ch10_ieee1588")  {e = TES_CH10_IEEE1588;}
    else                                
    {
        e = TES_UNDEFINED;
        in.setstate(std::ios::failbit);
    }

    return in;
}

} // namespace ds
} // namespace irig106
} // namespace lib
