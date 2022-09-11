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

typedef struct mnist_label_array_t
{
    int size;
    mnist_label_t* labels;
} mnist_label_array_t;

#ifdef __cplusplus
extern "C" {
#endif

void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num);

mnist_label_array_t* read_mnist_label(const char* filename);

mnist_image_array_t* read_mnist_image(const char* filename);

void lenet5_setup(CNN* cnn, NcSize2D inputSize,int outputSize);

void mnist_cnn_train(CNN* cnn, mnist_image_array_t* inputData, mnist_label_array_t* outputData, CNNOpts opts, int trainNum, FILE* fout);

// 测试cnn函数
float mnist_cnn_test(CNN* cnn, mnist_image_array_t* inputData, mnist_label_array_t* outputData, int testNum);

#ifdef __cplusplus
}
#endif
