#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmulh_lane_type
//----------------------------------------------------------------------
int16x4_t vqdmulh_n_s16(int16x4_t M, int16x4_t N, const int lane);
int32x2_t vqdmulh_n_s32(int32x2_t M, int32x2_t N, const int lane);

//----------------------------------------------------------------------
// vqdmulhq_lane_type
//----------------------------------------------------------------------
int16x8_t vqdmulhq_n_s16(int16x8_t M, int16x8_t N, const int lane);
int32x4_t vqdmulhq_n_s32(int32x4_t M, int32x4_t N, const int lane);

} // namespace pxl