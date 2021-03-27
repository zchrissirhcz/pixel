#include "matrix_add_sub.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

// A(mxn), B(mxn), C(mxn)
// C = A + B
void matrix_add_f32_naive(float* mA, float* mB, float* mC, size_t M, size_t N)
{
    size_t total_len = M * N;
    for (size_t i=0; i<total_len; i++) {
        mC[i] = mA[i] + mB[i];
    }
}

void matrix_add_f32_asimd(float* mA, float* mB, float* mC, size_t M, size_t N)
{
    size_t total_len = M * N;
    size_t done = 0;

#if __ARM_NEON
    size_t step = 4;
    size_t vec_size = total_len - total_len % step;
    float32x4_t vA;
    float32x4_t vB;
    float32x4_t vC;
    for (size_t i=0; i<vec_size; i+=step) {
        vA = vld1q_f32(mA);
        mA += step;

        vB = vld1q_f32(mB);
        mB += step;
        
        vC = vaddq_f32(vA, vB);

        vst1q_f32(mC, vC);
        mC += step;
    }
#endif // __ARM_NEON

    for (size_t i=done; i<total_len; i++) {
        mC[i] = mA[i] + mB[i];
    }
}


//----------------------------------------------------------------------

// A(mxn), B(mxn), C(mxn)
// C = A - B
void matrix_sub(float* mA, float* mB, float* mC, size_t M, size_t N)
{
    size_t total_len = M * N;
    for (size_t i=0; i<total_len; i++) {
        mC[i] = mA[i] - mB[i];
    }
}