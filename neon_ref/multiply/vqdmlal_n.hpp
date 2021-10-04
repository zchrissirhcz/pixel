#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlal_n_type
//----------------------------------------------------------------------
int32x4_t vqdmlal_n_s16(int32x4_t a, int16x4_t b, int16_t c);
int64x2_t vqdmlal_n_s32(int64x2_t a, int32x2_t b, int32_t c);

} // namespace pxl