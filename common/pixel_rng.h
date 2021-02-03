/**
* Portable random number generator
* slightly modifed from https://github.com/rdadolf/prng/blob/master/prng.h
* and know it from https://github.com/Tencent/ncnn/blob/master/tests/prng.h
* 
* Usage:
*
    PIXEL_SRAND(7767517); // or other seed. must set this, or you'll keep gettting same value
    float val = pixel_random_float(-1.2, 233);
*
*/

#ifndef PIXEL_RNG_H
#define PIXEL_RNG_H

#include <stdint.h>

#define PXL_RNG_LAG1 24U
#define PXL_RNG_LAG2 55U
#define PXL_RNG_RAND_SSIZE ((1U) << 6)
#define PXL_RNG_RAND_SMASK (PXL_RNG_RAND_SSIZE-1)
#define PXL_RNG_RAND_EXHAUST_LIMIT PXL_RNG_LAG2
// 10x is a heuristic, it just needs to be large enough to remove corellation
#define PXL_RNG_RAND_REFILL_COUNT ((PXL_RNG_LAG2*10)-PXL_RNG_RAND_EXHAUST_LIMIT)

typedef struct pxl_rng_rand_t {
    uint64_t s[PXL_RNG_RAND_SSIZE]; // Lags
    uint_fast16_t i; // Location of the current lag
    uint_fast16_t c; // Exhaustion count
} pxl_rng_rand_t;

#define PXL_RNG_RAND_MAX UINT64_MAX

static inline uint64_t pxl_rng_rand(pxl_rng_rand_t* state) {
    uint_fast16_t i;
    uint_fast16_t r, new_rands = 0;

    if (!(state->c)) { // Randomness exhausted, run forward to refill
        new_rands += PXL_RNG_RAND_REFILL_COUNT + 1;
        state->c = PXL_RNG_RAND_EXHAUST_LIMIT - 1;
    } else {
        new_rands = 1;
        state->c--;
    }

    for (r=0; r<new_rands; r++) {
        i = state->i;
        state->s[i&PXL_RNG_RAND_SMASK] = 
            state->s[(i+PXL_RNG_RAND_SSIZE-PXL_RNG_LAG1) & PXL_RNG_RAND_SMASK] +
            state->s[(i+PXL_RNG_RAND_SSIZE-PXL_RNG_LAG2) & PXL_RNG_RAND_SMASK];
        state->i++;
    }
    return state->s[i&PXL_RNG_RAND_SMASK];
}

static inline void pxl_rng_srand(uint64_t seed, pxl_rng_rand_t* state) {
    uint_fast16_t i;
    // Naive seed
    state->c = PXL_RNG_RAND_EXHAUST_LIMIT;
    state->i = 0;

    state->s[0] = seed;
    for(i=1; i<PXL_RNG_RAND_SSIZE; i++) {
        // Arbitrary magic, mostly to eliminate the effect of low-value seeds.
        // Probably could be better, but the run-up obviates any real need to.
        state->s[i] = i*(2147483647ULL) + seed;
    }

    // Run forward 10,000 numbers
    for(i=0; i<10000; i++) {
        pxl_rng_rand(state);
    }
}

static pxl_rng_rand_t g_pxl_rng_rand_state;
#define PIXEL_SRAND(seed) pxl_rng_srand(seed, &g_pxl_rng_rand_state)
#define PXL_RAND()      pxl_rng_rand(&g_pxl_rng_rand_state)

static float pixel_random_float(float a, float b)
{
    float random = ((float)PXL_RAND()) / (float)uint64_t(-1); // RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

static uint8_t pixel_random_uint8(uint8_t a, uint8_t b) 
{
    return (uint8_t)pixel_random_float(a, b);
}

// Clean up our macros
#undef PXL_RNG_LAG1
#undef PXL_RNG_LAG2
#undef PXL_RNG_RAND_SSIZE
#undef PXL_RNG_RAND_SMASK
#undef PXL_RNG_RAND_EXHAUST_LIMIT
#undef PXL_RNG_RAND_REFILL_COUNT
#undef PXL_RNG_RAND_MAX
#undef PXL_RAND

#endif // PIXEL_RNG_H