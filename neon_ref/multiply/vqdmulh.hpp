#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqdmulh_type
//----------------------------------------------------------------------
int16x4_t vqdmulh_s16(int16x4_t M, int16x4_t M);

} // namespace pxl