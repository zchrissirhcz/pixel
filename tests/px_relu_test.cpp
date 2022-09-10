#include "px_cnn.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(relu, naive)
{
    px_array_t* input = px_create_array(5);

    input->data[0] = -1;
    input->data[1] = 2;
    input->data[2] = -3;
    input->data[3] = 4;
    input->data[4] = -5;

    px_array_t* expected = px_create_array(5);
    expected->data[0] = 0;
    expected->data[1] = 2;
    expected->data[2] = 0;
    expected->data[3] = 4;
    expected->data[4] = 0;

    px_array_t* actual = px_create_array(5);
    px_relu_forward(input, actual);

    EXPECT_TRUE(px_array_almost_equal(expected, actual, 0));

    px_destroy_array(input);
    px_destroy_array(actual);
    px_destroy_array(expected);
}

TEST(relu, matrix)
{
    px_matrix_dim_t input_dim = {2, 2};
    px_matrix_t* input = px_create_matrix(input_dim);
    px_set_matrix_value(input, 0, 0, 1);
    px_set_matrix_value(input, 0, 1, -1);
    px_set_matrix_value(input, 1, 0, 233);
    px_set_matrix_value(input, 1, 1, -42);

    px_matrix_t* output = px_create_matrix(input_dim);
    px_forward_relu_layer_for_matrix(input, output);

    px_matrix_t* expected_output = px_create_matrix(input_dim);
    px_set_matrix_value(expected_output, 0, 0, 1);
    px_set_matrix_value(expected_output, 0, 1, 0);
    px_set_matrix_value(expected_output, 1, 0, 233);
    px_set_matrix_value(expected_output, 1, 1, 0);

    EXPECT_TRUE(px_matrix_almost_equal(expected_output, output, 0));

    px_destroy_matrix(input);
    px_destroy_matrix(expected_output);
    px_destroy_matrix(output);
}