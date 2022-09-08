#include "px_cnn.h"
#include "gtest/gtest.h"

TEST(max_pooling_layer, forward)
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

    px_kernel_size_t kernel_size = px_make_kernel_size(2, 2);
    px_stride_t stride = px_make_stride(1, 1);
    px_pooling_param_t pooling_param = px_make_pooling_param(kernel_size, stride);

    px_matrix_t* output = px_forward_max_pooling_layer_for_matrix(input, pooling_param);

    EXPECT_EQ(output->height, 4);
    EXPECT_EQ(output->width, 4);

    // PCNN_LOGE("input:\n");
    // dump_matrix(input);

    // PCNN_LOGE("output:\n");
    // dump_matrix(output);

    px_release_matrix(input);
    px_release_matrix(output);
}