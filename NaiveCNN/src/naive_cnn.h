#pragma once

#include <stdbool.h>
#include "naive_config.h"

typedef enum NcPaddingType {
    NC_PADDING_CONV_CAFFE = 0,
    NC_PADDING_CONV_TF_SAME = 1,
    NC_PADDING_CONV_TF_VALID = 2,
    NC_PADDING_POOL_CAFFE = 3,
    NC_PADDING_POOL_TF_SAME = 4,
    NC_PADDING_POOL_TF_VALID = 5
} NcPaddingType;

typedef enum NcPoolingType {
    NC_POOLING_AVERAGE = 0,
    NC_POOLING_MAX = 1
} NcPoolingType;

typedef enum NcLayerType {
    NC_LAYER_DATA=0,
    NC_LAYER_CONVOLUTION=1,
    NC_LAYER_POOLING=2,
    NC_LAYER_INNERPRODUCT=3,
    NC_LAYER_LOSS=4,
    NC_LAYER_UNKNOWN=404
} NcLayerType;

typedef enum NcBlobDimOrder {
    NCHW=0,
    NHWC=1
} NcBlobDimOrder;

typedef struct NcBlob {
    int n, h, w, c;
    int nstep; //h*w*c
    int mass; //n*nstep=n*h*w*c
    int rely_cnt_total; //how many blobs rely on this blob, before the inference run
    int rely_cnt; //for the current inference layer, how many blobs still rely on this blob
    NcBlobDimOrder order;
    float* data;
} NcBlob;

typedef struct NcBlob2D {
    int h, w;
    float* data;
} NcBlob2D;

typedef struct NcDim2D {
    int h, w;
} NcDim2D;

typedef NcDim2D NcStride;


typedef struct NcConvolutionParam {
    int in_height;
    int in_width;
    int in_channels;
    int out_channels;
    int out_height;
    int out_width;

    NcPaddingType padding_type;

    int map_size;
    NcStride* stride;

    bool is_full_connect;

    NcBlob* weight;
    NcBlob* d_weight;
    NcBlob* bias;

    // 下面三者的大小同输出的维度相同
    NcBlob* v; // 进入激活函数的输入值
    NcBlob* y; // 激活函数后神经元的输出

    // 输出像素的局部梯度
    NcBlob* d; // 网络的局部梯度,δ值
} NcConvolutionParam;

typedef struct NcPoolingParam {
    int bottom_blobs_num;
    int top_blobs_num;

    int in_height;
    int in_width;
    int map_size;
    int in_channels;
    int out_channels;
    int out_height;
    int out_width;
    NcPoolingType pooling_type;
    NcPaddingType padding_type;

    NcStride* stride;

    NcBlob* d;
    NcBlob* y;
} NcPoolingParam;

typedef struct NcInnerproductParam {
    int in_num;
    int out_num;

    int in_channels;
    int out_height;
    int out_width;
    int out_channels;

    NcBlob* bias;

    NcBlob* d;
    NcBlob* v;
    NcBlob* y;

    NcBlob* weight;

    bool is_full_connect;
} NcInnerproductParam;

typedef enum NC_RELU_TYPE {
    RELU=0,
    RELU6=1
} NC_RELU_TYPE;

typedef struct NcReluParam {
    NC_RELU_TYPE type;
} NcReluParam;

typedef struct NcLayerBottomTop {
    int blob_num;
    NcBlob** blobs;
    int* blob_ids;
} NcLayerInput, NcLayerOutput;

//forward and backward: for training
typedef void (*NcLayerForward)(void* param, NcLayerInput* input, NcLayerOutput* output);
typedef void (*NcLayerBackward)(void* param, NcLayerInput* input, NcLayerOutput* output);
// infer: for inference
typedef void(*NcLayerInfer)(void* param, NcLayerInput* input, NcLayerOutput* output);

typedef struct NcLayer {
    NcLayerType type;
    void* param;

    NcLayerForward forward;
    NcLayerBackward backward;
    NcLayerInfer infer;

    NcLayerInput* input;
    NcLayerOutput* output;
} NcLayer;

