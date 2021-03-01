#include "relu_layer.h"


void relu_forward_naive(float* input, size_t len, float* output)
{
    for (size_t i=0; i<len; i++) {
        output[i] = (input[i]<0) ? 0 : input[i];
    }
}

void relu_forward_asimd(float* input, size_t len, float* output)
{
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const size_t step = 4; // 16 bytes / sizeof(float);
    vec_size = len - len % step;
    float32x4_t zero = vdupq_n_f32(0);
    float32x4_t mask;
    float32x4_t v1;
    float32x4_t v2;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(input + i);
        mask = vcltq_f32(v1, zero);
        v2 = vbslq_f32(mask, zero, v1);
        vst1q_f32(output+i, v2);
    }
#elif defined(PIXEL_SIMD_SSE)
#endif
    for (size_t i=vec_size; i<len; i++) {
        output[i] = (input[i] > 0) ? input[i] : 0;
    }
}

void relu_forward_asm(float* input, size_t len, float* output)
{
    //TODO
}

//----------

void relu_inplace_forward_naive(float* data, size_t len)
{
    for (size_t i=0; i<len; i++) {
        data[i] = (data[i] > 0) ? data[i] : 0;
    }
}

void relu_inplace_forward_asimd(float* data, size_t len)
{
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const size_t step = 4; // 16 bytes / sizeof(float);
    vec_size = len - len % step;
    float32x4_t value, mask;
    float32x4_t zero = vdupq_n_f32(0);
    for (size_t i=0; i<vec_size; i+=step) {
        value = vld1q_f32(data + i);
        mask = vcltq_f32(value, zero);
        value = vbslq_f32(mask, zero, value);
        vst1q_f32(data + i, value);
    }
#elif defined(PIXEL_SIMD_SSE)
#endif
    for (size_t i=vec_size; i<len; i++) {
        data[i] = (data[i] > 0) ? data[i] : 0;
    }
}

void relu_inplace_forward_asm(float* data, size_t len)
{
    //TODO
}