#ifndef PIXEL_PLATFORM_H
#define PIXEL_PLATFORM_H

#include "pixel_log.h"
#include <inttypes.h>

// https://stackoverflow.com/a/12792056/2999096
static inline int _pxl_is_little_endian()
{
    volatile uint32_t data=0x01234567;
    return (*((uint8_t*)(&data))) == 0x67;
}

static inline int _pxl_is_big_endian()
{
    return 1 - _pxl_is_little_endian();
}


// https://stackoverflow.com/a/2182581/2999096
static void _pxl_swap_bytes(void* pv, size_t n)
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
#define _PXL_SWAP_BYTES(x) _pxl_swap_bytes(&(x), sizeof(x));


static int inline align_up(int x, int n) {
    return ((x + n - 1) / n ) * n;
}


#endif // PIXEL_PLATFORM_H