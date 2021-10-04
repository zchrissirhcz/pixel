#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqrdmulh_lane_type
//----------------------------------------------------------------------
int16x4_t vqrdmulh_lane_s16(int16x4_t N, int16x4_t M, const int lane);
int32x2_t vqrdmulh_lane_s32(int32x2_t N, int32x2_t M, const int lane);

//----------------------------------------------------------------------
// vqrdmulhq_lane_type
//----------------------------------------------------------------------
int16x8_t vqrdmulhq_lane_s16(int16x8_t N, int16x8_t M, const int lane);
int32x4_t vqrdmulhq_lane_s32(int32x4_t N, int32x4_t M, const int lane);

} // namespace pxl