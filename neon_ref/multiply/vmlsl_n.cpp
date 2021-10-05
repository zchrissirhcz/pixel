#include "vmlsl_n.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmlsl_n_type
//----------------------------------------------------------------------
int32x4_t vmlsl_n_s16(int32x4_t a, int16x4_t b, int16_t c)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] - (b[i] * c);
    }
    return D;
}

int64x2_t vmlsl_n_s32(int64x2_t a, int32x2_t b, int32_t c)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] - (b[i] * c);
    }
    return D;
}

uint32x4_t vmlsl_n_u16(uint32x4_t a, uint16x4_t b, uint16_t c)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] - (b[i] * c);
    }
    return D;
}

uint64x2_t vmlsl_n_u32(uint64x2_t a, uint32x2_t b, uint32_t c)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] - (b[i] * c);
    }
    return D;
}

} // namespace pxl