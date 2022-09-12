#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "px_mnist.h"
#include "cnn.h"

typedef struct Lenet
{
    int layerNum;
    ConvLayer* C1;
    PoolingLayer* S2;
    ConvLayer* C3;
    PoolingLayer* S4;
    InnerproductLayer* O5;

    float* err; // training error
    float* L; // 瞬时误差能量
} Lenet;

typedef struct LenetTrainOpts
{
    int num_epochs; // 训练的迭代次数
    float lr;   // learning rate
} LenetTrainOpts;

#ifdef __cplusplus
extern "C" {
#endif

void setup_lenet(Lenet* net, NcSize2D inputSize, int outputSize);

void train_lenet_on_mnist(Lenet* net, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, LenetTrainOpts opts, int trainNum, FILE* fout);
float test_lenet_on_mnist(Lenet* net, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, int testNum);

void load_lenet(Lenet* net, const char* filename);

void forward_lenet(Lenet* net, matrix_t* input_data);
void backward_lenet(Lenet* net, float* output_data);
void apply_grads_on_lenet(Lenet* net, LenetTrainOpts opts, matrix_t* input);
void clear_lenet(Lenet* net); // 将数据vyd清零

void save_lenet_train_data(Lenet* lenet, const char* filename, float** inputdata);
void save_lenet_inference_data(Lenet* net, const char* filename);

#ifdef __cplusplus
}
#endif
