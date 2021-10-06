#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

// vrndp means vector rounding to Plus infinity.

//----------------------------------------------------------------------
// vrndp_type
//----------------------------------------------------------------------
float32x2_t vrndp_f32(float32x2_t a);
float64x1_t vrndp_f64(float64x1_t a);
float16x4_t vrndp_f16(float16x4_t a);

//----------------------------------------------------------------------
// vrndpq_type
//----------------------------------------------------------------------
float32x4_t vrndpq_f32(float32x4_t a);
float64x2_t vrndpq_f64(float64x2_t a);
float16x8_t vrndpq_f16(float16x8_t a);

//----------------------------------------------------------------------
// vrndph_type
//----------------------------------------------------------------------
//float16_t vrndph_f16(float16_t a);

} // namespace pxl