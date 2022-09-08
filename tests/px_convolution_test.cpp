#include "px_cnn.h"
#include "gtest/gtest.h"
#include "px_log.h"
#include "px_compare.h"

TEST(convolution_forward_cube, check_output_dim)
{
    px_cube_dim_t input_dim = {0};
    input_dim.channel = 3;
    input_dim.height = 7;
    input_dim.width = 7;
    px_cube_t* input = px_make_cube(input_dim);

    px_cube_dim_t kernel_dim = {0};
    kernel_dim.channel = input_dim.channel;
    kernel_dim.height = 3;
    kernel_dim.width = 3;
    px_cube_t* kernel = px_make_cube(kernel_dim);

    px_stride_t stride = px_make_stride(2, 2);
    px_pad_t pad = px_make_pad(0, 0, 0, 0);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    float bias = 0;
    px_cube_t* output = px_forward_convolution_layer_for_cube(input, &kernel, 1, conv_param, &bias, 1);

    // check dim
    EXPECT_TRUE(output != NULL);
    EXPECT_EQ(output->channel, 1);
    EXPECT_EQ(output->height, 3);
    EXPECT_EQ(output->width, 3);

    px_release_cube(input);
    px_release_cube(kernel);
    px_release_cube(output);
}

TEST(convolution_forward_cube, check_output_value)
{
    px_cube_dim_t input_dim = {0};
    input_dim.channel = 3;
    input_dim.height = 7;
    input_dim.width = 7;
    px_cube_t* input = px_make_cube(input_dim);

    float input_channel0[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 2, 2, 0,
        0, 0, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 1, 0,
        0, 0, 2, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    float input_channel1[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 2, 0, 0,
        0, 0, 2, 1, 1, 2, 0,
        0, 1, 2, 0, 0, 2, 0,
        0, 0, 2, 1, 2, 1, 0,
        0, 2, 0, 1, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    float input_channel2[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 2, 0, 2, 0,
        0, 0, 0, 1, 2, 1, 0,
        0, 1, 0, 2, 2, 1, 0,
        0, 2, 0, 2, 0, 0, 0,
        0, 0, 0, 1, 1, 2, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    size_t buf_size = input_dim.height * input_dim.width * sizeof(float);
    memcpy(input->data, input_channel0, buf_size);
    memcpy(px_get_matrix_from_cube(input, 1).data, input_channel1, buf_size);
    memcpy(px_get_matrix_from_cube(input, 2).data, input_channel2, buf_size);

    px_cube_dim_t kernel_dim = {0};
    kernel_dim.channel = input_dim.channel;
    kernel_dim.height = 3;
    kernel_dim.width = 3;
    px_cube_t* kernel = px_make_cube(kernel_dim);

    // filling kernel values
    float kernel_channel0[] = {
        1, 1, -1,
        -1, 0, 1,
        -1, -1, 0
    };
    float kernel_channel1[] = {
        -1, 0, -1,
        0, 0, -1,
        1, -1, 0
    };
    float kernel_channel2[] = {
        0, 1, 0,
        1, 0, 1,
        0, -1, 1
    };

    buf_size = kernel->height * kernel->width * sizeof(float);
    memcpy(kernel->data, kernel_channel0, buf_size);
    memcpy(px_get_matrix_from_cube(kernel, 1).data, kernel_channel1, buf_size);
    memcpy(px_get_matrix_from_cube(kernel, 2).data, kernel_channel2, buf_size);

    px_stride_t stride = px_make_stride(2, 2);
    px_pad_t pad = px_make_pad(0, 0, 0, 0);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    float bias = 1;

    px_cube_t* output = px_forward_convolution_layer_for_cube(input, &kernel, 1, conv_param, &bias, 1);

    // check dim
    EXPECT_TRUE(output != NULL);
    EXPECT_EQ(output->channel, 1);
    EXPECT_EQ(output->height, 3);
    EXPECT_EQ(output->width, 3);

    float expected_output[] = {
        1, 0, -3,
        -6, 1, 1,
        4, -3, 1
    };

    for (int i=0; i<9; i++) {
        EXPECT_EQ(expected_output[i], output->data[i]);
    }

    px_release_cube(input);
    px_release_cube(kernel);
    px_release_cube(output);
}

TEST(convolution_forward_cube, check_output_value_multiple_kernels)
{
    px_cube_dim_t input_dim = {0};
    input_dim.channel = 3;
    input_dim.height = 7;
    input_dim.width = 7;
    px_cube_t* input = px_make_cube(input_dim);

    float input_channel0[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 2, 2, 0,
        0, 0, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 1, 0,
        0, 0, 2, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    float input_channel1[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 2, 0, 0,
        0, 0, 2, 1, 1, 2, 0,
        0, 1, 2, 0, 0, 2, 0,
        0, 0, 2, 1, 2, 1, 0,
        0, 2, 0, 1, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    float input_channel2[] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 2, 0, 2, 0,
        0, 0, 0, 1, 2, 1, 0,
        0, 1, 0, 2, 2, 1, 0,
        0, 2, 0, 2, 0, 0, 0,
        0, 0, 0, 1, 1, 2, 0,
        0, 0, 0, 0, 0, 0, 0,
    };

    size_t buf_size = input_dim.height * input_dim.width * sizeof(float);
    memcpy(input->data, input_channel0, buf_size);
    memcpy(px_get_matrix_from_cube(input, 1).data, input_channel1, buf_size);
    memcpy(px_get_matrix_from_cube(input, 2).data, input_channel2, buf_size);

    px_cube_dim_t kernel_dim = {0};
    kernel_dim.channel = input_dim.channel;
    kernel_dim.height = 3;
    kernel_dim.width = 3;

    // make kernel0
    px_cube_t* kernel0 = px_make_cube(kernel_dim);

    // filling kernel values
    float kernel0_channel0[] = {
        1, 1, -1,
        -1, 0, 1,
        -1, -1, 0
    };
    float kernel0_channel1[] = {
        -1, 0, -1,
        0, 0, -1,
        1, -1, 0
    };
    float kernel0_channel2[] = {
        0, 1, 0,
        1, 0, 1,
        0, -1, 1
    };

    buf_size = kernel0->height * kernel0->width * sizeof(float);
    memcpy(kernel0->data, kernel0_channel0, buf_size);
    memcpy(px_get_matrix_from_cube(kernel0, 1).data, kernel0_channel1, buf_size);
    memcpy(px_get_matrix_from_cube(kernel0, 2).data, kernel0_channel2, buf_size);

    // --- make kernel1
    px_cube_t* kernel1 = px_make_cube(kernel_dim);

    // filling kernel values
    float kernel1_channel0[] = {
        -1, -1, 0,
        -1, 1, 0,
        -1, 1, 0
    };
    float kernel1_channel1[] = {
        1, -1, 0,
        -1, 0, -1,
        -1, 0, 0
    };
    float kernel1_channel2[] = {
        -1, 0, 1,
        1, 0, 1,
        0, -1, 0
    };

    buf_size = kernel1->height * kernel1->width * sizeof(float);
    memcpy(kernel1->data, kernel1_channel0, buf_size);
    memcpy(px_get_matrix_from_cube(kernel1, 1).data, kernel1_channel1, buf_size);
    memcpy(px_get_matrix_from_cube(kernel1, 2).data, kernel1_channel2, buf_size);


    px_stride_t stride = px_make_stride(2, 2);
    px_pad_t pad = px_make_pad(0, 0, 0, 0);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    constexpr int kernels_num = 2;

    float bias[kernels_num] = {1, 0};
    px_cube_t* kernels[2] = {kernel0, kernel1};
    px_cube_t* output = px_forward_convolution_layer_for_cube(input, kernels, kernels_num, conv_param, bias, kernels_num);

    // check dim
    EXPECT_TRUE(output != NULL);
    EXPECT_EQ(output->channel, kernels_num);
    EXPECT_EQ(output->height, 3);
    EXPECT_EQ(output->width, 3);

    float expected_output[] = {
        1, 0, -3,
        -6, 1, 1,
        4, -3, 1,

        -1, -6, -4,
        -2, -3, -4,
        -1, -3, -3
    };

    for (int i = 0; i < 18; i++)
    {
        EXPECT_EQ(expected_output[i], output->data[i]);
    }

    px_release_cube(input);
    px_release_cube(kernel0);
    px_release_cube(kernel1);
    px_release_cube(output);
}

TEST(convolution_forward, kernel1x1_stride1x1)
{
    px_matrix_dim_t input_dim = {3, 3};
    px_matrix_t* input = px_make_matrix(input_dim);
    for (int i = 0; i < input->height; i++)
    {
        for (int j = 0; j < input->width; j++)
        {
            px_set_matrix_value(input, i, j, i*10 + j);
        }
    }

    px_matrix_dim_t kernel_dim = {1, 1};
    px_matrix_t* kernel = px_make_matrix(kernel_dim);
    px_set_matrix_value(kernel, 0, 0, 1);

    px_stride_t stride = px_make_stride(1, 1);
    px_pad_t pad = px_make_pad(0, 0, 0, 0);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    float bias = 0;
    px_matrix_t* output = px_forward_convolution_layer_for_matrix(input, kernel, conv_param, bias);

    PX_LOGE("input:\n");
    px_dump_matrix(input);

    PX_LOGE("kernel:\n");
    px_dump_matrix(kernel);

    PX_LOGE("output:\n");
    px_dump_matrix(output);

    px_release_matrix(input);
    px_release_matrix(output);
    px_release_matrix(kernel);
}

TEST(convolution_forward, kernel2x2_stride1x1)
{
    px_matrix_dim_t dim = {3, 3};
    px_matrix_t* input = px_make_matrix(dim);
    for (int i = 0; i < input->height; i++)
    {
        for (int j = 0; j < input->width; j++)
        {
            px_set_matrix_value(input, i, j, i*10 + j);
        }
    }

    px_matrix_dim_t kernel_dim = {2, 2};
    px_matrix_t* kernel = px_make_matrix(kernel_dim);
    px_set_matrix_value(kernel, 0, 0, 1);
    px_set_matrix_value(kernel, 0, 1, 1);
    px_set_matrix_value(kernel, 1, 0, 1);
    px_set_matrix_value(kernel, 1, 1, 1);


    px_stride_t stride = px_make_stride(1, 1);
    px_pad_t pad = px_make_pad(0, 0, 0, 0);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    float bias = 0;
    px_matrix_t* output = px_forward_convolution_layer_for_matrix(input, kernel, conv_param, bias);

    PX_LOGE("input:\n");
    px_dump_matrix(input);

    PX_LOGE("kernel:\n");
    px_dump_matrix(kernel);

    PX_LOGE("output:\n");
    px_dump_matrix(output);

    px_release_matrix(input);
    px_release_matrix(output);
    px_release_matrix(kernel);
}

TEST(convolution_forward, kernel2x2_stride2x2)
{
    px_matrix_dim_t input_dim = {5, 5};
    px_matrix_t* input = px_make_matrix(input_dim);
    for (int i = 0; i < input->height; i++)
    {
        for (int j = 0; j < input->width; j++)
        {
            px_set_matrix_value(input, i, j, i*10 + j);
        }
    }

    px_matrix_dim_t kernel_dim = {2, 2};
    px_matrix_t* kernel = px_make_matrix(kernel_dim);
    px_set_matrix_value(kernel, 0, 0, 1);
    px_set_matrix_value(kernel, 0, 1, 1);
    px_set_matrix_value(kernel, 1, 0, 1);
    px_set_matrix_value(kernel, 1, 1, 1);


    px_stride_t stride = px_make_stride(2, 2);
    px_pad_t pad = px_make_pad(0, 0, 0, 0);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    float bias = 0;
    px_matrix_t* output = px_forward_convolution_layer_for_matrix(input, kernel, conv_param, bias);

    PX_LOGE("input:\n");
    px_dump_matrix(input);

    PX_LOGE("kernel:\n");
    px_dump_matrix(kernel);

    PX_LOGE("output:\n");
    px_dump_matrix(output);

    px_release_matrix(input);
    px_release_matrix(output);
    px_release_matrix(kernel);
}

TEST(convolution_forward, kernel2x2_stride2x2_pad0101)
{
    px_matrix_dim_t input_dim = {5, 5};
    px_matrix_t* input = px_make_matrix(input_dim);
    for (int i = 0; i < input->height; i++)
    {
        for (int j=0; j < input->width; j++)
        {
            px_set_matrix_value(input, i, j, i*10 + j);
        }
    }

    px_matrix_dim_t kernel_dim = {2, 2};
    px_matrix_t* kernel = px_make_matrix(kernel_dim);
    px_set_matrix_value(kernel, 0, 0, 1);
    px_set_matrix_value(kernel, 0, 1, 1);
    px_set_matrix_value(kernel, 1, 0, 1);
    px_set_matrix_value(kernel, 1, 1, 1);


    px_stride_t stride = px_make_stride(2, 2);
    px_pad_t pad = px_make_pad(0, 1, 0, 1);
    px_conv_param_t conv_param = px_make_conv_param(stride, pad);

    float bias = 0;
    px_matrix_t* output = px_forward_convolution_layer_for_matrix(input, kernel, conv_param, bias);

    EXPECT_EQ(output->height, 3);
    EXPECT_EQ(output->width, 3);

    PX_LOGE("input:\n");
    px_dump_matrix(input);

    PX_LOGE("kernel:\n");
    px_dump_matrix(kernel);

    PX_LOGE("output:\n");
    px_dump_matrix(output);

    px_release_matrix(input);
    px_release_matrix(output);
    px_release_matrix(kernel);
}