#include "vrndn.hpp"
#include <math.h>
#include <limits.h>

namespace pxl {

// vrndn means vector rounding to nearest even number
// 转为最接近的偶数整数

//----------------------------------------------------------------------
// vrndn_type
//----------------------------------------------------------------------
float32x2_t vrndn_f32(float32x2_t a)
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
            int temp = a[i] + 0.5;
            //printf("%d %% 2=%d\n", temp, temp % 2);
            if (temp % 2==1 || temp % 2 ==-1)
            {
                temp--;
            }
            D[i] = temp;
        }
    }
    return D;
}

float64x1_t vrndn_f64(float64x1_t a)
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
            int64_t temp = a[i] + 0.5;
            //printf("%d %% 2=%d\n", temp, temp % 2);
            if (temp % 2==1 || temp % 2 ==-1)
            {
                temp--;
            }
            D[i] = temp;
        }
    }
    return D;
}

//float16x4_t vrndn_f16(float16x4_t a);

//----------------------------------------------------------------------
// vrndnq_type
//----------------------------------------------------------------------
float32x4_t vrndnq_f32(float32x4_t a)
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
            int temp = a[i] + 0.5;
            //printf("%d %% 2=%d\n", temp, temp % 2);
            if (temp % 2==1 || temp % 2 ==-1)
            {
                temp--;
            }
            D[i] = temp;
        }
    }
    return D;
}

float64x2_t vrndnq_f64(float64x2_t a)
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
            int64_t temp = a[i] + 0.5;
            //printf("%d %% 2=%d\n", temp, temp % 2);
            if (temp % 2==1 || temp % 2 ==-1)
            {
                temp--;
            }
            D[i] = temp;
        }
    }
    return D;
}

//float16x8_t vrndnq_f16(float16x8_t a);

//----------------------------------------------------------------------
// vrndnX_type
//----------------------------------------------------------------------
float32_t vrndns_f32(float32_t a)
{
    float32_t D;
    if (a > INT_MAX) {
        D = INT_MAX;
    }
    else if (a < INT_MIN)
    {
        D = INT_MIN;
    }
    else {
        int temp = a + 0.5;
        //printf("%d %% 2=%d\n", temp, temp % 2);
        if (temp % 2==1 || temp % 2 ==-1)
        {
            temp--;
        }
        D = temp;
    }
    return D;
}

//float16_t vrndnh_f16(float16_t a);


} // namespace pxl