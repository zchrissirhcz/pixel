#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmull_lane_type
//----------------------------------------------------------------------
int32x4_t vqdmull_lane_s16(int16x4_t M, int16x4_t N, const int lane);
int64x2_t vqdmull_lane_s32(int32x2_t M, int32x2_t N, const int lane);

//----------------------------------------------------------------------
// vqdmull_laneq_type
//----------------------------------------------------------------------
int32x4_t vqdmull_laneq_s16(int16x4_t M, int16x8_t N, const int lane);
int64x2_t vqdmull_laneq_s32(int32x2_t M, int32x4_t N, const int lane);

} // namespace pxl