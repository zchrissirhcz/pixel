#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vcagt_type
//----------------------------------------------------------------------
uint32x2_t vcagt_f32(float32x2_t a, float32x2_t b);
uint64x1_t vcagt_f64(float64x1_t a, float64x1_t b);
//uint16x4_t vcagt_f16(float16x4_t a, float16x4_t b);

//----------------------------------------------------------------------
// vcagtq_type
//----------------------------------------------------------------------
uint32x4_t vcagtq_f32(float32x4_t a, float32x4_t b);
uint64x2_t vcagtq_f64(float64x2_t a, float64x2_t b);
//uint16x8_t vcagtq_f16(float16x8_t a, float16x8_t b);

//----------------------------------------------------------------------
// vcagtX_type
//----------------------------------------------------------------------
uint32_t vcagts_f32(float32_t a, float32_t b);
uint64_t vcagtd_f64(float64_t a, float64_t b);
//uint16_t vcagth_f16(float16_t a, float16_t b);

} // namespace pxl