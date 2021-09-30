#include "vmull_lane.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmull_lane_type
//----------------------------------------------------------------------
int32x4_t vmull_lane_s16(int16x4_t N, int16x4_t M, const int lane)
{
    int32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] * M[lane];
    }
    return D;
}

int64x2_t vmull_lane_s32(int32x2_t N, int32x2_t M, const int lane)
{
    int64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] * M[lane];
    }
    return D;
}

uint32x4_t vmull_lane_s16(uint16x4_t N, uint16x4_t M, const int lane)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        D[i] = N[i] * M[lane];
    }
    return D;
}

uint64x2_t vmull_lane_s32(uint32x2_t N, uint32x2_t M, const int lane)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        D[i] = N[i] * M[lane];
    }
    return D;
}

} // namespace pxl