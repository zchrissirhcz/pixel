#include "vrsubhn.hpp"

namespace pxl {

int8x8_t vrsubhn_s16(int16x8_t N, int16x8_t M)
{
    int8x8_t D;
    const int16_t shift = 8;
    const int16_t delta = (1<<(shift-1));
    for (int i=0; i<8; i++)
    {
        D[i] = (N[i] - M[i] + delta) >> shift;
    }
    return D;
}

int16x4_t vrsubhn_s32(int32x4_t N, int32x4_t M)
{
    int16x4_t D;
    const int32_t shift = 16;
    const int32_t delta = (1<<(shift-1));
    for (int i=0; i<4; i++)
    {
        D[i] = (N[i] - M[i] + delta) >> shift;
    }
    return D;
}

int32x2_t vrsubhn_s64(int64x2_t N, int64x2_t M)
{
    int32x2_t D;
    const int64_t shift = 32;
    const int64_t delta = (1<<(shift-1));
    for (int i=0; i<2; i++)
    {
        D[i] = (N[i] - M[i] + delta) >> shift;
    }
    return D;
}

uint8x8_t vrsubhn_u16(uint16x8_t N, uint16x8_t M)
{
    uint8x8_t D;
    const uint16_t shift = 8;
    uint16_t delta = (1<<(shift-1));
    for (int i=0; i<8; i++)
    {
        D[i] = (N[i] - M[i] + delta) >> shift;
    }
    return D;
}

uint16x4_t vrsubhn_u32(uint32x4_t N, uint32x4_t M)
{
    uint16x4_t D;
    const uint32_t shift = 16;
    uint32_t delta = (1<<(shift-1));
    for (int i=0; i<4; i++)
    {
        D[i] = (N[i] - M[i] + delta) >> shift;
    }
    return D;
}

uint32x2_t vrsubhn_u64(uint64x2_t N, uint64x2_t M)
{
    uint32x2_t D;
    const uint64_t shift = 32;
    uint64_t delta = (1<<(shift-1));
    for (int i=0; i<2; i++)
    {
        D[i] = (N[i] - M[i] + delta) >> shift;
    }
    return D;
}

} // namespace pxl