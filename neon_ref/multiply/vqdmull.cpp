#include "vqdmull.hpp"

namespace pxl {

int32x4_t vqdmull_s16(int16x4_t M, int16x4_t N)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = (M[i] * N[i])*2;
    }
    return D;
}

int64x2_t vqdmull_s32(int32x2_t M, int32x2_t N)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = (M[i] * N[i])*2;
    }
    return D;
}

} // namespace pxl