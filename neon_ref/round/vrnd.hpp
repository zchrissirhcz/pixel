#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

// vrnd means vector rounding to zero

//----------------------------------------------------------------------
// vrnd_type
//----------------------------------------------------------------------
float32x2_t vrnd_f32(float32x2_t a);
float64x1_t vrnd_f64(float64x1_t a);
float16x4_t vrnd_f16(float16x4_t a);


//----------------------------------------------------------------------
// vrndq_type
//----------------------------------------------------------------------
float32x4_t vrndq_f32(float32x4_t a);
float64x2_t vrndq_f64(float64x2_t a);
float16x8_t vrndq_f16(float16x8_t a);

} // namespace pxl