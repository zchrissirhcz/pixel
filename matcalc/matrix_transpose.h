#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void matrix_transpose_u8_naive(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst);

void matrix_transpose_u8_order_opt(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst);

void matrix_transpose_u8_partition(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst, const uint32_t block_size);

void matrix_transpose_u8_partition_asimd(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst, const uint32_t block_size);

void transpose_u8_8x8_asimd(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3, 
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7);

void transpose_u8_16x16_asimd(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3, 
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7,
    unsigned char* data8, unsigned char* data9, unsigned char* data10, unsigned char* data11,
    unsigned char* data12, unsigned char* data13, unsigned char* data14, unsigned char* data15);

//----------------------------------------------------------------------
void matrix_transpose_f32_naive(float* src, uint32_t height, uint32_t width, float* dst);
void matrix_transpose_f32_order_opt(float* src, uint32_t height, uint32_t width, float* dst);
void matrix_transpose_f32_partition(float* src, uint32_t height, uint32_t width, float* dst, const uint32_t block_size);
void matrix_transpose_f32_partition_asimd(float* src, uint32_t height, uint32_t width, float* dst, const uint32_t block_size);
void transpose_f32_4x4_asimd(float* data0, float* data1, float* data2, float* data3);


#ifdef __cplusplus
}
#endif
