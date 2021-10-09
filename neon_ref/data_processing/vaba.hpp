#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vaba_type
//----------------------------------------------------------------------
int8x8_t vaba_s8(int8x8_t N, int8x8_t M, int8x8_t P);
int16x4_t vaba_s16(int16x4_t N, int16x4_t M, int16x4_t P);
int32x2_t vaba_s32(int32x2_t N, int32x2_t M, int32x2_t P);
uint8x8_t vaba_u8(uint8x8_t N, uint8x8_t M, uint8x8_t P);
uint16x4_t vaba_u16(uint16x4_t N, uint16x4_t M, uint16x4_t P);
uint32x2_t vaba_u32(uint32x2_t N, uint32x2_t M, uint32x2_t P);

//----------------------------------------------------------------------
// vabaq_type
//----------------------------------------------------------------------
int8x16_t vabaq_s8(int8x16_t N, int8x16_t M, int8x16_t P);
int16x8_t vabaq_s16(int16x8_t N, int16x8_t M, int16x8_t P);
int32x4_t vabaq_s32(int32x4_t N, int32x4_t M, int32x4_t P);
uint8x16_t vabaq_u8(uint8x16_t N, uint8x16_t M, uint8x16_t P);
uint16x8_t vabaq_u16(uint16x8_t N, uint16x8_t M, uint16x8_t P);
uint32x4_t vabaq_u32(uint32x4_t N, uint32x4_t M, uint32x4_t P);


} // namespace pxl
