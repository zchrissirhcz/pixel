#include "px_cnn.h"
#include <float.h> // FLT_MIN

px_matrix_dim_t px_get_pooling_output_matrix_dim(const px_matrix_dim_t input_dim, const px_pooling_param_t pooling_param)
{
    const int input_h = input_dim.height;
    const int input_w = input_dim.width;

    const int kernel_h = pooling_param.kernel_h;
    const int kernel_w = pooling_param.kernel_w;

    const int stride_h = pooling_param.stride_h;
    const int stride_w = pooling_param.stride_w;
    
    const int output_h = (input_h - kernel_h) / stride_h + 1;
    const int output_w = (input_w - kernel_w) / stride_w + 1;

    px_matrix_dim_t output_dim = {0};
    output_dim.height = output_h;
    output_dim.width = output_w;

    return output_dim;
}

px_matrix_t* px_forward_max_pooling_layer_for_matrix(const px_matrix_t* input, const px_pooling_param_t pooling_param)
{
    px_matrix_dim_t input_dim = px_get_matrix_dim(input);
    px_matrix_dim_t output_dim = px_get_pooling_output_matrix_dim(input_dim, pooling_param);
    
    const int kernel_h = pooling_param.kernel_h;
    const int kernel_w = pooling_param.kernel_w;

    const int input_h = input_dim.height;
    const int input_w = input_dim.width;

    const int stride_h = pooling_param.stride_h;
    const int stride_w = pooling_param.stride_w;

    px_matrix_t* output = px_make_matrix(output_dim);
    for (int i = 0; i+kernel_h-1 < input_h; i+=stride_h)
    {
        for (int j = 0; j+kernel_w-1 < input_w; j+=stride_w)
        {
            float max_value = FLT_MIN;
            for (int ki=0; ki<kernel_h; ki++)
            {
                for (int kj=0; kj<kernel_w; kj++)
                {
                    const int ri = i + ki;
                    const int rj = j + kj;
                    const int value = px_get_matrix_value(input, ri, rj);
                    if (max_value < value)
                    {
                        max_value = value;
                    }
                }
            }
            const int di = i / stride_h;
            const int dj = j / stride_w;
            px_set_matrix_value(output, di, dj, max_value);
        }
    }

    return output;
}

px_pooling_param_t px_make_pooling_param(const px_kernel_size_t kernel_size, const px_stride_t stride)
{
    px_pooling_param_t pooling_param;
    pooling_param.kernel_h = kernel_size.height;
    pooling_param.kernel_w = kernel_size.width;
    pooling_param.stride_h = stride.height;
    pooling_param.stride_w = stride.width;

    return pooling_param;
}