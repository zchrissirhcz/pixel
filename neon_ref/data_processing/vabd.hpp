#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vabd_type
//----------------------------------------------------------------------
int8x8_t vabd_s8(int8x8_t N, int8x8_t M);
int16x4_t vabd_s16(int16x4_t N, int16x4_t M);
int32x2_t vabd_s32(int32x2_t N, int32x2_t M);
uint8x8_t vabd_u8(uint8x8_t N, uint8x8_t M);
uint16x4_t vabd_u16(uint16x4_t N, uint16x4_t M);
uint32x2_t vabd_u32(uint32x2_t N, uint32x2_t M);
float32x2_t vabd_f32(float32x2_t N, float32x2_t M);

//----------------------------------------------------------------------
// vabdq_type
//----------------------------------------------------------------------
int8x16_t vabdq_s8(int8x16_t N, int8x16_t M);
int16x8_t vabdq_s16(int16x8_t N, int16x8_t M);
int32x4_t vabdq_s32(int32x4_t N, int32x4_t M);
uint8x16_t vabdq_u8(uint8x16_t N, uint8x16_t M);
uint16x8_t vabdq_u16(uint16x8_t N, uint16x8_t M);
uint32x4_t vabdq_u32(uint32x4_t N, uint32x4_t M);
float32x4_t vabdq_f32(float32x4_t N, float32x4_t M);

} // namespace pxl