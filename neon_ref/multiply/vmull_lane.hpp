#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmull_lane_type
//----------------------------------------------------------------------
int32x4_t vmull_lane_s16(int16x4_t N, int16x4_t M, const int lane);
int64x2_t vmull_lane_s32(int32x2_t N, int32x2_t M, const int lane);
uint32x4_t vmull_lane_s16(uint16x4_t N, uint16x4_t M, const int lane);
uint64x2_t vmull_lane_s32(uint32x2_t N, uint32x2_t M, const int lane);

//----------------------------------------------------------------------
// vmull_laneq_type
//----------------------------------------------------------------------
int32x4_t vmull_laneq_s16(int16x4_t N, int16x8_t M, const int lane);
int64x2_t vmull_laneq_s32(int32x2_t N, int32x4_t M, const int lane);
uint32x4_t vmull_laneq_u16(uint16x4_t N, uint16x8_t M, const int lane);
uint64x2_t vmull_laneq_u32(uint32x2_t N, uint32x4_t M, const int lane);

} // namespace pxl