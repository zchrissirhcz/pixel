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

void matrix_transpose_u8_partition(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst, const uint32_t block_size);

void matrix_transpose_u8_partition_asimd(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst, const uint32_t block_size);

void transpose_u8_8x8_asimd(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3, 
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7);

void transpose_u8_16x16_asimd(unsigned char* data0, unsigned char* data1, unsigned char* data2, unsigned char* data3, 
    unsigned char* data4, unsigned char* data5, unsigned char* data6, unsigned char* data7,
    unsigned char* data8, unsigned char* data9, unsigned char* data10, unsigned char* data11,
    unsigned char* data12, unsigned char* data13, unsigned char* data14, unsigned char* data15);


#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_MATRIX_TRANSPOSE_H