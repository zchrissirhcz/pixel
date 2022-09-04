#pragma once

#include "pixel_simd_base.h"

//----------------------------------------------------------------------
// => function implementations
//----------------------------------------------------------------------
#if defined(PIXEL_SIMD_NEON)
#include "pixel_simd_neon.h"
#elif defined(PIXEL_SIMD_SSE)
#include "pixel_simd_sse.h"
#elif defined(PIXEL_SIMD_SIMU)
#include "pixel_simd_simu.h"
#endif // PIXEL_NEON

