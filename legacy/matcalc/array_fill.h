#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void array_fill_u8_naive(unsigned char* data, size_t len, unsigned char value);
void array_fill_u8_asimd(unsigned char* data, size_t len, unsigned char value);

void array_fill_s32_naive(int* data, size_t len, int value);
void array_fill_s32_asimd(int* data, size_t len, int value);

void array_fill_u32_naive(unsigned int* data, size_t len, unsigned int value);
void array_fill_u32_asimd(unsigned int* data, size_t len, unsigned int value);

void array_fill_f32_naive(float* data, size_t len, float value);
void array_fill_f32_asimd(float* data, size_t len, float value);

#ifdef __cplusplus
}
#endif
