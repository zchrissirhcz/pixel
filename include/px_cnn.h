#pragma once

#include "px_typebase.h"

typedef struct px_array_t
{
    float* data;
    int len;
} px_array_t;

typedef struct px_size_t px_matrix_dim_t;

typedef struct px_matrix_t
{
    float* data;
    int height;
    int width;
} px_matrix_t;

typedef struct px_size_t px_kernel_size_t;

typedef struct px_pooling_param_t
{
    int kernel_h;
    int kernel_w;

    int stride_h;
    int stride_w;

    // TODO: pad
} px_pooling_param_t;

typedef struct px_size_t px_stride_t;

typedef struct px_cube_t
{
    float* data;
    int channel;
    int height;
    int width;
} px_cube_t;

typedef struct px_cube_dim_t
{
    int channel;
    int height;
    int width;
} px_cube_dim_t;

typedef struct px_tensor_dim_t
{
    int batch;
    int channel;
    int height;
    int width;
} px_tensor_dim_t;

typedef struct px_conv_param_t
{
    int stride_h;
    int stride_w;

    int pad_top;
    int pad_bottom;
    int pad_left;
    int pad_right;
} px_conv_param_t;

#ifdef __cplusplus
extern "C" {
#endif

void px_relu_forward(px_array_t* input, px_array_t* output);

px_array_t* px_create_array(int len);
void px_destroy_array(px_array_t* array);

px_matrix_t* px_create_matrix(px_matrix_dim_t dim);
void px_destroy_matrix(px_matrix_t* matrix);

int px_set_matrix_value(px_matrix_t* matrix, int i, int j, float value);
float px_get_matrix_value(const px_matrix_t* matrix, int i, int j);

int px_matrix_add_matrix(px_matrix_t* input, const px_matrix_t* plus);
int px_matrix_add_scalar(px_matrix_t* input, const float bias);

px_matrix_dim_t px_get_matrix_dim(const px_matrix_t* matrix);
int px_get_matrix_area(const px_matrix_t* matrix);

typedef float (*PxEltwiseFunction)(const float);
void px_forward_eltwise_layer_for_array(const px_array_t* input, px_array_t* output, PxEltwiseFunction eltwise_func);
void px_forward_eltwise_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output, PxEltwiseFunction eltwise_func);
void px_forward_eltwise_layer_for_cube(const px_cube_t* input, px_cube_t* output, PxEltwiseFunction eltwise_func);

void px_forward_sigmoid_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output);
void px_forward_tanh_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output);
void px_forward_relu_layer_for_matrix(const px_matrix_t* input, px_matrix_t* output);

void px_forward_relu_layer_for_cube(const px_cube_t* input, px_cube_t* output);
void px_forward_sigmoid_layer_for_cube(const px_cube_t* input, px_cube_t* output);
void px_forward_tanh_layer_for_cube(const px_cube_t* input, px_cube_t* output);

void px_forward_relu6_layer_for_cube(const px_cube_t* input, px_cube_t* output);

px_matrix_t* px_forward_max_pooling_layer_for_matrix(const px_matrix_t* input, const px_pooling_param_t pooling_param);
px_matrix_dim_t px_get_pooling_output_matrix_dim(const px_matrix_dim_t input_dim, const px_pooling_param_t pooling_param);

px_kernel_size_t px_create_kernel_size(const int height, const int width);
px_stride_t px_create_stride(const int height, const int width);
px_pooling_param_t px_create_pooling_param(const px_kernel_size_t kernel_size, const px_stride_t stride);

float px_inner_product(px_array_t* v1, px_array_t* v2);

px_cube_t* px_create_cube(const px_cube_dim_t dim);
void px_destroy_cube(px_cube_t* cube);
px_cube_dim_t px_get_cube_dim(const px_cube_t* cube);
int px_get_cube_volume(const px_cube_t* cube);

px_matrix_t* px_copy_make_border_for_matrix(const px_matrix_t* matrix, const px_pad_t pad);

px_matrix_dim_t px_get_conv_output_matrix_dim(const px_matrix_dim_t input_dim, const px_matrix_dim_t kernel_dim, const px_conv_param_t conv_param);

px_pad_t px_create_pad(const int top, const int bottom, const int left, const int right);
px_conv_param_t px_create_conv_param(const px_stride_t stride, const px_pad_t pad);

px_matrix_dim_t px_get_channel_wise_matrix_dim(const px_cube_t* cube);
px_matrix_t* px_forward_convolution_layer_for_cube_with_one_kernel(const px_cube_t* input, px_cube_t* kernel, const px_conv_param_t conv_param, const float bias);
float* px_get_matrix_data_from_cube(const px_cube_t* cube, const int channel_idx);
px_matrix_t px_get_matrix_from_cube(const px_cube_t* cube, const int channel_idx);

px_matrix_t* px_forward_convolution_layer_for_matrix(const px_matrix_t* input, const px_matrix_t* kernel, const px_conv_param_t conv_param, const float bias);
px_pad_t px_get_pad_from_conv_param(const px_conv_param_t conv_param);
px_cube_t* px_forward_convolution_layer_for_cube(const px_cube_t* input, px_cube_t** kernels, const int kernels_num, const px_conv_param_t conv_param, float* bias, const int bias_num);

px_matrix_dim_t px_create_matrix_dim(const int height, const int width);

px_cube_dim_t px_create_cube_dim(int channel, int height, int width);
px_tensor_dim_t px_create_tensor_dim(int batch, int channel, int height, int width);

#ifdef __cplusplus
}
#endif