// template<typename Result_t, typename Vector1_t, typename Vector2_t=Vector1_t>
// Result_t vadd_type(Vector1_t N, Vector2_t M)
// {
//     Result_t D;
//     for (int i..len)
//         D[i] = N[i] + M[i];
// }

#include "vadd.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vadd_type
//----------------------------------------------------------------------
int8x8_t vadd_s8(int8x8_t N, int8x8_t M)
{
    int8x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

int16x4_t vadd_s16(int16x4_t N, int16x4_t M)
{
    int16x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}


int32x2_t vadd_s32(int32x2_t N, int32x2_t M)
{
    int32x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

int64x1_t vadd_s64(int64x1_t N, int64x1_t M)
{
    int64x1_t D;
    D[0] = N[0] + M[0];
    return D;
}


uint8x8_t vadd_u8(uint8x8_t N, uint8x8_t M)
{
    uint8x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint16x4_t vadd_u16(uint16x4_t N, uint16x4_t M)
{
    uint16x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint32x2_t vadd_u32(uint32x2_t N, uint32x2_t M)
{
    uint32x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint64x1_t vadd_u64(uint64x1_t N, uint64x1_t M)
{
    uint64x1_t D;
    D[0] = N[0] + M[0];
    return D;
}

//----------------------------------------------------------------------
// vaddq_type
//----------------------------------------------------------------------
int8x16_t vaddq_s8(int8x16_t N, int8x16_t M)
{
    int8x16_t D;
    for (size_t i=0; i<16; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

int16x8_t vaddq_s16(int16x8_t N, int16x8_t M)
{
    int16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

int32x4_t vaddq_s32(int32x4_t N, int32x4_t M)
{
    int32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

int64x2_t vaddq_s64(int64x2_t N, int64x2_t M)
{
    int64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint8x16_t vaddq_u8(uint8x16_t N, uint8x16_t M)
{
    uint8x16_t D;
    for (size_t i=0; i<16; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint16x8_t vaddq_u16(uint16x8_t N, uint16x8_t M)
{
    uint16x8_t D;
    for (size_t i=0; i<8; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint32x4_t vaddq_u32(uint32x4_t N, uint32x4_t M)
{
    uint32x4_t D;
    for (size_t i=0; i<4; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

uint64x2_t vaddq_u64(uint64x2_t N, uint64x2_t M)
{
    uint64x2_t D;
    for (size_t i=0; i<2; i++)
    {
        D[i] = N[i] + M[i];
    }
    return D;
}

} // namespace pxl