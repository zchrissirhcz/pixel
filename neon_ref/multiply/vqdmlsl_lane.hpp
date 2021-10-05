#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlsl_lane_type
//----------------------------------------------------------------------
int32x4_t vqdmlsl_lane_s16(int32x4_t a, int16x4_t b, int16x4_t v, const int lane);
int64x2_t vqdmlsl_lane_s32(int64x2_t a, int32x2_t b, int32x2_t v, const int lane);

//----------------------------------------------------------------------
// vqdmlsl_laneq_type
//----------------------------------------------------------------------
int32x4_t vqdmlsl_laneq_s16(int32x4_t a, int16x4_t b, int16x8_t v, const int lane);
int64x2_t vqdmlsl_laneq_s32(int64x2_t a, int32x2_t b, int32x4_t v, const int lane);


} // namespace pxl