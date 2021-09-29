#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

int8x8_t vsubhn_s16(int16x8_t N, int16x8_t M);
int16x4_t vsubn_s32(int32x4_t N, int32x4_t M);
int32x2_t vsubn_s64(int64x2_t N, int64x2_t M);

uint8x8_t vsubhn_u16(uint16x8_t N, uint16x8_t M);
uint16x4_t vsubn_u32(uint32x4_t N, uint32x4_t M);
uint32x2_t vsubn_u64(uint64x2_t N, uint64x2_t M);

} // namespace pxl