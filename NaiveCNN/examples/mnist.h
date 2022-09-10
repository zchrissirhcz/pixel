#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "cnn.h"

typedef struct MnistImg{
    int w;           // 图像宽
    int h;           // 图像高
    float** ImgData; // 图像数据二维动态数组
} MnistImg;

typedef struct MnistImgArr{
    int ImgNum;        // 存储图像的数目
    MnistImg* ImgPtr;  // 存储图像数组指针
} MnistImgArr;         // 存储图像数据的数组

typedef struct MnistLabel{
    int l;            // 输出标记的长
    float* LabelData; // 输出标记数据
} MnistLabel;

typedef struct MnistLabelArr{
    int LabelNum;
    MnistLabel* LabelPtr;
} MnistLabelArr;  // 存储图像标记的数组

#ifdef __cplusplus
extern "C" {
#endif

void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num);

// 读入图像标记
MnistLabelArr* read_mnist_label(const char* filename);

// 读入图像
MnistImgArr* read_mnist_image(const char* filename);

void lenet5_setup(CNN* cnn, NcSize2D inputSize,int outputSize);

void mnist_cnn_train(CNN* cnn, MnistImgArr* inputData, MnistLabelArr* outputData, CNNOpts opts, int trainNum, FILE* fout);

// 测试cnn函数
float mnist_cnn_test(CNN* cnn, MnistImgArr* inputData, MnistLabelArr* outputData, int testNum);

#ifdef __cplusplus
}
#endif
