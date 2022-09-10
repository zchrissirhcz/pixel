#include "px_cnn.h"
#include "px_assert.h"
#include <string.h>

px_matrix_dim_t px_get_conv_output_matrix_dim(const px_matrix_dim_t input_dim, const px_matrix_dim_t kernel_dim, const px_conv_param_t conv_param)
{
    const int input_h = input_dim.height;
    const int input_w = input_dim.width;

    const int kernel_h = kernel_dim.height;
    const int kernel_w = kernel_dim.width;

    const int stride_h = conv_param.stride_h;
    const int stride_w = conv_param.stride_w;
    
    const int output_h = (input_h - kernel_h) / stride_h + 1;
    const int output_w = (input_w - kernel_w) / stride_w + 1;

    px_matrix_dim_t output_dim = {0};
    output_dim.height = output_h;
    output_dim.width = output_w;

    return output_dim;
}

px_conv_param_t px_create_conv_param(const px_stride_t stride, const px_pad_t pad)
{
    px_conv_param_t conv_param;

    conv_param.stride_h = stride.height;
    conv_param.stride_w = stride.width;

    conv_param.pad_top = pad.top;
    conv_param.pad_bottom = pad.bottom;
    conv_param.pad_left = pad.left;
    conv_param.pad_right = pad.right;

    return conv_param;
}

px_cube_t* px_forward_convolution_layer_for_cube(const px_cube_t* input, px_cube_t** kernels, const int kernels_num, const px_conv_param_t conv_param, float* bias, const int bias_num)
{
    PX_ASSERT(kernels_num == bias_num);
    px_matrix_dim_t input_channel_dim = px_get_channel_wise_matrix_dim(input);
    px_matrix_dim_t kernel_channel_dim = px_get_channel_wise_matrix_dim(kernels[0]);
    px_matrix_dim_t output_channel_dim = px_get_conv_output_matrix_dim(input_channel_dim, kernel_channel_dim, conv_param);

    px_cube_dim_t output_dim = {0};
    output_dim.height = output_channel_dim.height;
    output_dim.width = output_channel_dim.width;
    output_dim.channel = kernels_num;
    px_cube_t* output_cube = px_create_cube(output_dim);

    const int output_h = output_dim.height;
    const int output_w = output_dim.width;

    for (int i = 0; i < kernels_num; i++)
    {
        px_matrix_t* output_matrix = px_forward_convolution_layer_for_cube_with_one_kernel(input, kernels[i], conv_param, bias[i]);
        void* dst = px_get_matrix_data_from_cube(output_cube, i);
        void* src = output_matrix->data;
        const int buf_size = output_h * output_w * sizeof(float);
        memcpy(dst, src, buf_size);
    }
    return output_cube;
}

px_matrix_t* px_forward_convolution_layer_for_cube_with_one_kernel(const px_cube_t* input, px_cube_t* kernel, const px_conv_param_t conv_param, const float bias)
{
    /// TODO: https://blog.csdn.net/oMoDao1/article/details/83545144
    px_matrix_dim_t input_channel_dim = px_get_channel_wise_matrix_dim(input);
    px_matrix_dim_t kernel_channel_dim = px_get_channel_wise_matrix_dim(kernel);
    px_matrix_dim_t output_channel_dim = px_get_conv_output_matrix_dim(input_channel_dim, kernel_channel_dim, conv_param);

    const int output_h = output_channel_dim.height;
    const int output_w = output_channel_dim.width;

    const int kernel_h = kernel->height;
    const int kernel_w = kernel->width;

    const int input_h = input->height;
    const int input_w = input->width;

    px_matrix_t* output = px_create_matrix(output_channel_dim);
    memset(output->data, 0, output_h*output_w*sizeof(float));

    const int input_c = input->channel;
    for (int k = 0; k < input_c; k++)
    //for (int k = 0; k < 1; k++)
    {
        size_t buf_size = 0;

        // prepare input matrix
        px_matrix_t* input_matrix = px_create_matrix(input_channel_dim);
        buf_size = input_h * input_w * sizeof(float);
        memcpy(input_matrix->data, px_get_matrix_from_cube(input, k).data, buf_size);

        // prepare kernel matrix
        px_matrix_t* kernel_matrix = px_create_matrix(kernel_channel_dim);
        buf_size = kernel_h * kernel_w * sizeof(float);
        memcpy(kernel_matrix->data, px_get_matrix_from_cube(kernel, k).data, buf_size);

        // calculate output matrix
        const int current_bias = 0;
        px_matrix_t* output_matrix = px_forward_convolution_layer_for_matrix(input_matrix, kernel_matrix, conv_param, current_bias);

        // PCNN_LOGE("*** output_matrix:\n");
        // dump_matrix(output_matrix);

        px_matrix_add_matrix(output, output_matrix);
        // PCNN_LOGE(">>> after adding, output is:\n");
        // dump_matrix(output);
    }
    px_matrix_add_scalar(output, bias);

    return output;
}

px_matrix_t* px_forward_convolution_layer_for_matrix(const px_matrix_t* input, const px_matrix_t* kernel, const px_conv_param_t conv_param, const float bias)
{
    px_pad_t pad = px_get_pad_from_conv_param(conv_param);
    // PCNN_LOGE(">>> pad: top=%d, bottom=%d, left=%d, right=%d\n",
    //         pad.top, pad.bottom, pad.left, pad.right);
    px_matrix_t* padded_input = px_copy_make_border_for_matrix(input, pad);

    int input_h = padded_input->height;
    int input_w = padded_input->width;
    int kernel_h = kernel->height;
    int kernel_w = kernel->width;
    int stride_h = conv_param.stride_h;
    int stride_w = conv_param.stride_w;

    int output_h = (input_h - kernel_h) / stride_h + 1;
    int output_w = (input_w - kernel_w) / stride_w + 1;
    px_matrix_dim_t output_dim = {0};
    output_dim.height = output_h;
    output_dim.width = output_w;

    // PCNN_LOGE(">>> input_h=%d, kernel_h=%d, stride_h=%d, output_h=%d\n",
    //         input_h, kernel_h, stride_h, output_h);
    px_matrix_t* output = px_create_matrix(output_dim);

    for (int i=0; i+kernel_h-1<input_h; i+=stride_h) {
        for (int j=0; j+kernel_w-1<input_w; j+=stride_w) {
            float sum = bias;
            for (int ki=0; ki<kernel_h; ki++) {
                for (int kj=0; kj<kernel_w; kj++) {
                    const int ri = i + ki;
                    const int rj = j + kj;
                    float input_value = px_get_matrix_value(padded_input, ri, rj);
                    float kernel_value = px_get_matrix_value(kernel, ki, kj);
                    sum += input_value * kernel_value;
                }
            }
            const int out_i = i / stride_h;
            const int out_j = j / stride_w;
            px_set_matrix_value(output, out_i, out_j, sum);
        }
    }

    px_destroy_matrix(padded_input);

    // PCNN_LOGE(" output:\n");
    // dump_matrix(output);

    return output;
}

px_pad_t px_get_pad_from_conv_param(const px_conv_param_t conv_param)
{
    px_pad_t pad;
    pad.top = conv_param.pad_top;
    pad.bottom = conv_param.pad_bottom;
    pad.left = conv_param.pad_left;
    pad.right = conv_param.pad_right;

    return pad;
}