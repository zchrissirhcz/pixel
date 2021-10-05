#include "vqdmlsl.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlsl_type
//----------------------------------------------------------------------
int32x4_t vqdmlsl_s16(int32x4_t N, int16x4_t M, int16x4_t P)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - (M[i] * P[i]) * 2;
    }
    return D;
}

int64x2_t vqdmlsl_s32(int64x2_t N, int32x2_t M, int32x2_t P)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] - (M[i] * P[i]) * 2;
    }
    return D;
}


} // namespace pxl