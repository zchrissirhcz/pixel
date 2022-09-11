// 这里库文件主要存在关于二维矩阵数组的操作
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "naive_cnn.h"

#define full 0
#define same 1
#define valid 2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct matrix_t
{
    int height;
    int width;
    float** data;
} matrix_t;

matrix_t* create_matrix(int height, int width);
void destroy_matrix(matrix_t* matrix);
matrix_t* get_rotate180_matrix(matrix_t* input);

void addmat(matrix_t* src1, matrix_t* src2, matrix_t* dst);

matrix_t* correlation(matrix_t* map, matrix_t* input, int type);

matrix_t* conv(matrix_t* map, NcSize2D mapSize, matrix_t* input, NcSize2D inSize, int type); // 卷积操作

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
matrix_t* up_sample(matrix_t* input, int upc, int upr);

// 给二维矩阵边缘扩大，增加addw大小的0值边
matrix_t* mat_edge_expand(matrix_t* input, px_pad_t pad);

// 给二维矩阵边缘缩小，擦除shrinkc大小的边
matrix_t* mat_edge_shrink(matrix_t* input, int shrinkc, int shrinkr);

void save_mat_to_file(matrix_t* mat, const char* filename);

void multifactor(matrix_t* res, matrix_t* mat, float factor);

float summat(matrix_t* mat);

#ifdef __cplusplus
}
#endif

