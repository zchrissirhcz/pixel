#pragma once

#include <stdbool.h>
#include "px_cnn.h"
#include "px_log.h"
#include "px_image.h"

#define NC_MAX_PATH           256
#define NC_IMAGE_ALIGN        1 // for simplicity, we don't align now
#define NC_CHANNEL_ALIGN      1 // for simplicity, we don't align now
#define NC_MAX_BLOB_PER_LAYER 100

typedef enum NcPaddingType
{
    NC_PADDING_CONV_CAFFE = 0,
    NC_PADDING_CONV_TF_SAME = 1,
    NC_PADDING_CONV_TF_VALID = 2,
    NC_PADDING_POOL_CAFFE = 3,
    NC_PADDING_POOL_TF_SAME = 4,
    NC_PADDING_POOL_TF_VALID = 5
} NcPaddingType;

typedef enum NcPoolingType
{
    NC_POOLING_AVERAGE = 0,
    NC_POOLING_MAX = 1
} NcPoolingType;

typedef enum NcLayerType
{
    NC_LAYER_DATA = 0,
    NC_LAYER_CONVOLUTION = 1,
    NC_LAYER_POOLING = 2,
    NC_LAYER_INNERPRODUCT = 3,
    NC_LAYER_LOSS = 4,
    NC_LAYER_UNKNOWN = 404
} NcLayerType;

typedef enum NcBlobDimOrder
{
    NCHW = 0,
    NHWC = 1
} NcBlobDimOrder;

typedef struct NcBlob
{
    int batch;
    int height;
    int width;
    int channel;
    int nstep;          //h*w*c
    int mass;           //n*nstep=n*h*w*c
    int rely_cnt_total; //how many blobs rely on this blob, before the inference run
    int rely_cnt;       //for the current inference layer, how many blobs still rely on this blob
    NcBlobDimOrder order;
    float* data;
} NcBlob;

typedef px_matrix_t NcBlob2D;

typedef struct NcConvolutionParam
{
    int in_height;
    int in_width;
    int in_channels;
    int out_channels;
    int out_height;
    int out_width;

    NcPaddingType padding_type;

    int map_size;
    px_stride_t stride;

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

typedef struct NcPoolingParam
{
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

    px_stride_t stride;

    NcBlob* d;
    NcBlob* y;
} NcPoolingParam;

typedef struct NcInnerproductParam
{
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

typedef enum NC_RELU_TYPE
{
    RELU = 0,
    RELU6 = 1
} NC_RELU_TYPE;

typedef struct NcReluParam
{
    NC_RELU_TYPE type;
} NcReluParam;

typedef struct NcLayerBottomTop
{
    int blob_num;
    NcBlob** blobs;
    int* blob_ids;
} NcLayerInput, NcLayerOutput;

//forward and backward: for training
typedef void (*NcLayerForward)(void* param, NcLayerInput* input, NcLayerOutput* output);
typedef void (*NcLayerBackward)(void* param, NcLayerInput* input, NcLayerOutput* output);
// infer: for inference
typedef void (*NcLayerInfer)(void* param, NcLayerInput* input, NcLayerOutput* output);

typedef struct NcLayer
{
    NcLayerType type;
    void* param;

    NcLayerForward forward;
    NcLayerBackward backward;
    NcLayerInfer infer;

    NcLayerInput* input;
    NcLayerOutput* output;
} NcLayer;

typedef struct NcNet
{
    int layers_num;
    NcLayer** layers;

    int blobs_num;
    NcBlob** blobs;

    // the following members are for training only:
    float* e; // 训练误差
    float* L; // 瞬时误差能量
} NcNet;

typedef px_size_t px_size_t;

typedef struct NcTrainConfig
{
    int num_epoch;
    float alpha;
    int train_num;
    char log_pth[NC_MAX_PATH];
} NcTrainConfig;

typedef struct LenetDataConfig
{
    int num; // how many (x,y) pairs do we have?
    px_image_t** images;
    int* labels;
    char dataset[NC_MAX_PATH];
    char splitset[NC_MAX_PATH];
} LenetDataConfig;

#ifdef __cplusplus
extern "C" {
#endif

float nc_get_random_float(float s, float t);
NcBlob* nc_create_empty_blob(int n, int h, int w, int c);
NcBlob* nc_create_blob(int n, int h, int w, int c);
NcBlob* nc_create_blob3d(int h, int w, int c);
NcBlob* nc_create_blob2d(int h, int w);
NcBlob* nc_create_blob_random(int n, int h, int w, int c, float s, float t);
NcBlob* nc_create_blob_same(int n, int h, int w, int c, float v);
void nc_blob_data_realloc(NcBlob* blob, int n, int h, int w, int c);
void nc_blob_data_realloc3d(NcBlob* blob, int h, int w, int c);

// ! infer related
NcConvolutionParam* nc_infer_create_convolution_param(int map_size, int in_channels, int out_channels, NcPaddingType pad_type);
NcPoolingParam* nc_infer_create_pooling_param(int map_size, int in_channels, NcPoolingType pool_type, NcPaddingType pad_type);
NcInnerproductParam* nc_infer_create_innerproduct_param(int in_num, int out_num);
NcLayerInput* nc_infer_create_layer_input(int n, const int* blob_ids, NcNet* net);
NcLayerOutput* nc_infer_create_layer_output(int n, const int* blob_ids, NcNet* net);
void nc_infer_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_infer_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_infer_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output);

// ! train related
NcConvolutionParam* nc_train_create_convolution_param(int in_height, int in_width, int map_size, int in_channels, int out_channels);
NcPoolingParam* nc_train_create_pooling_param(int in_height, int in_width, int map_size, int in_channels, int out_channels, int pool_type);
NcInnerproductParam* nc_train_create_innerproduct_param(int in_num, int out_num);
NcLayerInput* nc_train_create_layer_input(int n, const int* blob_ids, const NcNet* net);
NcLayerOutput* nc_train_create_layer_output(int n, const int* blob_ids, const NcNet* net, NcBlob** output_blobs);
void nc_train_forward_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_forward_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_forward_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_backward_convolution(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_backward_pooling(void* param_, NcLayerInput* input, NcLayerOutput* output);
void nc_train_backward_innerproduct(void* param_, NcLayerInput* input, NcLayerOutput* output);

void nc_destroy_layer(NcLayer* layer);

void nc_net_forward(NcNet* net, px_image_t* image, int label);
void nc_net_backward(NcNet* net);
void nc_train_cls_net(NcNet* net, NcTrainConfig* train_cfg, LenetDataConfig* data_cfg);
void nc_cls_data_loader(LenetDataConfig* cfg);

NcLayer* nc_create_layer();

void nc_relu_forward(NcBlob* bottom);

void nc_pooling_forward_nhwc(NcPoolingParam* param, NcBlob* bottom, NcBlob* top);
void nc_pooling_forward_nchw(NcPoolingParam* param, NcBlob* bottom, NcBlob* top);
void nc_pooling_test_nchw();
void nc_pooling_test_nhwc();

void nc_convolution_forward_nhwc(NcConvolutionParam* param, NcBlob* bottom, NcBlob* top);
void nc_convolution_forward_nchw(NcConvolutionParam* param, NcBlob* bottom, NcBlob* top);
// unit test functions
void nc_convolution_test_nchw();
void nc_convolution_test_nhwc();

px_size_t nc_get_conv_output_size(px_size_t input_size, px_size_t kernel_size, px_size_t stride);

void nc_swap_rgb_and_bgr_inplace(px_image_t* im);

const char* layer_type_to_str(NcLayerType type);

#ifdef __cplusplus
}
#endif
