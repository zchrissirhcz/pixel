#include "array_fill.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

//----------------------------------------------------------------------

void array_fill_u8_naive(unsigned char* data, size_t len, unsigned char value)
{
    for (size_t i=0; i<len; i++) {
        data[i] = value;
    }
}

void array_fill_u8_asimd(unsigned char* data, size_t len, unsigned char value)
{
    size_t done = 0;
#if __ARM_NEON
    size_t step = 16;
    size_t vec_size = len - len % step;
    uint8x16_t v1 = vdupq_n_u8(value);
    for (size_t i=0; i<vec_size; i+=step) {
        vst1q_u8(data, v1);
        data += step;
    }
    done = vec_size;
#endif // __ARM_NEON

    for (size_t i=done; i<len; i++) {
        data[i] = value;
    }
}

//----------------------------------------------------------------------

void array_fill_s32_naive(int* data, size_t len, int value)
{
    for (size_t i=0; i<len; i++) {
        data[i] = value;
    }
}

void array_fill_s32_asimd(int* data, size_t len, int value)
{
    size_t done = 0;
#if __ARM_NEON
    size_t step = 4;
    size_t vec_size = len - len % step;
    int32x4_t v1 = vdupq_n_s32(value);
    for (size_t i=0; i<vec_size; i+=step) {
        vst1q_s32(data, v1);
        data += step;
    }
    done = vec_size;
#endif // __ARM_NEON

    for (size_t i=done; i<len; i++) {
        data[i] = value;
    }
}

//----------------------------------------------------------------------

void array_fill_u32_naive(unsigned int* data, size_t len, unsigned int value)
{
    for (size_t i=0; i<len; i++) {
        data[i] = value;
    }
}

void array_fill_u32_asimd(unsigned int* data, size_t len, unsigned int value)
{
    size_t done = 0;
#if __ARM_NEON
    size_t step = 4;
    size_t vec_size = len - len % step;
    uint32x4_t v1 = vdupq_n_u32(value);
    for (size_t i=0; i<vec_size; i+=step) {
        vst1q_u32(data, v1);
        data += step;
    }
    done = vec_size;
#endif // __ARM_NEON

    for (size_t i=done; i<len; i++) {
        data[i] = value;
    }
}

//----------------------------------------------------------------------

void array_fill_f32_naive(float* data, size_t len, float value)
{
    for (size_t i=0; i<len; i++) {
        data[i] = value;
    }
}

void array_fill_f32_asimd(float* data, size_t len, float value)
{
    size_t done = 0;
#if __ARM_NEON
    size_t step = 4;
    size_t vec_size = len - len % step;
    float32x4_t v1 = vdupq_n_f32(value);
    for (size_t i=0; i<vec_size; i+=step) {
        vst1q_f32(data, v1);
        data += step;
    }
    done = vec_size;
#endif // __ARM_NEON

    for (size_t i=done; i<len; i++) {
        data[i] = value;
    }
}