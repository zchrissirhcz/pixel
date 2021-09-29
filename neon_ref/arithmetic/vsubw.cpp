#include "vsubw.hpp"

namespace pxl {

int16x8_t vsubw_s8(int16x8_t N, int8x8_t M)
{
    int16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N[i] - (int16_t)(M[i]);
    }
    return D;
}

int32x4_t vsubw_s16(int32x4_t N, int16x4_t M)
{
    int32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N[i] - (int32_t)(M[i]);
    }
    return D;
}

int64x2_t vsubw_s32(int64x2_t N, int32x2_t M)
{
    int64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N[i] - (int64_t)(M[i]);
    }
    return D;
}


uint16x8_t vsubw_u8(uint16x8_t N, uint8x8_t M)
{
    uint16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N[i] - (uint16_t)(M[i]);
    }
    return D;
}

uint32x4_t vsubw_u16(uint32x4_t N, uint16x4_t M)
{
    uint32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N[i] - (uint32_t)(M[i]);
    }
    return D;
}

uint64x2_t vsubw_u32(uint64x2_t N, uint32x2_t M)
{
    uint64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N[i] - (uint64_t)(M[i]);
    }
    return D;
}

} // namespace pxl