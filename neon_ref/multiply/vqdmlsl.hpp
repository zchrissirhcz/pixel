#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmlsl_type
//----------------------------------------------------------------------
int32x4_t vqdmlsl_s16(int32x4_t N, int16x4_t M, int16x4_t P);
int64x2_t vqdmlsl_s32(int64x2_t N, int32x2_t M, int32x2_t P);

} // namespace pxl