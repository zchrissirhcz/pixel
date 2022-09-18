#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef unsigned char uchar;

static inline uint32_t align_up(uint32_t x, uint32_t n) {
    return ((x + n - 1) / n) * n;
}