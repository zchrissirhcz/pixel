#include "vmlsl.hpp"

namespace pxl {

int16x8_t vmlsl_s8(int16x8_t N, int8x8_t M, int8x8_t P)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] - (M[i] * P[i]);
    }
    return D;
}

int32x4_t vmlsl_s16(int32x4_t N, int16x4_t M, int16x4_t P)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - (M[i] * P[i]);
    }
    return D;
}

int64x2_t vmlsl_s32(int64x2_t N, int32x2_t M, int32x2_t P)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] - (M[i] * P[i]);
    }
    return D;
}

uint16x8_t vmlsl_u8(uint16x8_t N, uint8x8_t M, uint8x8_t P)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] - (M[i] * P[i]);
    }
    return D;
}

uint32x4_t vmlsl_u16(uint32x4_t N, uint16x4_t M, uint16x4_t P)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - (M[i] * P[i]);
    }
    return D;
}

uint64x2_t vmlsl_u32(uint64x2_t N, uint32x2_t M, uint32x2_t P)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] - (M[i] * P[i]);
    }
    return D;
}


} // namespace pxl