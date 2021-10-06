#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

// vrnda means vector Round to Integral, to nearest with ties to Away

//----------------------------------------------------------------------
// vrnda_type
//----------------------------------------------------------------------
float32x2_t vrnda_f32(float32x2_t a);
float64x1_t vrnda_f64(float64x1_t a);
//float16x4_t vrnda_f16(float16x4_t a);

//----------------------------------------------------------------------
// vrndaq_type
//----------------------------------------------------------------------
float32x4_t vrndaq_f32(float32x4_t a);
float64x2_t vrndaq_f64(float64x2_t a);
//float16x8_t vrndaq_f16(float16x8_t a);

//----------------------------------------------------------------------
// vrndah_type
//----------------------------------------------------------------------
//float16_t vrndah_f16(float16_t a);

} // namespace pxl