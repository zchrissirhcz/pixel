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
    tensor_t* mapData;  //存放特征模块的数据
    tensor_t* dmapData; //存放特征模块的数据的局部梯度

    array_t* biasData;    //偏置，偏置的大小，为outChannels
    bool isFullConnect; //是否为全连接
    bool* connectModel; //连接模式（默认为全连接）

    // 下面三者的大小同输出的维度相同
    cube_t* v; // 进入激活函数的输入值
    cube_t* y; // 激活函数后神经元的输出

    // 输出像素的局部梯度
    cube_t* d; // 网络的局部梯度,δ值
} ConvLayer;

typedef struct PoolingLayer
{
    int in_width;  //输入图像的宽
    int in_height; //输入图像的长
    int map_size;  //特征模板的大小

    int in_channels;  //输入图像的数目
    int out_channels; //输出图像的数目

    int pool_type;   //Pooling的方法
    array_t* biasData; //偏置

    cube_t* y; // 采样函数后神经元的输出,无激活函数
    cube_t* d; // 网络的局部梯度,δ值
} PoolingLayer;

typedef struct InnerproductLayer
{
    int inputNum;  //输入数据的数目
    int outputNum; //输出数据的数目

    float** wData;   // 权重数据，为一个inputNum*outputNum大小
    array_t* biasData; //偏置，大小为outputNum大小

    // 下面三者的大小同输出的维度相同
    array_t* v;  // 进入激活函数的输入值
    array_t* y; // 激活函数后神经元的输出
    array_t* d;  // 网络的局部梯度,δ值

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
void nnff(float* output, float* input, float** wdata, float* bias, px_size_t nn_size);

// 返回向量最大数的序号
int argmax(array_t* array);

float sigma_derivation(float y);


void clear_blob1d(float* data, int len);
void clear_blob2d(float** data, px_size_t size);
void clear_blob3d(float*** data, px_cube_dim_t cube_dim);
void clear_blob4d(float**** data, px_tensor_dim_t tensor_dim);

float* create_blob1d(int len);
float** create_blob2d(px_size_t size);
float*** create_blob3d(px_cube_dim_t cube_dim);
float**** create_blob4d(px_tensor_dim_t tensor_dim);

void destroy_blob1d(float* data);
void destroy_blob2d(float** data, px_size_t size);
void destroy_blob3d(float*** data, px_cube_dim_t cube_dim);
void destroy_blob4d(float**** data, px_tensor_dim_t tensor_dim);

void save_blob1d_to_file(float* data, int len, FILE* fout);
void save_blob2d_to_file(float** data, px_size_t size, FILE* fout);
void save_blob3d_to_file(float*** data, px_cube_dim_t cube_dim, FILE* fout);
void save_blob4d_to_file(float**** data, px_tensor_dim_t tensor_dim, FILE* fout);

array_t* create_array(int len);
void clear_array(array_t* array);
void save_array_to_file(array_t* array, FILE* fout);

cube_t* create_cube(px_cube_dim_t cube_dim);
void clear_cube(cube_t* cube);
void save_cube_to_file(cube_t* cube, FILE* fout);

tensor_t* create_tensor(px_tensor_dim_t tensor_dim);
void clear_tensor(tensor_t* tensor);

#ifdef __cplusplus
}
#endif
