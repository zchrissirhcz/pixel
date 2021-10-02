#include "vqdmulh.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmulh_type
//----------------------------------------------------------------------
int16x4_t vqdmulh_s16(int16x4_t M, int16x4_t M)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = ( (N[i] * M[i]) * 2 ) >> 8;
    }
    return D;
}

} // namespace pxl