#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

int32x4_t vqdmull_n_s16(int16x4_t M, int16_t N);
int64x2_t vqdmull_n_s32(int32x2_t M, int32_t N);

} // namespace pxl