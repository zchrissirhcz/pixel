#pragma once

// usage:
// for neon platforms:
//   #include "px_neon.h"
//   #if PIXEL_NEON
//     ...
//   #endif // PIXEL_NEON
// for sse platform:
//   #define PIXEL_NEON_TEST 1 // requires this
//   #include "px_neon.h"
//   #if PIXEL_NEON
//     ...
//   #endif // PIXEL_NEON
//   #endif

#if __ARM_NEON
#   include <arm_neon.h>
#   define PIXEL_NEON 1
#elif defined(_M_ARM64)
#   include <arm64_neon.h>
#   define PIXEL_NEON 1
#elif defined(PIXEL_NEON_TEST) && PIXEL_NEON_TEST
#   include "px_neon2sse.h"
#   #define PIXEL_NEON 1
#endif
