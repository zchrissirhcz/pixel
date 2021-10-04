#include "vqrdmulh_lane.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqrdmulh_lane_type
//----------------------------------------------------------------------
int16x4_t vqrdmulh_lan_s16(int16x4_t N, int16x4_t M, const int lane)
{
    int16x4_t D;
    const int32_t delta = 1<<15;
    for (int i=0; i<4; i++)
    {
        int32_t temp = N[i] * M[lane] * 2;
        D[i] = (temp + delta) >> 16;
    }
    return D;
}

int32x2_t vqrdmulh_lane_s32(int32x2_t N, int32x2_t M, const int lane)
{
    int32x2_t D;
    const int64_t delta = 1<<31;
    for (int i=0; i<2; i++)
    {
        int32_t temp = N[i] * M[lane] * 2;
        D[i] = (temp + delta) >> 32;
    }
    return D;
}

//----------------------------------------------------------------------
// vqrdmulhq_type
//----------------------------------------------------------------------
int16x8_t vqrdmulhq_lane_s16(int16x8_t N, int16x8_t M, const int lane)
{
    int16x8_t D;
    const int32_t delta = 1<<15;
    for (int i=0; i<8; i++)
    {
        int32_t temp = N[i] * M[lane] * 2;
        D[i] = (temp + delta) >> 16;
    }
    return D;
}

int32x4_t vqrdmulhq_lane_s32(int32x4_t N, int32x4_t M, const int lane)
{
    int32x4_t D;
    const int64_t delta = 1<<31;
    for (int i=0; i<4; i++)
    {
        int32_t temp = N[i] * M[lane] * 2;
        D[i] = (temp + delta) >> 32;
    }
    return D;
}

} // namespace pxl