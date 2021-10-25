#include "vrnd.hpp"

#include <limits.h>

namespace pxl {

// vrnd means vector rounding to zero
// 转为最接近的整数， 舍入方向是离0最近

//----------------------------------------------------------------------
// vrnd_type
//----------------------------------------------------------------------
float32x2_t vrnd_f32(float32x2_t a)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        if (a[i] > INT_MAX) {
            D[i] = INT_MAX;
        }
        else if (a[i] < INT_MIN)
        {
            D[i] = INT_MIN;
        }
        else {
            D[i] = (int)a[i];
        }
    }
    return D;
}

float64x1_t vrnd_f64(float64x1_t a)
{
    float64x1_t D;
    for (int i=0; i<1; i++)
    {
        if (a[i] > INT64_MAX) {
            D[i] = INT64_MAX;
        }
        else if (a[i] < INT64_MIN)
        {
            D[i] = INT64_MIN;
        }
        else {
            D[i] = (int64_t)a[i];
        }
    }
    return D;
}

//float16x4_t vrnd_f16(float16x4_t a);

//----------------------------------------------------------------------
// vrndq_type
//----------------------------------------------------------------------
float32x4_t vrndq_f32(float32x4_t a)
{
    float32x4_t D;
    for (int i=0; i<4; i++)
    {
        if (a[i] > INT_MAX) {
            D[i] = INT_MAX;
        }
        else if (a[i] < INT_MIN)
        {
            D[i] = INT_MIN;
        }
        else {
            D[i] = (int)a[i];
        }
    }
    return D;
}

float64x2_t vrndq_f64(float64x2_t a)
{
    float64x2_t D;
    for (int i=0; i<2; i++)
    {
        if (a[i] > INT64_MAX) {
            D[i] = INT64_MAX;
        }
        else if (a[i] < INT64_MIN)
        {
            D[i] = INT64_MIN;
        }
        else {
            D[i] = (int64_t)a[i];
        }
    }
    return D;
}

//float16x8_t vrndq_f16(float16x8_t a);

} // namespace pxl