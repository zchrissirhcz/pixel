#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmlal_type
//----------------------------------------------------------------------
int16x8_t vmlal_s8(int16x8_t N, int8x8_t M, int8x8_t P);
int32x4_t vmlal_s16(int32x4_t N, int16x4_t M, int16x4_t P);
int64x2_t vmlal_s32(int64x2_t N, int32x2_t M, int32x2_t P);

uint16x8_t vmlal_u8(uint16x8_t N, uint8x8_t M, uint8x8_t P);
uint32x4_t vmlal_u16(uint32x4_t N, uint16x4_t M, uint16x4_t P);
uint64x2_t vmlal_u32(uint64x2_t N, uint32x2_t M, uint32x2_t P);

} // namespace pxl