#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vpmax_type
//----------------------------------------------------------------------
int8x8_t vpmax_s8(int8x8_t N, int8x8_t M);
int16x4_t vpmax_s16(int16x4_t N, int16x4_t M);
int32x2_t vpmax_s32(int32x2_t N, int32x2_t M);
uint8x8_t vpmax_u8(uint8x8_t N, uint8x8_t M);
uint16x4_t vpmax_u16(uint16x4_t N, uint16x4_t M);
uint32x2_t vpmax_u32(uint32x2_t N, uint32x2_t M);
float32x2_t vpmax_u32(float32x2_t N, float32x2_t M);

} // namespace pxl