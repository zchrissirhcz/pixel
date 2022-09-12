#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "px_mnist.h"
#include "cnn.h"

#ifdef __cplusplus
extern "C" {
#endif

void lenet5_setup(CNN* cnn, NcSize2D inputSize, int outputSize);

void mnist_cnn_train(CNN* cnn, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, CnnTrainOpts opts, int trainNum, FILE* fout);

float mnist_cnn_test(CNN* cnn, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, int testNum);

const char* layer_type_to_str(NcLayerType type);

void nc_cls_data_loader(NcClsDataConfig* cfg);

#ifdef __cplusplus
}
#endif
