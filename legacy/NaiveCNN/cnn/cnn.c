#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "px_cnn.h"
#include "px_log.h"
#include "cnn.h"
#include "mat.h"
#include <float.h>


px_size_t get_conv_output_size(px_size_t in_size, px_size_t kernel_size, px_size_t stride, px_pad_t pad)
{
    int out_height = (in_size.height - kernel_size.height + pad.top + pad.bottom) / stride.height + 1;
    int out_width = (in_size.width - kernel_size.width + pad.left + pad.right) / stride.width + 1;
    px_size_t out_size = px_create_size(out_height, out_width);
    return out_size;
}

px_size_t get_pooling_out_size(px_size_t in_size, px_size_t kernel_size, px_size_t stride, px_pad_t pad)
{
    int out_height = (in_size.height - kernel_size.height + pad.top + pad.bottom) / stride.height + 1;
    int out_width = (in_size.width - kernel_size.width + pad.left + pad.right) / stride.height + 1;
    px_size_t out_size = px_create_size(out_height, out_width);
    return out_size;
}

ConvLayer* init_conv_layer(px_size_t in_size, int map_size, px_size_t stride, px_pad_t pad, int in_channels, int out_channels)
{
    ConvLayer* conv_layer = (ConvLayer*)malloc(sizeof(ConvLayer));
    memset(conv_layer, 0, sizeof(ConvLayer));

    conv_layer->in_height = in_size.height;
    conv_layer->in_width = in_size.width;
    conv_layer->map_size = map_size;

    conv_layer->in_channels = in_channels;
    conv_layer->out_channels = out_channels;

    conv_layer->is_fully_connected = true; // 默认为全连接

    conv_layer->stride_height = stride.height;
    conv_layer->stride_width = stride.width;

    conv_layer->pad_bottom = pad.bottom;
    conv_layer->pad_top = pad.top;
    conv_layer->pad_left = pad.left;
    conv_layer->pad_right = pad.right;

    // 权重空间的初始化，先行再列调用，[r][c]
    int i, j, c, r;
    
    px_tensor_dim_t tensor_dim = px_create_tensor_dim(in_channels, out_channels, map_size, map_size);
    conv_layer->kernel = create_tensor(tensor_dim);

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
                    conv_layer->kernel->data[i][j][r][c] = randnum * sqrt((float)6.0 / (float)(map_size * map_size * (in_channels + out_channels)));
                }
            }
        }
    }

    // 权重梯度变化
    conv_layer->d_kernel = create_tensor(tensor_dim);
    clear_tensor(conv_layer->d_kernel);

    conv_layer->bias = create_array(out_channels);

    px_size_t kernel_size = px_create_size(map_size, map_size);
    px_size_t out_size = get_conv_output_size(in_size, kernel_size, stride, pad);
    conv_layer->out_height = out_size.height;
    conv_layer->out_width = out_size.width;

    px_cube_dim_t cube_dim = px_create_cube_dim(out_channels, out_size.height, out_size.width);
    conv_layer->d = create_cube(cube_dim);;
    conv_layer->v = create_cube(cube_dim);
    conv_layer->y = create_cube(cube_dim);

    clear_cube(conv_layer->d);
    clear_cube(conv_layer->v);
    clear_cube(conv_layer->y);

    return conv_layer;
}

PoolingLayer* init_pooling_layer(px_size_t in_size, int mapSize, px_size_t stride, px_pad_t pad, int inChannels, int outChannels, int pool_type)
{
    PoolingLayer* pool_layer = (PoolingLayer*)malloc(sizeof(PoolingLayer));
    memset(pool_layer, 0, sizeof(PoolingLayer));

    pool_layer->in_height = in_size.height;
    pool_layer->in_width = in_size.width;
    pool_layer->map_size = mapSize;
    pool_layer->in_channels = inChannels;
    pool_layer->out_channels = outChannels;
    pool_layer->pool_type = pool_type;

    pool_layer->bias = create_array(outChannels);

    pool_layer->stride_height = stride.height;
    pool_layer->stride_width = stride.width;

    pool_layer->pad_bottom = pad.bottom;
    pool_layer->pad_top = pad.top;
    pool_layer->pad_left = pad.left;
    pool_layer->pad_right = pad.right;

    px_size_t kernel_size = px_create_size(mapSize, mapSize);
    px_size_t out_size = get_pooling_out_size(in_size, kernel_size, stride, pad);
    pool_layer->out_height = out_size.height;
    pool_layer->out_width = out_size.width;

    px_cube_dim_t cube_dim = px_create_cube_dim(outChannels, out_size.height, out_size.width);
    pool_layer->d = create_cube(cube_dim);
    pool_layer->y = create_cube(cube_dim);
    clear_cube(pool_layer->d);
    clear_cube(pool_layer->y);

    return pool_layer;
}

InnerproductLayer* init_innerproduct_layer(int inputNum, int outputNum)
{
    InnerproductLayer* ip_layer = (InnerproductLayer*)malloc(sizeof(InnerproductLayer));

    ip_layer->input_num = inputNum;
    ip_layer->output_num = outputNum;

    ip_layer->bias = create_array(outputNum);
    ip_layer->d = create_array(outputNum);
    ip_layer->v = create_array(outputNum);
    ip_layer->y = create_array(outputNum);

    clear_array(ip_layer->bias);
    clear_array(ip_layer->d);
    clear_array(ip_layer->v);
    clear_array(ip_layer->y);

    // 权重的初始化
    px_size_t size = px_create_size(outputNum, inputNum);
    ip_layer->weight = create_matrix(size);

    srand((unsigned)time(NULL));
    for (int i = 0; i < outputNum; i++)
    {
        for (int j = 0; j < inputNum; j++)
        {
            float randnum = (((float)rand() / (float)RAND_MAX) - 0.5) * 2; // 产生一个-1到1的随机数
            ip_layer->weight->data[i][j] = randnum * sqrt((float)6.0 / (float)(inputNum + outputNum));
        }
    }

    ip_layer->is_fully_connected = true;

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

float activation_sigma(float input, float bias)
{
    float temp = input + bias;
    return (float)1.0 / ((float)(1.0 + exp(-temp)));
}

void forward_avg_pooling_for_matrix(matrix_t* input, matrix_t* output, px_size_t kernel_size, px_size_t stride, px_pad_t pad)
{
    px_size_t in_size = px_create_size(input->height, input->width);
    px_size_t out_size = get_pooling_out_size(in_size, kernel_size, stride, pad);
    if (output->width != out_size.width || output->height != out_size.height)
    {
        PX_LOGE("ERROR: output size is wrong!!");
        return;
    }

    for (int i = 0; i < output->height; i++)
    {
        for (int j = 0; j < output->width; j++)
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



void destroy_conv_layer(ConvLayer* conv_layer)
{
    destroy_tensor(conv_layer->kernel);
    destroy_tensor(conv_layer->d_kernel);
    destroy_array(conv_layer->bias);
    destroy_cube(conv_layer->d);
    destroy_cube(conv_layer->v);
    destroy_cube(conv_layer->y);
    free(conv_layer);
}

void destroy_pooling_layer(PoolingLayer* pool_layer)
{
    destroy_array(pool_layer->bias);
    destroy_cube(pool_layer->d);
    destroy_cube(pool_layer->y);
    free(pool_layer);
}

void destroy_innerproduct_layer(InnerproductLayer* ip_layer)
{
    destroy_array(ip_layer->bias);
    destroy_array(ip_layer->d);
    destroy_array(ip_layer->v);
    destroy_array(ip_layer->y);

    destroy_matrix(ip_layer->weight);
}
