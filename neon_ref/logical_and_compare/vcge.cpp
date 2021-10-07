#include "vcge.hpp"

namespace pxl {

/*
vcge compares the value of each element in a vector with the value of the corresponding element
of a second vector, or zero. If they are equal, the corresponding element in the destination vector
is set to all ones. Otherwise, it is set to all zeros.

如果N[i]>=M[i]，那么D[i]的二进制是全1的
如果N[i]<M[i]，那么D[i]的二进制是全0的
*/

//----------------------------------------------------------------------
// vcge_type
//----------------------------------------------------------------------
uint8x8_t vcge_s8(int8x8_t N, int8x8_t M)
{
    uint8x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFF;
        }
    }
    return D;
}

uint16x4_t vcge_s16(int16x4_t N, int16x4_t M)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFF;
        }
    }
    return D;
}

uint32x2_t vcge_s32(int32x2_t N, int32x2_t M)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFFFFFF;
        }
    }
    return D;
}

uint8x8_t vcge_u8(uint8x8_t N, uint8x8_t M)
{
    uint8x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFF;
        }
    }
    return D;
}

uint16x4_t vcge_u16(uint16x4_t N, uint16x4_t M)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFF;
        }
    }
    return D;
}

uint32x2_t vcge_u32(uint32x2_t N, uint32x2_t M)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFFFFFF;
        }
    }
    return D;
}

uint32x2_t vcge_f32(float32x2_t N, float32x2_t M)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFFFFFF;
        }
    }
    return D;
}

//----------------------------------------------------------------------
// vcgeq_type
//----------------------------------------------------------------------
uint8x16_t vcgeq_s8(int8x16_t N, int8x16_t M)
{
    uint8x16_t D;
    for (int i=0; i<16; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFF;
        }
    }
    return D;
}

uint16x8_t vcgeq_s16(int16x8_t N, int16x8_t M)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFF;
        }
    }
    return D;
}

uint32x4_t vcgeq_s32(int32x4_t N, int32x4_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFFFFFF;
        }
    }
    return D;
}

uint8x16_t vcgeq_u8(uint8x16_t N, uint8x16_t M)
{
    uint8x16_t D;
    for (int i=0; i<16; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFF;
        }
    }
    return D;
}

uint16x8_t vcgeq_u16(uint16x8_t N, uint16x8_t M)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFF;
        }
    }
    return D;
}

uint32x4_t vcgeq_u32(uint32x4_t N, uint32x4_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFFFFFF;
        }
    }
    return D;
}

uint32x4_t vcgeq_f32(float32x4_t N, float32x4_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]>=M[i]) {
            D[i] = 0xFFFFFFFF;
        }
    }
    return D;
}


} // namespace pxl