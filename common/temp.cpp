#include <stdint.h>
#include "debug.h"

namespace lib {
namespace ieee {
namespace work {

union Double64
{
    double  m_Double;

    struct 
    {
        #ifdef HIGH_BIT_FIRST
            uint64_t    m_IsNegative:1;
            uint64_t    m_Exponent:11;
            uint64_t    m_Mantissa0:20;
            uint64_t    m_Mantissa1:32;
        #else
            uint64_t    m_Mantissa:52;
            uint64_t    m_Exponent:11;
            uint64_t    m_IsNegative:1;
        #endif
    };
};

} // namespace work 
} // namespace ieee 
} // namespace lib 

int main(int argc, char* argv[])
{
    lib::ieee::work::Double64   d;

    d.m_Double = 3;

    DEBUGVAR(sizeof(double));
    DEBUGVAR(d.m_Double);
    DEBUGVAR(d.m_IsNegative);
    DEBUGVAR(d.m_Exponent);
    DEBUGVAR(HEX(d.m_Mantissa));
}

