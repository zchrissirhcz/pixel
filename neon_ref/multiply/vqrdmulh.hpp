#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqrdmulh_type
//----------------------------------------------------------------------
int16x4_t vqrdmulh_s16(int16x4_t N, int16x4_t M);
int32x2_t vqrdmulh_s32(int32x2_t N, int32x2_t M);

//----------------------------------------------------------------------
// vqrdmulhq_type
//----------------------------------------------------------------------
int16x8_t vqrdmulhq_s16(int16x8_t N, int16x8_t M);
int32x4_t vqrdmulhq_s32(int32x4_t N, int32x4_t M);

} // namespace pxl