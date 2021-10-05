#include "vmls.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmls_type
//----------------------------------------------------------------------
int8x8_t vmls_s8(int8x8_t N, int8x8_t M, int8x8_t P)
{
    int8x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

int16x4_t vmls_s16(int16x4_t N, int16x4_t M, int16x4_t P)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

int32x2_t vmls_s32(int32x2_t N, int32x2_t M, int32x2_t P)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

uint8x8_t vmls_u8(uint8x8_t N, uint8x8_t M, uint8x8_t P)
{
    uint8x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

uint16x4_t vmls_u16(uint16x4_t N, uint16x4_t M, uint16x4_t P)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

uint32x2_t vmls_u32(uint32x2_t N, uint32x2_t M, uint32x2_t P)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

float32x2_t vmls_f32(float32x2_t N, float32x2_t M, float32x2_t P)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

//----------------------------------------------------------------------
// vmlsq_type
//----------------------------------------------------------------------
int8x16_t vmlsq_s8(int8x16_t N, int8x16_t M, int8x16_t P)
{
    int8x16_t D;
    for (int i=0; i<16; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

int16x8_t vmlsq_s16(int16x8_t N, int16x8_t M, int16x8_t P)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

int32x4_t vmlsq_s32(int32x4_t N, int32x4_t M, int32x4_t P)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

uint8x16_t vmlsq_u8(uint8x16_t N, uint8x16_t M, uint8x16_t P)
{
    uint8x16_t D;
    for (int i=0; i<16; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

uint16x8_t vmlsq_u16(uint16x8_t N, uint16x8_t M, uint16x8_t P)
{
    uint16x8_t D;
    for (int i=0; i<8; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

uint32x4_t vmlsq_u32(uint32x4_t N, uint32x4_t M, uint32x4_t P)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

float32x4_t vmlsq_f32(float32x4_t N, float32x4_t M, float32x4_t P)
{
    float32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] - M[i] * P[i];
    }
    return D;
}

} // namespace pxl