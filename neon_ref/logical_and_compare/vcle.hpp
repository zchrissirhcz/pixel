#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vcle_type
//----------------------------------------------------------------------
uint8x8_t vcle_s8(int8x8_t N, int8x8_t M);
uint16x4_t vcle_s16(int16x4_t N, int16x4_t M);
uint32x2_t vcle_s32(int32x2_t N, int32x2_t M);
uint8x8_t vcle_u8(uint8x8_t N, uint8x8_t M);
uint16x4_t vcle_u16(uint16x4_t N, uint16x4_t M);
uint32x2_t vcle_u32(uint32x2_t N, uint32x2_t M);
uint32x2_t vcle_f32(float32x2_t N, float32x2_t M);

//----------------------------------------------------------------------
// vcleq_type
//----------------------------------------------------------------------
uint8x16_t vcleq_s8(int8x16_t N, int8x16_t M);
uint16x8_t vcleq_s16(int16x8_t N, int16x8_t M);
uint32x4_t vcleq_s32(int32x4_t N, int32x4_t M);
uint8x16_t vcleq_u8(uint8x16_t N, uint8x16_t M);
uint16x8_t vcleq_u16(uint16x8_t N, uint16x8_t M);
uint32x4_t vcleq_u32(uint32x4_t N, uint32x4_t M);
uint32x4_t vcleq_f32(float32x4_t N, float32x4_t M);


} // namespace pxl