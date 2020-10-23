//------------------------------------------------------------------------------
///@file lib_crc.h                                                              
///@par  Classification:  UNCLASSIFIED - OPEN SOURCE                                        
//------------------------------------------------------------------------------

#include <boost/crc.hpp>
#include <vector>
#include "debug.h"

#ifndef LIB_CRC_H_FILE_GUARD
#define LIB_CRC_H_FILE_GUARD

namespace lib {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: CRC                                                              
///                                                                             
///@brief   Extend the API of boost::crc.                                       
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             
///@note    It is expected the the parameter to the template (and hence the base
///         class) will be a version of boost::crc_optimal.  Other types haven't
///         beeen tested.  Your mileage may vary.  If a rash or fever persist,  
///         please consult a physician.                                         
///                                                                             
///                                                                             
///@version                                                                     
///     2010-05-05      DHF         File creation
///     2020-10-23      PN          Open Source                               
//------------------------------------------------------------------------------
template <typename TYPE>
class crc_optimal : public TYPE
{
    public:
        typedef typename TYPE::value_type value_type;

        crc_optimal()  {}
        virtual ~crc_optimal() {}

        //----------------------------------------------------------------------
        ///  Bring the TYPE::operator()() forward.                              
        //----------------------------------------------------------------------
        value_type operator()() { return TYPE::operator()(); }

        //----------------------------------------------------------------------
        ///  Provide operator() for the various numeric types.                  
        //----------------------------------------------------------------------
        void operator()(unsigned char byte) { return TYPE::operator()(byte); }
        void operator()(char b) { return TYPE::operator()((unsigned char)b); }

        void operator()(uint16_t w16) {
            TYPE::operator()((w16 & 0xFF00) >> 8);
            TYPE::operator()((w16 & 0x00FF) >> 0);
        }
        void operator()(int16_t w16) { operator()(uint16_t(w16)); }
        
        void operator()(uint32_t w32) {
            TYPE::operator()(uint16_t((w32 & 0xFFFF0000) >> 16));
            TYPE::operator()(uint16_t((w32 & 0x0000FFFF) >>  0));
        }
        void operator()(int32_t w32) { operator()(uint32_t(w32)); }
        
        void operator()(uint64_t w64) {
            TYPE::operator()(uint32_t((w64 & 0xFFFFFFFF00000000) >> 32));
            TYPE::operator()(uint32_t((w64 & 0x00000000FFFFFFFF) >>  0));
        }
        void operator()(int64_t w64) { operator()(uint64_t(w64)); }
        
        //----------------------------------------------------------------------
        ///  Process an array / pointer to the data.                            
        //----------------------------------------------------------------------
        template <typename ITEM_TYPE>
        void operator()(const ITEM_TYPE* items, size_t item_count) 
        {
            for (size_t i=0; i < item_count; ++i) {
                uint64_t mask = uint64_t(0xFF) << (8 * (sizeof(ITEM_TYPE)-1));

                for (int byte = sizeof(ITEM_TYPE)-1; byte >= 0; --byte) {
                    TYPE::process_byte(char((uint64_t(items[i] & mask) >> (byte * 8)) & 0xFF));
                    mask = mask >> 8;
                }
            }
        }

        //----------------------------------------------------------------------
        ///  Process a vector of data.                                          
        //----------------------------------------------------------------------
        template <typename ITEM_TYPE>
        void operator()(const std::vector<ITEM_TYPE>& v) {
            return operator()(&v[0], v.size());
        }

    protected:

    private:
        //----------------------------------------------------------------------
        //  boost::crc_optimal doesn't have these.                              
        //----------------------------------------------------------------------
        crc_optimal(const crc_optimal& that);
        crc_optimal& operator=(const crc_optimal& that);

}; // class CRC //

typedef crc_optimal<boost::crc_32_type> crc_32_type;

} // namespace lib

#endif // #ifndef LIB_CRC_H_FILE_GUARD

