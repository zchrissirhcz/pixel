#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "px_log.h"
#include "cnn.h"
#include "mat.h"
#include <float.h>



ConvLayer* init_conv_layer(int in_width, int in_height, int map_size, int in_channels, int out_channels)
{
    ConvLayer* conv_layer = (ConvLayer*)malloc(sizeof(ConvLayer));

    conv_layer->in_height = in_height;
    conv_layer->in_width = in_width;
    conv_layer->map_size = map_size;

    conv_layer->in_channels = in_channels;
    conv_layer->out_channels = out_channels;

    conv_layer->isFullConnect = true; // 默认为全连接

    // 权重空间的初始化，先行再列调用，[r][c]
    int i, j, c, r;
    
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(in_channels, out_channels, map_size, map_size);
    conv_layer->mapData = create_blob4d(tensor_dim);

    srand((unsigned)time(NULL));
    for (i = 0; i < in_channels; i++)
    {
        for (j = 0; j < out_channels; j++)
        {
            for (r = 0; r < map_size; r++)
            {
                for (c = 0; c < map_size; c++)
                {
                    float randnum = (((float)rand() / (float)RAND_MAX) - 0.5) * 2;
                    conv_layer->mapData[i][j][r][c] = randnum * sqrt((float)6.0 / (float)(map_size * map_size * (in_channels + out_channels)));
                }
            }
        }
    }

    // 权重梯度变化
    conv_layer->dmapData = create_blob4d(tensor_dim);
    clear_blob4d(conv_layer->dmapData, tensor_dim);

    conv_layer->biasData = create_array(out_channels);

    int outW = in_width - map_size + 1;
    int outH = in_height - map_size + 1;

    px_cube_dim_t cube_dim = px_create_cube_dim(out_channels, outH, outW);
    conv_layer->d = create_blob3d(cube_dim);
    conv_layer->v = create_blob3d(cube_dim);
    conv_layer->y = create_blob3d(cube_dim);

    clear_blob3d(conv_layer->d, cube_dim);
    clear_blob3d(conv_layer->v, cube_dim);
    clear_blob3d(conv_layer->y, cube_dim);

    return conv_layer;
}

PoolingLayer* init_pooling_layer(int inputWidth, int inputHeight, int mapSize, int inChannels, int outChannels, int pool_type)
{
    PoolingLayer* poolL = (PoolingLayer*)malloc(sizeof(PoolingLayer));

    poolL->in_height = inputHeight;
    poolL->in_width = inputWidth;
    poolL->map_size = mapSize;
    poolL->in_channels = inChannels;
    poolL->out_channels = outChannels;
    poolL->pool_type = pool_type;

    poolL->biasData = create_array(outChannels);

    int outW = inputWidth / mapSize;
    int outH = inputHeight / mapSize;

    int j, r;
    px_cube_dim_t cube_dim = px_create_cube_dim(outChannels, outH, outW);
    poolL->d = create_blob3d(cube_dim);
    poolL->y = create_blob3d(cube_dim);
    clear_blob3d(poolL->d, cube_dim);
    clear_blob3d(poolL->y, cube_dim);

    return poolL;
}

InnerproductLayer* init_innerproduct_layer(int inputNum, int outputNum)
{
    InnerproductLayer* ip_layer = (InnerproductLayer*)malloc(sizeof(InnerproductLayer));

    ip_layer->inputNum = inputNum;
    ip_layer->outputNum = outputNum;

    ip_layer->biasData = create_array(outputNum);
    ip_layer->d = create_array(outputNum);
    ip_layer->v = create_array(outputNum);
    ip_layer->y = create_array(outputNum);

    clear_array(ip_layer->biasData);
    clear_array(ip_layer->d);
    clear_array(ip_layer->v);
    clear_array(ip_layer->y);

    // 权重的初始化
    px_size_t size = px_create_size(outputNum, inputNum);
    ip_layer->wData = create_blob2d(size);

    srand((unsigned)time(NULL));
    for (int i = 0; i < outputNum; i++)
    {
        for (int j = 0; j < inputNum; j++)
        {
            float randnum = (((float)rand() / (float)RAND_MAX) - 0.5) * 2; // 产生一个-1到1的随机数
            ip_layer->wData[i][j] = randnum * sqrt((float)6.0 / (float)(inputNum + outputNum));
        }
    }

    ip_layer->isFullConnect = true;

    return ip_layer;
}

int argmax(array_t* array)
{
    float max_value = FLT_MIN;
    int max_index = 0;
    for (int i = 0; i < array->len; i++)
    {
        if (max_value < array->data[i])
        {
            max_value = array->data[i];
            max_index = i;
        }
    }
    return max_index;
}

// 激活函数 input是数据，inputNum说明数据数目，bias表明偏置
float activation_sigma(float input, float bias) // sigma激活函数
{
    float temp = input + bias;
    return (float)1.0 / ((float)(1.0 + exp(-temp)));
}

void forward_avg_pooling_for_matrix(matrix_t* input, matrix_t* output, px_size_t kernel_size)
{
    int outputW = input->width / kernel_size.width;
    int outputH = input->height / kernel_size.height;
    if (output->width != outputW || output->height != outputH)
    {
        PX_LOGE("ERROR: output size is wrong!!");
        return;
    }

    for (int i = 0; i < outputH; i++)
    {
        for (int j = 0; j < outputW; j++)
        {
            float sum = 0.0f;
            for (int m = i * kernel_size.height; m < (i + 1) * kernel_size.height; m++)
            {
                for (int n = j * kernel_size.width; n < (j + 1) * kernel_size.width; n++)
                {
                    sum = sum + input->data[m][n];
                }
            }
            output->data[i][j] = sum / (float)(kernel_size.height * kernel_size.width);
        }
    }
}

float dot_product(float* vec1, float* vec2, int vec_length)
{
    float res = 0.f;
    for (int i = 0; i < vec_length; i++)
    {
        res += vec1[i] * vec2[i];
    }
    return res;
}

void nnff(float* output, float* input, float** wdata, float* bas, px_size_t nnSize)
{
    const int w = nnSize.width;
    const int h = nnSize.height;

    for (int i = 0; i < h; i++)
    {
        output[i] = dot_product(input, wdata[i], w) + bas[i];
    }
}

// Logic激活函数的自变量微分
float sigma_derivation(float y)
{
    return y * (1 - y); // 这里y是指经过激活函数的输出值，而不是自变量
}


array_t* create_array(int len)
{
    float* data = create_blob1d(len);
    array_t* array = (array_t*)malloc(sizeof(array_t));
    array->len = len;
    array->data = data;
    return array;
}

void clear_array(array_t* array)
{
    clear_blob1d(array->data, array->len);
}

void save_array_to_file(array_t* array, FILE* fout)
{
    save_blob1d_to_file(array->data, array->len, fout);
}