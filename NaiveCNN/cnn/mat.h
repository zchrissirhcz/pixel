// 这里库文件主要存在关于二维矩阵数组的操作
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "px_typebase.h"
#include "px_cnn.h"
#include "types.h"

#define NC_FULL  0
#define NC_SAME  1
#define NC_VALID 2

#ifdef __cplusplus
extern "C" {
#endif

matrix_t* create_matrix(px_size_t size);
void destroy_matrix(matrix_t* matrix);
void save_matrix_to_file(matrix_t* matrix, FILE* fout);

void matrix_add(matrix_t* src1, matrix_t* src2, matrix_t* dst);

matrix_t* get_rotate180_matrix(matrix_t* input);

matrix_t* correlation_for_matrix(matrix_t* map, matrix_t* input, int type);

matrix_t* conv_for_matrix(matrix_t* map, matrix_t* input, int type);

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
matrix_t* matrix_upsample(matrix_t* input, int width_multiplier, int height_multiplier);

// expaded regions filled with 0
matrix_t* matrix_copy_make_border(matrix_t* input, px_pad_t pad);

matrix_t* matrix_cut_make_border(matrix_t* input, px_pad_t pad);

void matrix_multiply_lambda(matrix_t* res, matrix_t* mat, float factor);

float matrix_sum(matrix_t* mat);

#ifdef __cplusplus
}
#endif
