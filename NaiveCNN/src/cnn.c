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
    ConvLayer* covL = (ConvLayer*)malloc(sizeof(ConvLayer));

    covL->in_height = in_height;
    covL->in_width = in_width;
    covL->map_size = map_size;

    covL->in_channels = in_channels;
    covL->out_channels = out_channels;

    covL->isFullConnect = true; // 默认为全连接

    // 权重空间的初始化，先行再列调用，[r][c]
    int i, j, c, r;
    srand((unsigned)time(NULL));
    covL->mapData = (float****)malloc(in_channels * sizeof(float***));
    for (i = 0; i < in_channels; i++)
    {
        covL->mapData[i] = (float***)malloc(out_channels * sizeof(float**));
        for (j = 0; j < out_channels; j++)
        {
            covL->mapData[i][j] = (float**)malloc(map_size * sizeof(float*));
            for (r = 0; r < map_size; r++)
            {
                covL->mapData[i][j][r] = (float*)malloc(map_size * sizeof(float));
                for (c = 0; c < map_size; c++)
                {
                    float randnum = (((float)rand() / (float)RAND_MAX) - 0.5) * 2;
                    covL->mapData[i][j][r][c] = randnum * sqrt((float)6.0 / (float)(map_size * map_size * (in_channels + out_channels)));
                }
            }
        }
    }
    // 权重梯度变化
    covL->dmapData = (float****)malloc(in_channels * sizeof(float***));
    for (i = 0; i < in_channels; i++)
    {
        covL->dmapData[i] = (float***)malloc(out_channels * sizeof(float**));
        for (j = 0; j < out_channels; j++)
        {
            covL->dmapData[i][j] = (float**)malloc(map_size * sizeof(float*));
            for (r = 0; r < map_size; r++)
            {
                covL->dmapData[i][j][r] = (float*)calloc(map_size, sizeof(float));
            }
        }
    }

    covL->biasData = (float*)calloc(out_channels, sizeof(float));

    int outW = in_width - map_size + 1;
    int outH = in_height - map_size + 1;

    covL->d = (float***)malloc(out_channels * sizeof(float**));
    covL->v = (float***)malloc(out_channels * sizeof(float**));
    covL->y = (float***)malloc(out_channels * sizeof(float**));
    for (j = 0; j < out_channels; j++)
    {
        covL->d[j] = (float**)malloc(outH * sizeof(float*));
        covL->v[j] = (float**)malloc(outH * sizeof(float*));
        covL->y[j] = (float**)malloc(outH * sizeof(float*));
        for (r = 0; r < outH; r++)
        {
            covL->d[j][r] = (float*)calloc(outW, sizeof(float));
            covL->v[j][r] = (float*)calloc(outW, sizeof(float));
            covL->y[j][r] = (float*)calloc(outW, sizeof(float));
        }
    }

    return covL;
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

    poolL->biasData = (float*)calloc(outChannels, sizeof(float));

    int outW = inputWidth / mapSize;
    int outH = inputHeight / mapSize;

    int j, r;
    poolL->d = (float***)malloc(outChannels * sizeof(float**));
    poolL->y = (float***)malloc(outChannels * sizeof(float**));
    for (j = 0; j < outChannels; j++)
    {
        poolL->d[j] = (float**)malloc(outH * sizeof(float*));
        poolL->y[j] = (float**)malloc(outH * sizeof(float*));
        for (r = 0; r < outH; r++)
        {
            poolL->d[j][r] = (float*)calloc(outW, sizeof(float));
            poolL->y[j][r] = (float*)calloc(outW, sizeof(float));
        }
    }

    return poolL;
}

InnerproductLayer* init_innerproduct_layer(int inputNum, int outputNum)
{
    InnerproductLayer* outL = (InnerproductLayer*)malloc(sizeof(InnerproductLayer));

    outL->inputNum = inputNum;
    outL->outputNum = outputNum;

    outL->biasData = (float*)calloc(outputNum, sizeof(float));

    outL->d = (float*)calloc(outputNum, sizeof(float));
    outL->v = (float*)calloc(outputNum, sizeof(float));
    outL->y = (float*)calloc(outputNum, sizeof(float));

    // 权重的初始化
    outL->wData = (float**)malloc(outputNum * sizeof(float*)); // 输入行，输出列
    int i, j;
    srand((unsigned)time(NULL));
    for (i = 0; i < outputNum; i++)
    {
        outL->wData[i] = (float*)malloc(inputNum * sizeof(float));
        for (j = 0; j < inputNum; j++)
        {
            float randnum = (((float)rand() / (float)RAND_MAX) - 0.5) * 2; // 产生一个-1到1的随机数
            outL->wData[i][j] = randnum * sqrt((float)6.0 / (float)(inputNum + outputNum));
        }
    }

    outL->isFullConnect = true;

    return outL;
}

int argmax(float* data, int len)
{
    float max_value = FLT_MIN;
    int max_index = 0;
    for (int i = 0; i < len; i++)
    {
        if (max_value < data[i])
        {
            max_value = data[i];
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

void forward_avg_pooling(matrix_t* input, matrix_t* output, px_size_t kernel_size)
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

void nnff(float* output, float* input, float** wdata, float* bas, NcSize2D nnSize)
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

const char* layer_type_to_str(NcLayerType type)
{
    switch (type)
    {
    case NC_LAYER_DATA:
        return "NC_DATA";
    case NC_LAYER_CONVOLUTION:
        return "NC_CONVOLUTION";
    case NC_LAYER_POOLING:
        return "NC_POOLING";
    case NC_LAYER_INNERPRODUCT:
        return "NC_INNERPRODUCT";
    case NC_LAYER_LOSS:
        return "NC_LOSS";
    default:
        return "UNKNOWN";
    }
}