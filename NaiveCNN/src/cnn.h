#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "mat.h"

#define NC_AvePool 0
#define NC_MaxPool 1
#define NC_MinPool 2

typedef struct ConvLayer
{
    int in_width;  //输入图像的宽
    int in_height; //输入图像的长
    int map_size;  // kernel size

    int in_channels;  //输入图像的数目
    int out_channels; //输出图像的数目

    // 关于特征模板的权重分布，这里是一个四维数组
    // 其大小为inChannels*outChannels*mapSize*mapSize大小
    // 这里用四维数组，主要是为了表现全连接的形式，实际上卷积层并没有用到全连接的形式
    // 这里的例子是DeapLearningToolboox里的CNN例子，其用到就是全连接
    float**** mapData;  //存放特征模块的数据
    float**** dmapData; //存放特征模块的数据的局部梯度

    float* biasData;    //偏置，偏置的大小，为outChannels
    bool isFullConnect; //是否为全连接
    bool* connectModel; //连接模式（默认为全连接）

    // 下面三者的大小同输出的维度相同
    float*** v; // 进入激活函数的输入值
    float*** y; // 激活函数后神经元的输出

    // 输出像素的局部梯度
    float*** d; // 网络的局部梯度,δ值
} ConvLayer;

typedef struct PoolingLayer
{
    int in_width;  //输入图像的宽
    int in_height; //输入图像的长
    int map_size;  //特征模板的大小

    int in_channels;  //输入图像的数目
    int out_channels; //输出图像的数目

    int pool_type;   //Pooling的方法
    float* biasData; //偏置

    float*** y; // 采样函数后神经元的输出,无激活函数
    float*** d; // 网络的局部梯度,δ值
} PoolingLayer;

typedef struct InnerproductLayer
{
    int inputNum;  //输入数据的数目
    int outputNum; //输出数据的数目

    float** wData;   // 权重数据，为一个inputNum*outputNum大小
    float* biasData; //偏置，大小为outputNum大小

    // 下面三者的大小同输出的维度相同
    float* v; // 进入激活函数的输入值
    float* y; // 激活函数后神经元的输出
    float* d; // 网络的局部梯度,δ值

    bool isFullConnect; //是否为全连接
} InnerproductLayer;

#ifdef __cplusplus
extern "C" {
#endif

ConvLayer* init_conv_layer(int in_width, int in_height, int map_size, int in_channels, int out_channels);
PoolingLayer* init_pooling_layer(int in_width, int in_height, int map_size, int in_channels, int out_channels, int pool_type);
InnerproductLayer* init_innerproduct_layer(int in_num, int out_num);

void forward_avg_pooling_for_matrix(matrix_t* input, matrix_t* output, px_size_t kernel_size);

// 激活函数 input是数据，inputNum说明数据数目，bas表明偏置
float activation_sigma(float input, float bias); // sigma激活函数

// 单层全连接神经网络的前向传播
void nnff(float* output, float* input, float** wdata, float* bias, NcSize2D nn_size);

// 返回向量最大数的序号
int argmax(float* data, int len);

float sigma_derivation(float y);

const char* layer_type_to_str(NcLayerType type);

#ifdef __cplusplus
}
#endif
