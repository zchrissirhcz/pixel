#pragma once

#include "pixel_neon_base.hpp"

namespace pxl {

//----------------------------------------------------------------------
// vqshrun_n_type
//----------------------------------------------------------------------

/// @param n 1-8
uint8x8_t vqshrun_n_s16(int16x8_t v, const int n);

/// @param n 1-16
uint16x4_t vqshrun_n_s32(int32x4_t v, const int n);

/// @param n 1-32
uint32x2_t vqshrun_n_s64(int64x2_t v, const int n);


} // namespace pxl