typedef struct NcNet{
    int layers_num;
    NcLayer** layers;

    int blobs_num;
    NcBlob** blobs;

    // the following members are for training only:
    float* e; // 训练误差
    float* L; // 瞬时误差能量
} NcNet;

typedef struct NcSize2D {
    int w; // width
    int h; // height
} NcSize2D;

//NaiveCNN's Image struct
typedef struct NcImage {
    int w, h, c;
    int linebytes;
    int elem_num;
    unsigned char* data;
} NcImage;

typedef struct NcTrainConfig {
    int num_epoch;
    float alpha;
    int train_num;
    char log_pth[NC_MAX_PATH];
} NcTrainConfig;

typedef struct NcClsDataConfig {
    int num; // how many (x,y) pairs do we have?
    NcImage** images;
    int* labels;
    char dataset[NC_MAX_PATH];
    char splitset[NC_MAX_PATH];
} NcClsDataConfig;

#ifdef __cplusplus
extern "C" {
#endif

// create image
NcImage* nc_make_image(int h, int w, int c, unsigned char* data);

// create empty image
NcImage* nc_make_empty_image(int h, int w, int c);

float nc_get_random_float(float s, float t);
NcBlob* nc_blob_make_empty(int n, int h, int w, int c);
NcBlob* nc_blob_make(int n, int h, int w, int c);
NcBlob* nc_blob_make3d(int h, int w, int c);
NcBlob* nc_blob_make2d(int h, int w);
NcBlob* nc_blob_make_random(int n, int h, int w, int c, float s, float t);
NcBlob* nc_blob_make_same(int n, int h, int w, int c, float v);
NcBlob* nc_blob_make2d_empty(int h, int w);
void nc_blob_data_realloc(NcBlob* blob, int n, int h, int w, int c);
void nc_blob_data_realloc3d(NcBlob* blob, int h, int w, int c);

NcStride* nc_stride_make(int h, int w);


// ! infer related
NcConvolutionParam* nc_infer_make_convolution_param(int map_size, int in_channels, int out_channels, NcPaddingType pad_type);
NcPoolingParam* nc_infer_make_pooling_param(int map_size, int in_channels, NcPoolingType pool_type, NcPaddingType pad_type);
NcInnerproductParam* nc_infer_make_innerproduct_param(int in_num, int out_num);
NcLayerInput* nc_infer_make_layer_input(int n, const int* blob_ids, NcNet* net);
NcLayerOutput* nc_infer_make_layer_output(int n, const int* blob_ids, NcNet* net);
void nc_infer_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_infer_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_infer_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output);


// ! train related
NcConvolutionParam* nc_train_make_convolution_param(int in_height, int in_width, int map_size, int in_channels, int out_channels);
NcPoolingParam* nc_train_make_pooling_param(int in_height, int in_width, int map_size, int in_channels, int out_channels, int pool_type);
NcInnerproductParam* nc_train_make_innerproduct_param(int in_num, int out_num);
NcLayerInput* nc_train_make_layer_input(int n, const int* blob_ids, const NcNet* net);
NcLayerOutput* nc_train_make_layer_output(int n, const int* blob_ids, const NcNet* net, NcBlob** output_blobs);
void nc_train_forward_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_forward_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_forward_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_backward_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_backward_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_backward_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output);

void nc_free_layer(NcLayer* layer);

// very straight forward and naive implementation of 2D convolution
void nc_conv2d(const NcBlob2D* input, const NcBlob2D* kernel, NcBlob2D* output, NcStride* stride);

void nc_net_forward(NcNet* net, NcImage* image, int label);
void nc_net_backward(NcNet* net);
void nc_train_cls_net(NcNet* net, NcTrainConfig* train_cfg, NcClsDataConfig* data_cfg);
void nc_cls_data_loader(NcClsDataConfig* cfg);
void nc_read_mnist_image(const char* filename, NcImage*** _images, int* _image_num);
void nc_read_mnist_label(const char* filename, int** _labels, int* _label_num);


NcLayer* nc_make_layer();

void nc_relu_forward(NcBlob* bottom);

#ifdef __cplusplus
}
#endif
