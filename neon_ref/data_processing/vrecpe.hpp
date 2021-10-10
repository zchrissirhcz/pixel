#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vrecpe_type
//----------------------------------------------------------------------
uint32x2_t vrecpe_u32(uint32x2_t N);
float32x2_t vrecpe_f32(float32x2_t N);

//----------------------------------------------------------------------
// vrecpeq_type
//----------------------------------------------------------------------
uint32x4_t vrecpeq_u32(uint32x4_t N);
float32x4_t vrecpeq_f32(float32x4_t N);

} // namespace pxl