#include "vqdmlal_lane.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlal_lane_type
//----------------------------------------------------------------------
int32x4_t vqdmlal_lane_s16(int32x4_t a, int16x4_t b, int16x4_t v, const int lane)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] + (b[i] * v[lane]) * 2;
    }
    return D;
}

int64x2_t vqdmlal_lane_s32(int64x2_t a, int32x2_t b, int32x2_t v, const int lane)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] + (b[i] * v[lane]) * 2;
    }
    return D;
}

//----------------------------------------------------------------------
// vqdmlal_laneq_type
//----------------------------------------------------------------------
int32x4_t vqdmlal_laneq_s16(int32x4_t a, int16x4_t b, int16x8_t v, const int lane)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] + (b[i] * v[lane]) * 2;
    }
    return D;
}

int64x2_t vqdmlal_laneq_s32(int64x2_t a, int32x2_t b, int32x4_t v, const int lane)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] + (b[i] * v[lane]) * 2;
    }
    return D;
}


} // namespace pxl