#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "cnn.h"

typedef matrix_t mnist_image_t;

typedef struct mnist_image_array_t
{
    int size;
    mnist_image_t* images;
} mnist_image_array_t;

typedef px_array_t mnist_label_t;

typedef struct MnistLabelArr
{
    int LabelNum;
    mnist_label_t* LabelPtr;
} MnistLabelArr;  // 存储图像标记的数组

#ifdef __cplusplus
extern "C" {
#endif

void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num);

// 读入图像标记
MnistLabelArr* read_mnist_label(const char* filename);

// 读入图像
mnist_image_array_t* read_mnist_image(const char* filename);

void lenet5_setup(CNN* cnn, NcSize2D inputSize,int outputSize);

void mnist_cnn_train(CNN* cnn, mnist_image_array_t* inputData, MnistLabelArr* outputData, CNNOpts opts, int trainNum, FILE* fout);

// 测试cnn函数
float mnist_cnn_test(CNN* cnn, mnist_image_array_t* inputData, MnistLabelArr* outputData, int testNum);

#ifdef __cplusplus
}
#endif
