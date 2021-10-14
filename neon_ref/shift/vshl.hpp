#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

/**
VSHL left shifts each element in a vector by an amount specified in the corresponding element in
the second input vector. The shift amount is the signed integer value of the least significant byte
of the element in the second input vector. The bits shifted out of each element are lost. If the
signed integer value is negative, it results in a right shift. 
 */

//----------------------------------------------------------------------
// vshl_type
//----------------------------------------------------------------------
int8x8_t vshl_s8(int8x8_t M, int8x8_t N);
int16x4_t vshl_s16(int16x4_t M, int16x4_t N);
int32x2_t vshl_s32(int32x2_t M, int32x2_t N);
int64x1_t vshl_s64(int64x1_t M, int64x1_t N);
uint8x8_t vshl_u8(uint8x8_t M, uint8x8_t N);
uint16x4_t vshl_u16(uint16x4_t M, uint16x4_t N);
uint32x2_t vshl_u32(uint32x2_t M, uint32x2_t N);
uint64x1_t vshl_u64(uint64x1_t M, uint64x1_t N);

//----------------------------------------------------------------------
// vshlq_type
//----------------------------------------------------------------------
int8x16_t vshlq_s8(int8x16_t M, int8x16_t N);
int16x8_t vshlq_s16(int16x8_t M, int16x8_t N);
int32x4_t vshlq_s32(int32x4_t M, int32x4_t N);
int64x2_t vshlq_s64(int64x2_t M, int64x2_t N);
uint8x16_t vshlq_u8(uint8x16_t M, uint8x16_t N);
uint16x8_t vshlq_u16(uint16x8_t M, uint16x8_t N);
uint32x4_t vshlq_u32(uint32x4_t M, uint32x4_t N);
uint64x2_t vshlq_u64(uint64x2_t M, uint64x2_t N);


} // namespace pxl
