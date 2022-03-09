#include "vqdmlal_n.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlal_n_type
//----------------------------------------------------------------------
int32x4_t vqdmlal_n_s16(int32x4_t a, int16x4_t b, int16_t c)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        //TODO: saturate the result
        D[i] += a[i] + (b[i] * c) * 2;
    }
    return D;
}

int64x2_t vqdmlal_n_s32(int64x2_t a, int32x2_t b, int32_t c)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        //TODO: saturate the result
        D[i] = a[i] + (b[i] * c) * 2;
    }
    return D;
}

} // namespace pxl