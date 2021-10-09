#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqabs_type
//----------------------------------------------------------------------
int8x8_t vqabs_s8(int8x8_t N);
int16x4_t vabs_s16(int16x4_t N);
int32x2_t vabs_s32(int32x2_t N);

//----------------------------------------------------------------------
// vqabsq_type
//----------------------------------------------------------------------
int8x16_t vqabsq_s8(int8x16_t N);
int16x8_t vqabsq_s16(int16x8_t N);
int32x4_t vqabsq_s32(int32x4_t N);

} // namespace pxl