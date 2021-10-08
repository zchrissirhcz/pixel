#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vabs_type
//----------------------------------------------------------------------
int8x8_t vabs_s8(int8x8_t N);
int16x4_t vabs_s16(int16x4_t N);
int32x2_t vabs_s32(int32x2_t N);
float32x2_t vabs_f32(float32x2_t N);

//----------------------------------------------------------------------
// vabsq_type
//----------------------------------------------------------------------
int8x16_t vabsq_s8(int8x16_t N);
int16x8_t vabsq_s16(int16x8_t N);
int32x4_t vabsq_s32(int32x4_t N);
float32x4_t vabsq_f32(float32x4_t N);

} // namespace pxl