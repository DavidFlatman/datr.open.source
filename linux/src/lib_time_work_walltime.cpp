//------------------------------------------------------------------------------
///@file lib_time_work_walltime.cpp                                             
///                                                                             
///@par Classification:  UNCLASSIFIED, OPEN SOURCE                              
///                                                                             
///@brief   Determine the wall time between object contruction and destruction. 
///                                                                             
///@par Public Domain                                                           
///         Portions of the software resulted from work developed under a       
///         U.S. Government contract and are subject to the following license:  
///         the Government is granted for iteself and others working on its     
///         behalf a paid-up, nonexclusive, irrevocable, worldwide license in   
///         this software to reproduce, prepare derivative works, and perform   
///         publicly and display publicly.                                      
///                                                                             
///@par As Is:                                                                  
///         THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
///         IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED      
///         WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  
///         ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY     
///         DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL  
///         DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   
///         GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS       
///         INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
///         IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR     
///         OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN 
///         IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                       
///                                                                             
///                                                                             
///@par Quote:                                                                  
///         As you slide down the banister of life, remember me as a splinter   
///         along the way.                                                      
///                                                                   -- unknown
///                                                                             
///                                                                             
///@version 2020-05-04  DHF     Open sourced                                    
///                                                                             
///@version 2015-08-02  DHF     DeltaTime::seconds renamed inSeconds.           
///                                                                             
///@version 2011-01-24  DHF     File creation                                   
///                                                                             
//------------------------------------------------------------------------------

#include "lib_time_work_walltime.h"
#include "lib_time_work_deltatime.h"

namespace lib {
namespace time {
namespace work {


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
WallTime::WallTime()
    : m_StartTime(lib::time::work::DateTime::now())
{
} // WallTime::WallTime(const std::string& message) //


//------------------------------------------------------------------------------
///@brief   Reclaim resources held by object.                                   
//------------------------------------------------------------------------------
WallTime::~WallTime()
{

} // WallTime::~WallTime


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WallTime::write(
    std::ostream& out
  , const std::string& message
  , bool performReset
)
{
    lib::time::work::DeltaTime diff(lib::time::work::DateTime::now() - m_StartTime);

    out << diff.toString() << " " << message << std::endl;

    if (performReset) reset();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void WallTime::reset()
{
    m_StartTime = lib::time::work::DateTime::now();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
std::string WallTime::toString() const
{
    return (lib::time::work::DateTime::now() - m_StartTime).toString();
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
double WallTime::seconds() const
{
    return (lib::time::work::DateTime::now() - m_StartTime).inSeconds();
}

} // namespace work //
} // namespace time //
} // namespace lib //


