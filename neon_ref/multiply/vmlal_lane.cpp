#include "vmlal_lane.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmlal_lane_type
//----------------------------------------------------------------------
int32x4_t vmlal_lane_s16(int32x4_t a, int16x4_t b, int16x4_t v, const int lane)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

int64x2_t vmlal_lane_s32(int64x2_t a, int32x2_t b, int32x2_t v, const int lane)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

uint32x4_t vmlal_lane_u16(uint32x4_t a, uint16x4_t b, uint16x4_t v, const int lane)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

uint64x2_t vmlal_lane_u32(uint64x2_t a, uint32x2_t b, uint32x2_t v, const int lane)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

//----------------------------------------------------------------------
// vmlal_laneq_type
//----------------------------------------------------------------------
int32x4_t vmlal_laneq_s16(int32x4_t a, int16x4_t b, int16x8_t v, const int lane)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

int64x2_t vmlal_laneq_s32(int64x2_t a, int32x2_t b, int32x4_t v, const int lane)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

uint32x4_t vmlal_laneq_u16(uint32x4_t a, uint16x4_t b, uint16x8_t v, const int lane)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

uint64x2_t vmlal_laneq_u32(uint64x2_t a, uint32x2_t b, uint32x4_t v, const int lane)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = a[i] + b[i] * v[lane];
    }
    return D;
}

} // namespace pxl