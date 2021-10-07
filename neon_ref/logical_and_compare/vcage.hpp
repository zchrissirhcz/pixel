#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vcage_type
//----------------------------------------------------------------------
uint32x2_t vcage_f32(float32x2_t a, float32x2_t b);
uint64x1_t vcage_f64(float64x1_t a, float64x1_t b);
//uint16x4_t vcage_f16(float16x4_t a, float16x4_t b);

//----------------------------------------------------------------------
// vcageq_type
//----------------------------------------------------------------------
uint32x4_t vcageq_f32(float32x4_t a, float32x4_t b);
uint64x2_t vcageq_f64(float64x2_t a, float64x2_t b);
//uint16x8_t vcageq_f16(float16x8_t a, float16x8_t b);

//----------------------------------------------------------------------
// vcageX_type
//----------------------------------------------------------------------
uint32_t vcages_f32(float32_t a, float32_t b);
uint64_t vcaged_f64(float64_t a, float64_t b);
//uint16_t vcageh_f16(float16_t a, float16_t b);

} // namespace pxl