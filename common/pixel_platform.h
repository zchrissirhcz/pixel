#pragma once

#include "pixel_log.h"
#include <inttypes.h>

// https://stackoverflow.com/a/12792056/2999096
static inline int pxl_is_little_endian()
{
    volatile uint32_t data=0x01234567;
    return (*((uint8_t*)(&data))) == 0x67;
}

static inline int pxl_is_big_endian()
{
    return 1 - pxl_is_little_endian();
}


// https://stackoverflow.com/a/2182581/2999096
static void pxl_swap_bytes(void* pv, size_t n)
{
    PIXEL_ASSERT(n>0, "n>0 required");

    char *p = (char*)pv;
    size_t lo, hi;
    for(lo=0, hi=n-1; hi>lo; lo++, hi--)
    {
        char tmp=p[lo];
        p[lo] = p[hi];
        p[hi] = tmp;
    }
}
#define PXL_SWAP_BYTES(x) pxl_swap_bytes(&(x), sizeof(x));


static int inline pxl_align_up(int x, int n) {
    return ((x + n - 1) / n ) * n;
}



void pxl_platform_echo()
{
#ifdef _MSC_VER
    PIXEL_LOGI("MSVC\n");
#endif

#if ANDROID
    PIXEL_LOGI("android\n");
#endif

#if __linux__ && !(ANDROID)
    PIXEL_LOGI("linux\n");
#endif

#if __APPLE__
    PIXEL_LOGI("Apple\n");
#endif

    PIXEL_LOGI("hello\n");

    return 0;
}


