#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmul_n_type
//----------------------------------------------------------------------
int16x4_t vmul_n_s16(int16x4_t N, int16_t M);
int32x2_t vmul_n_s32(int32x2_t N, int32_t M);
uint16x4_t vmul_n_u16(uint16x4_t N, uint16_t M);
uint32x2_t vmul_n_u32(uint32x2_t N, uint32_t M);
float32x2_t vmul_n_f32(float32x2_t N, float32_t M);

#if __aarch64__
float64x1_t vmul_n_f64(float64x1_t N, float64_t M);
#endif // __aarch64__

//----------------------------------------------------------------------
// vmulq_n_type
//----------------------------------------------------------------------
int16x8_t vmulq_n_s16(int16x8_t N, int16_t M);
int32x4_t vmulq_n_s32(int32x4_t N, int32_t M);
uint16x8_t vmulq_n_u16(uint16x8_t N, uint16_t M);
uint32x4_t vmulq_n_u32(uint32x4_t N, uint32_t M);
float32x4_t vmulq_n_f32(float32x4_t N, float32_t M);

#if __aarch64__
float64x2_t vmulq_n_f64(float64x2_t N, float64_t M);
#endif // __aarch64__

} // namespace pxl