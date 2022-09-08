#pragma once

/*
 * Use xorshift algorithm for fast random float number generation
 * https://en.wikipedia.com/wiki/Xorshift
 *
 *
 * Usage:
 *   #include "common/pixel_fast_rng.h"
 *
 *   g_state.a = 7767517; // seed
 *   float a = pixel_fast_random_float()
 */

#include <stdint.h>

typedef struct xorshift64_state {
    uint64_t a;
} xorshift64_state;

#ifdef __cplusplus
extern "C" {
#endif

static uint64_t pxl_xorshift64(xorshift64_state* state);
static float pixel_fast_random_float(float a, float b);

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------
// implementations
//----------------------------------------------------------------------
xorshift64_state g_state; // global status

uint64_t pxl_xorshift64(xorshift64_state* state)
{
    uint64_t x = state->a;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return state->a = x;
}

float pixel_fast_random_float(float a, float b)
{
    float random = ((float)pxl_xorshift64(&g_state)) / (float)((uint64_t)(-1));
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

