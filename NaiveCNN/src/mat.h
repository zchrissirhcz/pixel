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
matrix_t* get_rotate180_matrix(matrix_t* input, NcSize2D matSize);

void addmat(matrix_t* res, matrix_t* mat1, NcSize2D matSize1, matrix_t* mat2, NcSize2D matSize2);// 矩阵相加

matrix_t* correlation(matrix_t* map, NcSize2D mapSize, matrix_t* input, NcSize2D inSize, int type);// 互相关

matrix_t* conv(matrix_t* map, NcSize2D mapSize, matrix_t* input, NcSize2D inSize, int type); // 卷积操作

// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
float** up_sample(float** mat, NcSize2D matSize, int upc, int upr);

// 给二维矩阵边缘扩大，增加addw大小的0值边
matrix_t* mat_edge_expand(matrix_t* mat, NcSize2D matSize, int addc, int addr);

// 给二维矩阵边缘缩小，擦除shrinkc大小的边
matrix_t* mat_edge_shrink(matrix_t* mat, NcSize2D matSize, int shrinkc, int shrinkr);

void savemat(float** mat, NcSize2D matSize, const char* filename);// 保存矩阵数据

void multifactor(matrix_t* res, matrix_t* mat, NcSize2D matSize, float factor);// 矩阵乘以系数

float summat(matrix_t* mat, NcSize2D matSize);// 矩阵各元素的和

#ifdef __cplusplus
}
#endif

