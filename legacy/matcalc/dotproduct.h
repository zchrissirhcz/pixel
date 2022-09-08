#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

float dotproduct_f32_naive(float* a, float* b, uint32_t len);
float dotproduct_f32_asimd(float* a, float* b, uint32_t len);
float dotproduct_f32_asimd2(float* a, float* b, uint32_t len);
float dotproduct_f32_asimd3(float* a, float* b, uint32_t len);

//----------------------------------------------------------------------

uint64_t dotproduct_u8_naive(uint8_t* a, uint8_t* b, uint32_t len);
uint64_t dotproduct_u8_asimd(uint8_t* a, uint8_t* b, uint32_t len);
uint64_t dotproduct_u8_asimd2(uint8_t* a, uint8_t* b, uint32_t len);
uint64_t dotproduct_u8_asimd3(uint8_t* a, uint8_t* b, uint32_t len);
uint64_t dotproduct_u8_asimd4(uint8_t* a, uint8_t* b, uint32_t len);

#ifdef __cplusplus
}
#endif

