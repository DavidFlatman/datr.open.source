//------------------------------------------------------------------------------
//UNCLASSIFIED, OPEN SOURCE
///@file lib_time_walltime.h                                                    
//------------------------------------------------------------------------------

#ifndef LIB_TIME_WALLTIME_H_FILE_GUARD
#define LIB_TIME_WALLTIME_H_FILE_GUARD

#include <boost/scoped_ptr.hpp>
#include <iostream>
#include <string>

#include "lib_time_work_datetime.h"

namespace lib {
namespace time {

//------------------------------------------------------------------------------
///                                                                             
///@par Class: WallTime                                                         
///                                                                             
///@brief   Determine the wall time between object contruction and destruction. 
///                                                                             
///@par Thread Safety:  unknown none class object                               
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
///@par Quote:                                                                  
///         I don't want to bring them to justice. I want to defeat them.       
///                    -- Former Speaker Newt Gingrich on how we should deal    
///                       with terrorists behind Tuesday's attacks, FoxNews,    
///                       9/11/01                                               
///                                                                             
///                                                                             
///                                                                             
///@version 2011-01-24  DHF         File creation                               
///                                                                             
//------------------------------------------------------------------------------
class WallTime
{
    public:
        WallTime();
        virtual ~WallTime();

        void reset();
        void write(
            std::ostream& out
          , const std::string& message = ""
          , bool performReset = false
        );

        std::string toString() const;
        double seconds() const;

        inline void operator()(
            std::ostream& out
          , const std::string& message = ""
          , bool performReset = false
        ) { write(out, message, performReset); }

    protected:

    private:
        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        WallTime(const WallTime& that);
        WallTime& operator=(const WallTime& that);

        lib::time::work::DateTime   m_StartTime;

}; // class WallTime //

} // namespace time
} // namespace lib


#endif // #ifndef LIB_TIME_WALLTIME_H_FILE_GUARD

