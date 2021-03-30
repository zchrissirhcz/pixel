#ifndef PIXEL_MATCALC_MATRIX_TRANSPOSE_H
#define PIXEL_MATCALC_MATRIX_TRANSPOSE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void matrix_transpose_u8_naive(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst);

void matrix_transpose_u8_order_opt(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst);

// 划分矩阵，先转置所有分块，再对每个分块做转置
void transpose_f32_4x4(float* data0, float* data1, float* data2, float* data3);
void matrix_transpose_u8_partition8x8(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst);

void transpose_u8_8x8(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3, 
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7);
void matrix_transpose_u8_partition8x8_asimd(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_TRANSPOSE_H