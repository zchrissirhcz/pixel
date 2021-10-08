#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vtst_type
//----------------------------------------------------------------------
uint8x8_t vtst_s8(int8x8_t N, int8x8_t M);
uint16x4_t vtst_s16(int16x4_t N, int16x4_t M);
uint32x2_t vtst_s32(int32x2_t N, int32x2_t M);
uint8x8_t vtst_u8(uint8x8_t N, uint8x8_t M);
uint16x4_t vtst_u16(uint16x4_t N, uint16x4_t M);
uint32x2_t vtst_u32(uint32x2_t N, uint32x2_t M);

//----------------------------------------------------------------------
// vtstq_type
//----------------------------------------------------------------------
uint8x16_t vtstq_s8(int8x16_t N, int8x16_t M);
uint16x8_t vtstq_s16(int16x8_t N, int16x8_t M);
uint32x4_t vtstq_s32(int32x4_t N, int32x4_t M);
uint8x16_t vtstq_u8(uint8x16_t N, uint8x16_t M);
uint16x8_t vtstq_u16(uint16x8_t N, uint16x8_t M);
uint32x4_t vtstq_u32(uint32x4_t N, uint32x4_t M);


} // namespace pxl