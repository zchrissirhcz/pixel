# Neon Intrinsics Reference Implementations

There is a (WIP) reference implementation of ARM NEON intrinsics with C++, compared with its counterpart with unittests.

## Known issue

header file order: `arm_neon.h` must be after `pixel_neon.hpp` when using:
```c++
#include "pixel_neon.hpp"
#include <arm_neon.h>
```
otherwise it will cause conflicts of macros (neon_neon.h) and functions(pixel_neon.hpp), such as `vmul_lane_s16` .

## References

- ARM DEN0018A (NEON Programmer's Guide 1.0), Appendix D (not covering all intrinsics)

- https://developer.arm.com/architectures/instruction-sets/intrinsics/ (online query, contains nearly all intrinsics)

- https://github.com/rogerou/Arm-neon-intrinsics/blob/master/IHI0073B_arm_neon_intrinsics_ref.pdf (A more comprehensive NEON intrinsics reference pdf)

- https://github.com/thenifty/neon-guide

- https://github.com/dingjikerbo/Android-Boost

- https://blog.csdn.net/EmSoftEn/article/details/51718763

- https://github.com/rogerou/Arm-neon-intrinsics