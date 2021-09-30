#include "vmul_lane.hpp"

namespace pxl {

// 注：在 2_DEN0018A_neon_programmers_guide_en.pdf 中没有找到 vmul_type_type 的相关 API
// 如下API是在 https://developer.arm.com/architectures/instruction-sets/intrinsics/ 找到的

//----------------------------------------------------------------------
// vmul_lane_type
//----------------------------------------------------------------------
int16x4_t   vmul_lane_s16   (int16x4_t a, int16x4_t v, const int lane)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

int32x2_t   vmul_lane_s32   (int32x2_t a, int32x2_t v, const int lane)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

uint16x4_t  vmul_lane_u16   (uint16x4_t a, uint16x4_t v, const int lane)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

uint32x2_t  vmul_lane_u32   (uint32x2_t a, uint32x2_t v, const int lane)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

float32x2_t vmul_lane_f32   (float32x2_t a, float32x2_t v, const int lane)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

float64x1_t vmul_lane_f64   (float64x1_t a, float64x1_t v, const int lane)
{
    float64x1_t D;
    for (int i=0; i<1; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

//float16x4_t vmul_lane_f16   (float16x4_t a, float16x4_t v, const int lane);

//----------------------------------------------------------------------
// vmul_laneq_type
//----------------------------------------------------------------------
int16x4_t   vmul_laneq_s16  (int16x4_t a, int16x8_t v, const int lane)
{
    int16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

int32x2_t   vmul_laneq_s32  (int32x2_t a, int32x4_t v, const int lane)
{
    int32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

uint16x4_t  vmul_laneq_u16  (uint16x4_t a, uint16x8_t v, const int lane)
{
    uint16x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}


uint32x2_t  vmul_laneq_u32  (uint32x2_t a, uint32x4_t v, const int lane)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

float32x2_t vmul_laneq_f32  (float32x2_t a, float32x4_t v, const int lane)
{
    float32x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

float64x1_t vmul_laneq_f64  (float64x1_t a, float64x2_t v, const int lane)
{
    float64x1_t D;
    for (int i=0; i<1; i++)
    {
        D[i] = a[i] * v[lane];
    }
    return D;
}

//float16x4_t vmul_laneq_f16  (float16x4_t a, float16x8_t v, const int lane);

} // namespace pxl