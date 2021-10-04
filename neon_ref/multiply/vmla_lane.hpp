#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmla_lane_type
//----------------------------------------------------------------------
int16x4_t vmla_lane_s16(int16x4_t a, int16x4_t b, int16x4_t v, const int lane);
int32x2_t vmla_lane_s32(int32x2_t a, int32x2_t b, int32x2_t v, const int lane);
uint16x4_t vmla_lane_u16(uint16x4_t a, uint16x4_t b, uint16x4_t v, const int lane);
uint32x2_t vmla_lane_u32(uint32x2_t a, uint32x2_t b, uint32x2_t v, const int lane);
float32x2_t vmla_lane_f32(float32x2_t a, float32x2_t b, float32x2_t v, const int lane);

//----------------------------------------------------------------------
// vmla_laneq_type
//----------------------------------------------------------------------
int16x4_t vmla_laneq_s16(int16x4_t a, int16x4_t b, int16x8_t v, const int lane);
int32x2_t vmla_laneq_s32(int32x2_t a, int32x2_t b, int32x4_t v, const int lane);
uint16x4_t vmla_laneq_u16(uint16x4_t a, uint16x4_t b, uint16x8_t v, const int lane);
uint32x2_t vmla_laneq_u32(uint32x2_t a, uint32x2_t b, uint32x4_t v, const int lane);
float32x2_t vmla_laneq_f32(float32x2_t a, float32x2_t b, float32x4_t v, const int lane);

//----------------------------------------------------------------------
// vmlaq_lane_type
//----------------------------------------------------------------------
int16x8_t vmlaq_lane_s16(int16x8_t a, int16x8_t b, int16x4_t v, const int lane);
int32x4_t vmlaq_lane_s32(int32x4_t a, int32x4_t b, int32x2_t v, const int lane);
uint16x8_t vmlaq_lane_u16(uint16x8_t a, uint16x8_t b, uint16x4_t v, const int lane);
uint32x4_t vmlaq_lane_u32(uint32x4_t a, uint32x4_t b, uint32x2_t v, const int lane);
float32x4_t vmlaq_lane_f32(float32x4_t a, float32x4_t b, float32x2_t v, const int lane);

//----------------------------------------------------------------------
// vmlaq_laneq_type
//----------------------------------------------------------------------
int16x8_t vmlaq_laneq_s16(int16x8_t a, int16x8_t b, int16x8_t v, const int lane);
int32x4_t vmlaq_laneq_s32(int32x4_t a, int32x4_t b, int32x4_t v, const int lane);
uint16x8_t vmlaq_laneq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t v, const int lane);
uint32x4_t vmlaq_laneq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t v, const int lane);
float32x4_t vmlaq_laneq_f32(float32x4_t a, float32x4_t b, float32x4_t v, const int lane);

} // namespace pxl