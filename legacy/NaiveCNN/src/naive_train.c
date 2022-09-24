#include "naive_cnn.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

NcConvolutionParam* nc_train_create_convolution_param(int in_height, int in_width, int map_size, int in_channels, int out_channels)
{
    NcConvolutionParam* param = (NcConvolutionParam*)malloc(sizeof(NcConvolutionParam));
    param->in_height = in_height;
    param->in_width = in_width;
    param->map_size = map_size;
    param->in_channels = in_channels;
    param->out_channels = out_channels;
    param->is_full_connect = true; //we use fully connect on default

    param->stride = px_create_stride(1, 1);

    // 权重空间初始化
    param->weight = nc_create_blob_random(out_channels, map_size, map_size, in_channels, -1.0f, 1.0f);

    // 权重梯度初始化
    param->d_weight = nc_create_blob_same(out_channels, map_size, map_size, in_channels, 0.0f);

    param->bias = nc_create_blob(out_channels, 1, 1, 1);

    int out_height = in_height - map_size + 1;
    int out_width = in_width - map_size + 1;
    param->out_height = out_height;
    param->out_width = out_width;

    param->d = nc_create_blob_same(out_channels, out_height, out_width, 1, 0.0f);
    param->v = nc_create_blob_same(out_channels, out_height, out_width, 1, 0.0f);
    param->y = nc_create_blob_same(out_channels, out_height, out_width, 1, 0.0f);

    return param;
}

NcPoolingParam* nc_train_create_pooling_param(int in_height, int in_width, int map_size, int in_channels, int out_channels, int pool_type)
{
    NcPoolingParam* param = (NcPoolingParam*)malloc(sizeof(NcPoolingParam));

    param->in_height = in_height;
    param->in_width = in_width;
    param->map_size = map_size;
    param->in_channels = in_channels;
    param->out_channels = out_channels;
    param->pooling_type = pool_type;

    int out_height = in_height / map_size;
    int out_width = in_width / map_size;
    param->out_height = out_height;
    param->out_width = out_width;

    param->d = nc_create_blob_same(out_channels, out_height, out_width, 1, 0.0f);
    param->y = nc_create_blob_same(out_channels, out_height, out_width, 1, 0.0f);

    return param;
}

NcInnerproductParam* nc_train_create_innerproduct_param(int in_num, int out_num)
{
    NcInnerproductParam* param = (NcInnerproductParam*)malloc(sizeof(NcInnerproductParam));

    param->in_num = in_num;
    param->out_num = out_num;

    param->in_channels = 1;
    param->out_height = 1;
    param->out_width = out_num;
    param->out_channels = 1;

    param->bias = nc_create_blob_same(out_num, 1, 1, 1, 0.0f);
    param->d = nc_create_blob_same(out_num, 1, 1, 1, 0.0f);
    param->v = nc_create_blob_same(out_num, 1, 1, 1, 0.0f);
    param->y = nc_create_blob_same(out_num, 1, 1, 1, 0.0f);

    float q = sqrtf(6.0f / (in_num + out_num));
    param->weight = nc_create_blob_same(out_num, 1, 1, in_num, q);
    param->is_full_connect = true;

    return param;
}

// allocate each layer's input blobs
// pointing each input blob to network's blob according to blob id
NcLayerInput* nc_train_create_layer_input(int n, const int* blob_ids, const NcNet* net)
{
    NcLayerInput* input = (NcLayerInput*)malloc(sizeof(NcLayerInput));
    input->blob_num = n;
    input->blobs = (NcBlob**)malloc(sizeof(NcBlob*) * n);
    input->blob_ids = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        input->blob_ids[i] = blob_ids[i];
        input->blobs[i] = net->blobs[blob_ids[i]];
    }
    return input;
}

// allocate each layer's output blobs
// each output blob is pointed by network's blob according to blob id
NcLayerOutput* nc_train_create_layer_output(int n, const int* blob_ids, const NcNet* net, NcBlob** output_blobs)
{
    NcLayerOutput* output = (NcLayerOutput*)malloc(sizeof(NcLayerOutput));
    output->blob_num = n;
    output->blobs = (NcBlob**)malloc(sizeof(NcBlob*) * n);
    output->blob_ids = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
    {
        output->blob_ids[i] = blob_ids[i];
        output->blobs[i] = output_blobs[i];
        if (NULL == net->blobs[blob_ids[i]])
        {
            net->blobs[blob_ids[i]] = output->blobs[i];
        }
    }
    return output;
}

void nc_train_forward_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output)
{
    PX_LOGE("-- Convolution forward()\n");
    NcConvolutionParam* param = (NcConvolutionParam*)param_;
    NcBlob* bottom = input->blobs[0];
    NcBlob* top = output->blobs[0];

    // now only test NHWC order
    bottom->order = NHWC; //!! important!!

    if (bottom->order == NHWC)
    {
        nc_convolution_forward_nhwc(param, bottom, top);
    }
    else if (bottom->order == NCHW)
    {
        nc_convolution_forward_nchw(param, bottom, top);
    }
    else
    {
        PX_LOGE("Error! Unsupported convolution input blob dimension order code: %d\n", bottom->order);
    }
}

void nc_train_forward_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output)
{
    PX_LOGE("-- Pooling forward()\n");
    NcPoolingParam* param = (NcPoolingParam*)param_;
}

void nc_train_forward_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output)
{
    PX_LOGE("-- Innerproduct forward()\n");
    NcInnerproductParam* param = (NcInnerproductParam*)param_;
}

void nc_train_backward_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output)
{
    PX_LOGE("-- Convolution backward()\n");
    NcConvolutionParam* param = (NcConvolutionParam*)param_;
}

void nc_train_backward_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output)
{
    PX_LOGE("-- Pooling backward()\n");
    NcPoolingParam* param = (NcPoolingParam*)param_;
}

void nc_train_backward_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output)
{
    PX_LOGE("-- Innerproduct backward()\n");
    NcInnerproductParam* param = (NcInnerproductParam*)param_;
}