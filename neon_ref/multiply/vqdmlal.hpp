#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlal_type
//----------------------------------------------------------------------
int32x4_t vqdmlal_s16(int32x4_t N, int16x4_t M, int16x4_t P);
int64x2_t vqdmlal_s32(int64x2_t N, int32x2_t M, int32x2_t P);

} // namespace pxl