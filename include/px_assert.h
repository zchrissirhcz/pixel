#pragma once

#ifdef USE_DEBUGGER
#  if __GNUC__
#    define PX_ASSERT(c) if (!(c)) __builtin_trap()
#  elif _MSC_VER
#    define PX_ASSERT(c) if (!(c)) __debugbreak()
#  else
#    define PX_ASSERT(c) if (!(c)) *(volatile int *)0 = 0
#  endif
#else
#  include <assert.h>
#  define PX_ASSERT(c) assert(c)
#endif