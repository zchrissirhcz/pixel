#pragma once

#if defined __SSE2__ || defined _M_X64 || (defined _M_IX86_FP && _M_IX86_FP >= 2)
#   ifdef _MSC_VER
#       pragma warning(push)
#       pragma warning(disable: 4838)
#       pragma warning(disable: 4309)
#       pragma warning(disable: 4838)
#       pragma warning(disable: 4244)
#       pragma warning(disable: 4245)
#       pragma warning(disable: 4100)
#   elif defined(__GNUC__)
#       pragma GCC diagnostic push
#       pragma GCC diagnostic ignored "-Wsequence-point"
#       pragma GCC diagnostic ignored "-Wunused-function" // unused static functions
#       pragma GCC diagnostic ignored "-Wunused-parameter"
#       pragma GCC diagnostic ignored "-Wstrict-aliasing" // _MM_EXTRACT_PS
#   endif // _MSC_VER
#   define NEON2SSE_DISABLE_PERFORMANCE_WARNING // disable performance warning
// https://github.com/intel/ARM_NEON_2_x86_SSE/blob/master/NEON_2_SSE.h
#   include "NEON_2_SSE.h"
#   ifdef _MSC_VER
#       pragma warning(pop)
#   elif defined(__GNUC__)
#       pragma GCC diagnostic pop
#   endif // _MSC_VER
#   undef __ARM_NEON 
#   define __ARM_NEON 1 // enable NEON for test
#endif // SSE
