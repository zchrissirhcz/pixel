#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vaddw_type
//----------------------------------------------------------------------
int16x8_t vaddw_s8(int16x8_t N, int8x8_t M);

int32x4_t vaddw_s16(int32x4_t N, int16x4_t M);

int64x2_t vaddw_s32(int64x2_t N, int32x2_t M);

uint16x8_t vaddw_u8(uint16x8_t N, uint8x8_t M);

uint32x4_t vaddw_u16(uint32x4_t N, uint16x4_t M);

uint64x2_t vaddw_u32(uint64x2_t N, uint32x2_t M);

} // namespace pxl