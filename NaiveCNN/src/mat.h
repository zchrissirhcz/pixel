// 这里库文件主要存在关于二维矩阵数组的操作
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "naive_cnn.h"

#define NC_FULL 0
#define NC_SAME 1
#define NC_VALID 2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct matrix_t
{
    int height;
    int width;
    float** data;
} matrix_t;

matrix_t* create_matrix_ptr(int height, int width);
void destroy_matrix_ptr(matrix_t* matrix);
matrix_t* get_rotate180_matrix(matrix_t* input);

matrix_t create_matrix(int height, int width);
void destroy_matrix_data(matrix_t* matrix);

void addmat(matrix_t* src1, matrix_t* src2, matrix_t* dst);

matrix_t* correlation(matrix_t* map, matrix_t* input, int type);

matrix_t* conv(matrix_t* map, matrix_t* input, int type);

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
matrix_t* matrix_upsample(matrix_t* input, int width_multiplier, int height_multiplier);

// expaded regions filled with 0
matrix_t* matrix_copy_make_border(matrix_t* input, px_pad_t pad);

matrix_t* matrix_cut_make_border(matrix_t* input, px_pad_t pad);

void save_mat_to_file(matrix_t* mat, const char* filename);

void multifactor(matrix_t* res, matrix_t* mat, float factor);

float summat(matrix_t* mat);

#ifdef __cplusplus
}
#endif

