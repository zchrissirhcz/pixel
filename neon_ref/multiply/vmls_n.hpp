#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmls_n_type
//----------------------------------------------------------------------
int16x4_t vmls_n_s16(int16x4_t a, int16x4_t b, int16_t c);
int32x2_t vmls_n_s32(int32x2_t a, int32x2_t b, int32_t c);
uint16x4_t vmls_n_u16(uint16x4_t a, uint16x4_t b, uint16_t c);
uint32x2_t vmls_n_u32(uint32x2_t a, uint32x2_t b, uint32_t c);
float32x2_t vmls_n_f32(float32x2_t a, float32x2_t b, float32_t c);

//----------------------------------------------------------------------
// vmlsq_n_type
//----------------------------------------------------------------------
int16x8_t vmlsq_n_s16(int16x8_t a, int16x8_t b, int16_t c);
int32x4_t vmlsq_n_s32(int32x4_t a, int32x4_t b, int32_t c);
uint16x8_t vmlsq_n_u16(uint16x8_t a, uint16x8_t b, uint16_t c);
uint32x4_t vmlsq_n_u32(uint32x4_t a, uint32x4_t b, uint32_t c);
float32x4_t vmlsq_n_f32(float32x4_t a, float32x4_t b, float32_t c);

} // namespace pxl