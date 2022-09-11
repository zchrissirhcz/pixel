#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "mnist.h"

#ifdef __cplusplus
extern "C" {
#endif

void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num);

void lenet5_setup(CNN* cnn, NcSize2D inputSize,int outputSize);

void mnist_cnn_train(CNN* cnn, mnist_image_array_t* inputData, mnist_label_array_t* outputData, CNNOpts opts, int trainNum, FILE* fout);

float mnist_cnn_test(CNN* cnn, mnist_image_array_t* inputData, mnist_label_array_t* outputData, int testNum);

#ifdef __cplusplus
}
#endif
