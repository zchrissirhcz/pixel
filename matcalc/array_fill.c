#include "array_fill.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

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