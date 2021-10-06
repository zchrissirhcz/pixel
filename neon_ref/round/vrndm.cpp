#include "vrndm.hpp"

namespace pxl {

// vrndm means vector rounding to Minus infinity.

//----------------------------------------------------------------------
// vrndm_type
//----------------------------------------------------------------------
float32x2_t vrndm_f32(float32x2_t a)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (a[i] == (int64_t)a[i])
        {
            D[i] = a[i];
        }
        else {
            D[i] = (int64_t)(a[i] - 0.5);
        }
    }
    return D;
}


float64x1_t vrndm_f64(float64x1_t a)
{
    float64x1_t D;
    for (int i=0; i<1; i++)
    {
        if (a[i] == (int64_t)a[i])
        {
            D[i] = a[i];
        }
        else {
            D[i] = (int64_t)(a[i] - 0.5);
        }
    }
    return D;
}

//float16x4_t vrndm_f16(float16x4_t a);

//----------------------------------------------------------------------
// vrndmq_type
//----------------------------------------------------------------------
float32x4_t vrndmq_f32(float32x4_t a)
{
    float32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (a[i] == (int64_t)a[i])
        {
            D[i] = a[i];
        }
        else {
            D[i] = (int64_t)(a[i] - 0.5);
        }
    }
    return D;
}

float64x2_t vrndmq_f64(float64x2_t a)
{
    float64x2_t D;
    for (int i=0; i<2; i++)
    {
        if (a[i] == (int64_t)a[i])
        {
            D[i] = a[i];
        }
        else {
            D[i] = (int64_t)(a[i] - 0.5);
        }
    }
    return D;
}

//float16x8_t vrndmq_f16(float16x8_t a);

//----------------------------------------------------------------------
// vrndmh_type
//----------------------------------------------------------------------
//float16_t vrndmh_f16(float16_t a);

} // namespace pxl