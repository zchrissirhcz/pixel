#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vmlal_n_type
//----------------------------------------------------------------------
int32x4_t vmlal_n_s16(int32x4_t a, int16x4_t b, int16_t c);
int64x2_t vmlal_n_s32(int64x2_t a, int32x2_t b, int32_t c);
uint32x4_t vmlal_n_u16(uint32x4_t a, uint16x4_t b, uint16_t c);
uint64x2_t vmlal_n_u32(uint64x2_t a, uint32x2_t b, uint32_t c);

} // namespace pxl