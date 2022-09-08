#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void relu_forward_naive(float* input, size_t len, float* output);
void relu_forward_asimd(float* input, size_t len, float* output);
void relu_forward_asm(float* input, size_t len, float* output);

void relu_inplace_forward_naive(float* data, size_t len);
void relu_inplace_forward_asimd(float* data, size_t len);
void relu_inplace_forward_asm(float* data, size_t len);

#ifdef __cplusplus
}
#endif
