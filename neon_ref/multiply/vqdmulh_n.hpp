#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmulh_n_type
//----------------------------------------------------------------------
int16x4_t vqdmulh_n_s16(int16x4_t M, int16_t N);
int32x2_t vqdmulh_n_s32(int32x2_t M, int32_t N);

//----------------------------------------------------------------------
// vqdmulhq_n_type
//----------------------------------------------------------------------
int16x8_t vqdmulhq_n_s16(int16x8_t M, int16_t N);
int32x4_t vqdmulhq_n_s32(int32x4_t M, int32_t N);

} // namespace pxl