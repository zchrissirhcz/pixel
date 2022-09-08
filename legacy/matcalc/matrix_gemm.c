#include "matrix_gemm.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

// A(mxk), B(kxn), C（mxn）
// C = A B
void matrix_gemm_f32_naive(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N)
{
    for (uint32_t i=0; i<M; i++) {
        for (uint32_t j=0; j<N; j++) {
            float sum = 0;
            for (uint32_t k=0; k<K; k++) {
                uint32_t a_idx = i*K + k;
                uint32_t b_idx = k*N + j;
                sum += mA[a_idx] * mB[b_idx];
            }
            uint32_t c_idx = i*N + j;
            mC[c_idx] = sum;
        }
    }
}

void matrix_gemm_f32_order_opt(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N)
{
    for (uint32_t i=0; i<M; i++) {
        for (uint32_t k=0; k<K; k++) {
            for (uint32_t j=0; j<N; j++) {
                uint32_t a_idx = i*K + k;
                uint32_t b_idx = k*N + j;
                uint32_t c_idx = i*N + j;
                mC[c_idx] += mA[a_idx] * mB[b_idx];
            }
        }
    }
}

void matrix_gemm_f32_order_opt2(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N)
{
    uint32_t c_idx_base = 0;
    uint32_t b_idx_base = 0;
    uint32_t a_idx_base = 0;
    
    uint32_t a_idx;
    uint32_t b_idx;
    uint32_t c_idx;

    for (uint32_t i=0; i<M; i++) {
        b_idx_base = 0;
        for (uint32_t k=0; k<K; k++) {
            a_idx = a_idx_base + k;
            for (uint32_t j=0; j<N; j++) {
                b_idx = b_idx_base + j;
                c_idx = c_idx_base + j;
                mC[c_idx] += mA[a_idx] * mB[b_idx];
            }
            b_idx_base += N;
        }
        c_idx_base += N;
        a_idx_base += K;
    }
}

// based on matrix_gemm_f32_order_opt
// this implementation is even 2x slower than matrix_gemm_f32_order_opt
void matrix_gemm_f32_asimd(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N)
{
    for (uint32_t i=0; i<M; i++) {
        for (uint32_t k=0; k<K; k++) {
            uint32_t done = 0;
            uint32_t a_idx = i*K + k;
#if __ARM_NEON
            uint32_t total_len = N;
            uint32_t step = 4*2;
            uint32_t vec_size = total_len - total_len % step;
            float32x4_t v_mA = vdupq_n_f32(mA[a_idx]);
            uint32_t b_idx_base = k*N;
            uint32_t c_idx_base = i*N;
            for (uint32_t j=0; j<vec_size; j+=step) {
                //uint32_t b_idx = k*N + j;
                //uint32_t c_idx = i*N + j;
                uint32_t b_idx = b_idx_base + j;
                uint32_t c_idx = c_idx_base + j;
                float32x4x2_t v_mB = vld2q_f32(mB + b_idx);
                float32x4x2_t v_mC = vld2q_f32(mC + c_idx);
                v_mC.val[0] = vmlaq_f32(v_mC.val[0], v_mA, v_mB.val[0]);
                v_mC.val[1] = vmlaq_f32(v_mC.val[1], v_mA, v_mB.val[1]);
                vst2q_f32(mC + c_idx, v_mC);
            }
            done = vec_size;
#endif // __ARM_NEON
            for (uint32_t j=done; j<N; j++) {
                uint32_t b_idx = k*N + j;
                uint32_t c_idx = i*N + j;
                mC[c_idx] += mA[a_idx] * mB[b_idx];
            }
        }
    }
}