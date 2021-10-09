#include "vqabs.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqabs_type
//----------------------------------------------------------------------
int8x8_t vqabs_s8(int8x8_t N)
{
    int8x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]==INT8_MIN)
        {
            D[i] = INT8_MAX;
        }
        else
        {
            D[i] = abs(N[i]);
        }
    }
    return D;
}

int16x4_t vqabs_s16(int16x4_t N)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]==INT16_MIN)
        {
            D[i] = INT16_MAX;
        }
        else
        {
            D[i] = abs(N[i]);
        }
    }
    return D;
}

int32x2_t vqabs_s32(int32x2_t N)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (N[i]==INT32_MIN)
        {
            D[i] = INT32_MAX;
        }
        else
        {
            D[i] = abs(N[i]);
        }
    }
    return D;
}

//----------------------------------------------------------------------
// vqabsq_type
//----------------------------------------------------------------------
int8x16_t vqabsq_s8(int8x16_t N)
{
    int8x16_t D;
    for (int i=0; i<16; i++)
    {
        if (N[i]==INT8_MIN)
        {
            D[i] = INT8_MAX;
        }
        else
        {
            D[i] = abs(N[i]);
        }
    }
    return D;
}

int16x8_t vqabsq_s16(int16x8_t N)
{
    int16x8_t D;
    for (int i=0; i<8; i++)
    {
        if (N[i]==INT16_MIN)
        {
            D[i] = INT16_MAX;
        }
        else
        {
            D[i] = abs(N[i]);
        }
    }
    return D;
}

int32x4_t vqabsq_s32(int32x4_t N)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (N[i]==INT32_MIN)
        {
            D[i] = INT32_MAX;
        }
        else
        {
            D[i] = abs(N[i]);
        }
    }
    return D;
}

} // namespace pxl