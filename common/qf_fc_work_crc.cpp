//------------------------------------------------------------------------------
///@file qc_fc_work_crc.cpp                                                     
///@par  Classification:  UNCLASSIFIED - OPEN SOURCE                                                                            
///                                                                             
///@brief   Perform a quality check on Fibre Channel's CRC values.              
///                                                                             
///@par Purpose:                                                                
///         Read through the given data; when inside a fibre frame (after SOF   
///         and before EOF), compute the CRC and compare to the last value in   
///         the frame.                                                          
///                                                                             
///                                                                             
///@version
///     2020-10-23      PN          Open Source                                                                     
///     2010-05-05      DHF         File creation                               
//------------------------------------------------------------------------------

#include "qc_fc_work_crc.h"

namespace qc {
namespace fc {
namespace work {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CRC::CRC()
    : m_CountOfFrames(0)
    , m_CountOfBadCRCs(0)
    , m_InFrame(false)
{
} // CRC::CRC() //

//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
CRC::~CRC()
{

} // CRC::~CRC

//------------------------------------------------------------------------------
///@brief   Process the given FC data; only the words between a SOF and EOF     
///         are processed.                                                      
///@param   words       The FC words read from the data file.                   
///@param   wordCount   Number of 32-bit words pointed to by words.             
//------------------------------------------------------------------------------
void CRC::process(
    const uint32_t* words
  , int32_t wordCount
  , bool skipEveryOtherWord
)
{
    int increment = skipEveryOtherWord ? 2 : 1;

    for (int w=0; w < wordCount; w += increment) {
        
    }

} // void CRC::process(int32_t* words, int32_t wordCount) //

//------------------------------------------------------------------------------
///@brief   Return the number of frames processed; a frame is not considered    
///         processed until the EOF is encountered.                             
//------------------------------------------------------------------------------
int64_t CRC::frames() const
{
    return m_CountOfFrames;
} // int64_t CRC::frames() const //

//------------------------------------------------------------------------------
///@brief   Return the number of bad CRCs found.                                
//------------------------------------------------------------------------------
int64_t CRC::badCRCs() const
{
    return m_CountOfBadCRCs;
} // int64_t CRC::badCRCs() const //

} // namespace work //
} // namespace fc //
} // namespace qc //

