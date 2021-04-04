#ifndef PIXEL_CNNLAYER_RELU_LAYER_H
#define PIXEL_CNNLAYER_RELU_LAYER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void relu_f32_forward_naive(float* src, float* dst, uint32_t len);
void relu_f32_forward_asimd(float* src, float* dst, uint32_t len);
void relu_f32_forward_asimd2(float* src, float* dst, uint32_t len);
void relu_f32_forward_asimd3(float* src, float* dst, uint32_t len);

void relu_f32_forward_asm(float* src, float* dst, uint32_t len);
void relu_f32_forward_asm2(float* src, float* dst, uint32_t len);
void relu_f32_forward_asm3(float* src, float* dst, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif