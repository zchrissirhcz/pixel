#include "vaddl.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vaddw_type
//----------------------------------------------------------------------
int8x8_t vdup_n_s8(int8_t N)
{
    int8x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N;
    }
    return D;
}

int16x4_t vdup_n_s16(int16_t N)
{
    int16x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N;
    }
    return D;
}

int32x2_t vdup_n_s32(int32_t N)
{
    int32x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N;
    }
    return D;
}

int64x1_t vdup_n_s64(int64_t N)
{
    int64x1_t D;
    for (size_t i=0; i<1; i++)
    {
        D[i] = N;
    }
    return D;
}

uint8x8_t vdup_n_u8(uint8_t N)
{
    uint8x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N;
    }
    return D;
}

uint16x4_t vdup_n_u16(uint16_t N)
{
    uint16x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N;
    }
    return D;
}

uint32x2_t vdup_n_u32(uint32_t N)
{
    uint32x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N;
    }
    return D;
}

uint64x1_t vdup_n_u64(uint64_t N)
{
    uint64x1_t D;
    for (size_t i=0; i<1; i++)
    {
        D[i] = N;
    }
    return D;
}


//----------------------------------------------------------------------
// vdupq_n_type
//----------------------------------------------------------------------

int8x16_t vdupq_n_s8(int8_t N)
{
    int8x16_t D;
    for (size_t i=0; i<16; i++)
    {
        D[i] = N;
    }
    return D;
}

int16x8_t vdupq_n_s16(int16_t N)
{
    int16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N;
    }
    return D;
}

int32x4_t vdupq_n_s32(int32_t N)
{
    int32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N;
    }
    return D;
}

int64x2_t vdupq_n_s64(int64_t N)
{
    int64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N;
    }
    return D;
}

uint8x16_t vdupq_n_u8(uint8_t N)
{
    uint8x16_t D;
    for (size_t i=0; i<16; i++)
    {
        D[i] = N;
    }
    return D;
}

uint16x8_t vdupq_n_u16(uint16_t N)
{
    uint16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N;
    }
    return D;
}

uint32x4_t vdupq_n_u32(uint32_t N)
{
    uint32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N;
    }
    return D;
}

uint64x2_t vdupq_n_u64(uint64_t N)
{
    uint64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N;
    }
    return D;
}


} // namespace pxl