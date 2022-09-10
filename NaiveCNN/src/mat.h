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

//---------------------------------------------------------------------
// function declarations
//---------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
    
	float** rotate180(float** mat, NcSize2D matSize);// 矩阵翻转180度

	void addmat(float** res, float** mat1, NcSize2D matSize1, float** mat2, NcSize2D matSize2);// 矩阵相加

	float** correlation(float** map, NcSize2D mapSize, float** inputData, NcSize2D inSize, int type);// 互相关

	float** conv(float** map, NcSize2D mapSize, float** inputData, NcSize2D inSize, int type); // 卷积操作

	// 这个是矩阵的上采样（等值内插），upc及upr是内插倍数
	float** up_sample(float** mat, NcSize2D matSize, int upc, int upr);

	// 给二维矩阵边缘扩大，增加addw大小的0值边
	float** mat_edge_expand(float** mat, NcSize2D matSize, int addc, int addr);

	// 给二维矩阵边缘缩小，擦除shrinkc大小的边
	float** mat_edge_shrink(float** mat, NcSize2D matSize, int shrinkc, int shrinkr);

	void savemat(float** mat, NcSize2D matSize, const char* filename);// 保存矩阵数据

	void multifactor(float** res, float** mat, NcSize2D matSize, float factor);// 矩阵乘以系数

	float summat(float** mat, NcSize2D matSize);// 矩阵各元素的和

#ifdef __cplusplus
}
#endif

