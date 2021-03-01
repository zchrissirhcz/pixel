#ifndef PIXEL_CNN_RELU_LAYER_H
#define PIXEL_CNN_RELU_LAYER_H

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

#endif // PIXEL_CNN_RELU_LAYER_H