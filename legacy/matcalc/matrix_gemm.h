#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// A(mxk), B(kxn), C（mxn）
// C = A B
void matrix_gemm_f32_naive(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N);

// 调整遍历顺序
void matrix_gemm_f32_order_opt(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N);

void matrix_gemm_f32_order_opt2(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N);

void matrix_gemm_f32_asimd(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N);

#ifdef __cplusplus
}
#endif
