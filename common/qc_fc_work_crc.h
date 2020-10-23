//------------------------------------------------------------------------------
///@file qc_fc_work_crc.h                                                       
///@par  Classification:  UNCLASSIFIED - OPEN SOURCE                                          
//------------------------------------------------------------------------------

#ifndef QC_FC_WORK_CRC_H_FILE_GUARD
#define QC_FC_WORK_CRC_H_FILE_GUARD

#include <boost/crc.hpp>
#include <stdint.h>

namespace qc {
namespace fc {
namespace work {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: CRC                                                              
///                                                                             
///@brief   Perform a quality check on Fibre Channel's CRC values.              
///                                                                             
///@par Purpose:                                                                
///         Read through the given data; when inside a fibre frame (after SOF   
///         and before EOF), compute the CRC and compare to the last value in   
///         the frame.                                                          
///                                                                             
///@par Thread Safety:  class                                                   
///                                                                             ///                                                                             
///@version
///     2020-10-23      PN          Open Source                                                                     
///     2010-05-05      DHF         File creation                               
//------------------------------------------------------------------------------
class CRC
{
    public:
        CRC();
        virtual ~CRC();

        void process(
            const uint32_t* words
          , int32_t wordCount
          , bool skipEveryOtherWord = false
        );

        int64_t frames() const;
        int64_t badCRCs() const;

    protected:

    private:
        //----------------------------------------------------------------------
        //  I don't need these ... do you?                                      
        //----------------------------------------------------------------------
        CRC(const CRC& that);
        CRC& operator=(const CRC& that);

        boost::crc_32_type  m_CRC;
        int64_t             m_CountOfFrames;
        int64_t             m_CountOfBadCRCs;
        bool                m_InFrame;

}; // class CRC //

} // namespace work
} // namespace fc
} // namespace qc

#endif // #ifndef QC_FC_WORK_CRC_H_FILE_GUARD

