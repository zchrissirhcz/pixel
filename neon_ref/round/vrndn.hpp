#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

// vrndn means vector rounding to nearest with ties to even

//----------------------------------------------------------------------
// vrndn_type
//----------------------------------------------------------------------
float32x2_t vrndn_f32(float32x2_t a);
float64x1_t vrndn_f64(float64x1_t a);
//float16x4_t vrndn_f16(float16x4_t a);

//----------------------------------------------------------------------
// vrndnq_type
//----------------------------------------------------------------------
float32x4_t vrndnq_f32(float32x4_t a);
float64x2_t vrndnq_f64(float64x2_t a);
//float16x8_t vrndnq_f16(float16x8_t a);

//----------------------------------------------------------------------
// vrndnX_type
//----------------------------------------------------------------------
float32_t vrndns_f32(float32_t a);
//float16_t vrndnh_f16(float16_t a);


} // namespace pxl