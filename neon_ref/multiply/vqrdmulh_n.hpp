#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqrdmulh_n_type
//----------------------------------------------------------------------
int16x4_t vqrdmulh_n_s16(int16x4_t N, int16_t M);
int32x2_t vqrdmulh_n_s32(int32x2_t N, int32_t M);

//----------------------------------------------------------------------
// vqrdmulhq_n_type
//----------------------------------------------------------------------
int16x8_t vqrdmulhq_n_s16(int16x8_t N, int16_t M);
int32x4_t vqrdmulhq_n_s32(int32x4_t N, int32_t M);

} // namespace pxl