#include "vtst.hpp"

namespace pxl {

/*
如果N[i]&M[i]!=0，那么D[i]的二进制是全1的
如果N[i]&M[i]==0，那么D[i]的二进制是全0的
*/

//----------------------------------------------------------------------
// vtst_type
//----------------------------------------------------------------------
uint8x8_t vtst_s8(int8x8_t N, int8x8_t M)
{
    uint8x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint16x4_t vtst_s16(int16x4_t N, int16x4_t M)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint32x2_t vtst_s32(int32x2_t N, int32x2_t M)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint8x8_t vtst_u8(uint8x8_t N, uint8x8_t M)
{
    uint8x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint16x4_t vtst_u16(uint16x4_t N, uint16x4_t M)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint32x2_t vtst_u32(uint32x2_t N, uint32x2_t M)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

//----------------------------------------------------------------------
// vtstq_type
//----------------------------------------------------------------------
uint8x16_t vtstq_s8(int8x16_t N, int8x16_t M)
{
    uint8x16_t D;
    for (int i=0; i<16; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint16x8_t vtstq_s16(int16x8_t N, int16x8_t M)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint32x4_t vtstq_s32(int32x4_t N, int32x4_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint8x16_t vtstq_u8(uint8x16_t N, uint8x16_t M)
{
    uint8x16_t D;
    for (int i=0; i<16; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint16x8_t vtstq_u16(uint16x8_t N, uint16x8_t M)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint32x4_t vtstq_u32(uint32x4_t N, uint32x4_t M)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]&M[i]) {
            D[i] = 0xFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}


} // namespace pxl