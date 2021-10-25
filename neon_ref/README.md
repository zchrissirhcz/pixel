# Neon Intrinsics Reference Implementations

[![linux-aarch64](https://github.com/zchrissirhcz/pixel/actions/workflows/linux-aarch64.yml/badge.svg)](https://github.com/zchrissirhcz/pixel/actions/workflows/linux-aarch64.yml)

## Intro
Here is a (WIP) reference implementation of ARM NEON intrinsics with C++, compared with its counterpart with unittests. The purpose is running neon intrinsics on PC, with benifits:
- easily debugging with neon intrinsics
- don't have to buy an ARM device such Apple M1
- don't have to learn how to configure and run android console application

## Usage
```c++
#include "pixel_neon.hpp" // instead of `#include <arm_neon.h>` directly
using namespace pxl; // required

int main()
{
    // same usage as ARM NEON
    int8x8_t v1 = {120, 2, 3, 4, 5,  6,  7,   1};
    int8x8_t v2 = {121, 3, 5, 7, 9, 11, 13, -15};
    int8x8_t v_out = vrhadd_s8(v1, v2);

    // easily print, or debug with break points
    for (int i=0; i<8; i++)
    {
        printf("%d, ", v_out[i]);
    }
    printf("\n");
    return 0;
}
```

## Known issue

**header file ordr**

`arm_neon.h` must be after `pixel_neon.hpp` when using:
```c++
#include "pixel_neon.hpp"
#include <arm_neon.h>
```
otherwise it will cause conflicts of macros (neon_neon.h) and functions(pixel_neon.hpp), such as `vmul_lane_s16` .


**test**

Vector array element assign would be error (on some platform):
```c++
int8x8x2_t v_src;
v_src.val[0] = {1, 2, 3, 4, 5, 6, 7, 8};
v_src.val[1] = {9, 10, 11, 12, 13, 14, 15, 16};
```
Replacement:
```c++
int8x8x2_t v_src = {
    {1, 2, 3, 4, 5, 6, 7, 8},
    {9, 10, 11, 12, 13, 14, 15, 16}
}
```

## References

- ARM DEN0018A (NEON Programmer's Guide 1.0), Appendix D (not covering all intrinsics)

- https://developer.arm.com/architectures/instruction-sets/intrinsics/ (online query, contains nearly all intrinsics)

- https://github.com/rogerou/Arm-neon-intrinsics/blob/master/IHI0073B_arm_neon_intrinsics_ref.pdf (A more comprehensive NEON intrinsics reference pdf)

- https://github.com/thenifty/neon-guide

- https://github.com/dingjikerbo/Android-Boost

- https://blog.csdn.net/EmSoftEn/article/details/51718763

- https://github.com/rogerou/Arm-neon-intrinsics