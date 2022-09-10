#include "naive_cnn.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "naive_convolution.h"
#include "naive_pooling.h"
#include "naive_relu.h"

// make convolution param for inference
NcConvolutionParam* nc_infer_make_convolution_param(int map_size, int in_channels, int out_channels, NcPaddingType pad_type){
    NcConvolutionParam* param = (NcConvolutionParam*)malloc(sizeof(NcConvolutionParam));
    param->map_size = map_size;
    param->in_channels = in_channels;
    param->out_channels = out_channels;
    param->stride = nc_stride_make(1, 1); //now only use (1,1) stride
    param->weight = nc_blob_make_random(out_channels, map_size, map_size, in_channels, -1.0f, 1.0f);
    param->bias = nc_blob_make(out_channels, 1, 1, 1);

    param->padding_type = pad_type;

    // the following are not set but will used during infernce, make them -233
    param->in_height = -233;
    param->in_width = -233;
    param->out_height = -233;
    param->out_width = -233;

    // the following are not used during inference, make them 0/null/false
    param->d_weight = NULL;
    param->d = NULL;
    param->v = NULL;
    param->y = NULL;
    param->is_full_connect = false;

    return param;
}

NcPoolingParam* nc_infer_make_pooling_param(int map_size, int in_channels, NcPoolingType pool_type, NcPaddingType pad_type) {
    NcPoolingParam* param = (NcPoolingParam*)malloc(sizeof(NcPoolingParam));
    param->in_channels = in_channels;
    param->out_channels = in_channels;
    param->stride = nc_stride_make(1, 1); //now only use (1,1) stride
    param->map_size = map_size;
    param->pooling_type = pool_type;
    param->padding_type = pad_type;
    
    // the following are not set but will used during infernce, make them -233
    param->in_height = -233;
    param->in_width = -233;
    param->out_height = -233;
    param->out_width = -233;

    // the following are not used during inference, make them 0/null/false
    param->d = NULL;
    param->y = NULL;

    return param;
}

NcInnerproductParam* nc_infer_make_innerproduct_param(int in_num, int out_num) {
    NcInnerproductParam* param = (NcInnerproductParam*)malloc(sizeof(NcInnerproductParam));

    param->in_num = in_num;
    param->out_num = out_num;

    param->in_channels = 1;
    param->out_height = 1;
    param->out_width = out_num;
    param->out_channels = 1;

    param->bias = nc_blob_make_same(out_num, 1, 1, 1, 0.0f);

    float q = sqrtf(6.0f / (in_num + out_num));
    param->weight = nc_blob_make_same(out_num, 1, 1, in_num, q);
    param->is_full_connect = true;

    // the following are not used during inference, make them 0/null/false
    param->d = NULL;
    param->v = NULL;
    param->y = NULL;

    return param;
}



// allocate each layer's input blobs
// pointing each input blob to network's blob according to blob id
NcLayerInput* nc_infer_make_layer_input(int n, const int* blob_ids, NcNet* net) {
    NcLayerInput* input = (NcLayerInput*)malloc(sizeof(NcLayerInput));
    input->blob_num = n;
    input->blobs = (NcBlob**)malloc(sizeof(NcBlob*)*n);
    input->blob_ids = (int*)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++) {
        input->blob_ids[i] = blob_ids[i];
        input->blobs[i] = net->blobs[blob_ids[i]];
        net->blobs[blob_ids[i]]->rely_cnt_total++; //mark as relied
    }
    return input;
}

// allocate each layer's output blobs
// each output blob is pointed by network's blob according to blob id
NcLayerOutput* nc_infer_make_layer_output(int n, const int* blob_ids, NcNet* net) {
    NcLayerOutput* output = (NcLayerOutput*)malloc(sizeof(NcLayerOutput));
    output->blob_num = n;
    output->blobs = (NcBlob**)malloc(sizeof(NcBlob*)*n);
    output->blob_ids = (int*)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++) {
        output->blob_ids[i] = blob_ids[i];
        output->blobs[i] = net->blobs[blob_ids[i]];
    }
    return output;
}

// for each infer function, it should contain 3 steps:
// 1. calculate output blob's data size, allocate output blob data if necessary (usually needed)
// 2. do computation
// 3. decrease input blob's rely count, and release blob if rely count==0
// note! for in-place operation, needn't considering the rely_cnt
void nc_infer_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output) {
    printf("===> into NaiveCNN infer convolution\n");
    // step1
    NcConvolutionParam* param = (NcConvolutionParam*)param_;
    NcBlob* bottom = input->blobs[0];
    NcBlob* top = output->blobs[0];

    param->in_height = bottom->h;
    param->in_width = bottom->w;
    param->out_height = param->in_height - param->map_size + 1;
    param->out_width = param->in_width - param->map_size + 1;

    nc_blob_data_realloc3d(top, param->out_height, param->out_width, param->out_channels);

    // step2
    if (bottom->order == NHWC) {
        nc_convolution_forward_nhwc(param, bottom, top);
    }
    else if(bottom->order==NCHW) {
        nc_convolution_forward_nchw(param, bottom, top);
    }

    // step3
    bottom->rely_cnt--;
    if (bottom->rely_cnt == 0) {
        free(bottom->data);
        bottom->data = NULL;
    }

    printf("-- leave NaiveCNN infer convolution\n");
}

void nc_infer_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output) {
    printf("===> into NaiveCNN infer pooling\n");
    // step1
    NcPoolingParam* param = (NcPoolingParam*)param_;
    NcBlob* bottom = input->blobs[0];
    NcBlob* top = output->blobs[0];

    param->in_height = bottom->h;
    param->in_width = bottom->w;
    param->out_height = (param->in_height - param->map_size) / param->stride->h + 1;
    param->out_width = (param->in_width - param->map_size) / param->stride->w + 1;

    nc_blob_data_realloc3d(top, param->out_height, param->out_width, param->out_channels);

    // step2
    if (bottom->order == NHWC) {
        nc_pooling_forward_nhwc(param, bottom, top);
    }
    else if (bottom->order == NCHW) {
        nc_pooling_forward_nchw(param, bottom, top);
    }

    // step3
    bottom->rely_cnt--;
    if (bottom->rely_cnt == 0) {
        free(bottom->data);
        bottom->data = NULL;
    }

    printf("-- leave NaiveCNN infer pooling\n");
}

void nc_infer_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output) {
    printf("===> into NaiveCNN infer innerproduct\n");
    NcInnerproductParam* param = (NcInnerproductParam*)param_;

    printf("-- leave NaiveCNN infer innerproduct\n");
}
