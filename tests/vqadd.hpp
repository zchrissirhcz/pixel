#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqadd_type
//----------------------------------------------------------------------
int8x8_t vqadd_s8(int8x8_t N, int8x8_t M);
int16x4_t vqadd_s16(int16x4_t N, int16x4_t M);
int32x2_t vqadd_s32(int32x2_t N, int32x2_t M);
int64x1_t vqadd_s64(int64x1_t N, int64x1_t M);

uint8x8_t vqadd_u8(uint8x8_t N, uint8x8_t M);
uint16x4_t vqadd_u16(uint16x4_t N, uint16x4_t M);
uint32x2_t vqadd_u32(uint32x2_t N, uint32x2_t M);
uint64x1_t vqadd_s64(uint64x1_t N, uint64x1_t M);

//----------------------------------------------------------------------
// vqaddq_type
//----------------------------------------------------------------------
int8x16_t vqaddq_s8(int8x16_t N, int8x16_t M);
int16x8_t vqaddq_s16(int16x8_t N, int16x8_t M);
int32x4_t vqaddq_s32(int32x4_t N, int32x4_t M);
int64x2_t vqaddq_s64(int64x2_t N, int64x2_t M);

uint8x16_t vqaddq_u8(uint8x16_t N, uint8x16_t M);
uint16x8_t vqaddq_u16(uint16x8_t N, uint16x8_t M);
uint32x4_t vqaddq_u32(uint32x4_t N, uint32x4_t M);
uint64x2_t vqaddq_u64(uint64x2_t N, uint64x2_t M);

} // namespace pxl