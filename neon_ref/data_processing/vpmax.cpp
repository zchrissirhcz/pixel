#include "vpmax.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vpmax_type
//----------------------------------------------------------------------

// vpmax means compare adjacent Pairs and copied the larger one to the destination
// vpmax 比较相邻的一对元素，取其中较大的那个作为结果

int8x8_t vpmax_s8(int8x8_t N, int8x8_t M)
{
    int8x8_t D;
    int i=0, j=0;
    for (i=0; i<8; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<8; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}

int16x4_t vpmax_s16(int16x4_t N, int16x4_t M)
{
    int16x4_t D;
    int i=0, j=0;
    for (i=0; i<4; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<4; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}

int32x2_t vpmax_s32(int32x2_t N, int32x2_t M)
{
    int32x2_t D;
    int i=0, j=0;
    for (i=0; i<2; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<2; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}

uint8x8_t vpmax_u8(uint8x8_t N, uint8x8_t M)
{
    uint8x8_t D;
    int i=0, j=0;
    for (i=0; i<8; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<8; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}

uint16x4_t vpmax_u16(uint16x4_t N, uint16x4_t M)
{
    uint16x4_t D;
    int i=0, j=0;
    for (i=0; i<4; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<4; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}

uint32x2_t vpmax_u32(uint32x2_t N, uint32x2_t M)
{
    uint32x2_t D;
    int i=0, j=0;
    for (i=0; i<2; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<2; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}

float32x2_t vpmax_u32(float32x2_t N, float32x2_t M)
{
    float32x2_t D;
    int i=0, j=0;
    for (i=0; i<2; i+=2)
    {
        D[j] = N[i] > N[i+1] ? N[i] : N[i+1];
        j++;
    }
    for (i=0; i<2; i+=2)
    {
        D[j] = M[i] > M[i+1] ? M[i] : M[i+1];
        j++;
    }
    return D;
}


} // namespace pxl