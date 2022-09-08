#pragma once

#ifdef USE_DEBUGGER
#  if __GNUC__
#    define PX_ASSERT(expr) if (!(expr)) __builtin_trap()
#  elif _MSC_VER
#    define PX_ASSERT(expr) if (!(expr)) __debugbreak()
#  else
#    define PX_ASSERT(expr) if (!(expr)) *(volatile int *)0 = 0
#  endif
#else
#  include <assert.h>
#  define PX_ASSERT(expr) assert(expr)
#endif