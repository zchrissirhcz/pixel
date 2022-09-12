#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "px_assert.h"

// https://stackoverflow.com/a/2182581/2999096
static void px_swap_bytes(void* pv, size_t n)
{
    PX_ASSERT(n > 0);

    char* p = (char*)pv;
    size_t lo, hi;
    for (lo = 0, hi = n - 1; hi > lo; lo++, hi--)
    {
        char tmp = p[lo];
        p[lo] = p[hi];
        p[hi] = tmp;
    }
}
#define PX_SWAP_BYTES(x) px_swap_bytes(&(x), sizeof(x));

// https://stackoverflow.com/a/12792056/2999096
static inline bool px_is_little_endian()
{
    volatile uint32_t data = 0x01234567;
    return (*((uint8_t*)(&data))) == 0x67;
}

static inline bool px_is_big_endian()
{
    return !px_is_little_endian();
}

// Convert big-endian integer(4 bytes) to little-endian integer
static int px_reverse_int(int n)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = n & 255;
    ch2 = (n >> 8) & 255;
    ch3 = (n >> 16) & 255;
    ch4 = (n >> 24) & 255;
    return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}