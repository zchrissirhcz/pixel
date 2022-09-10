#include "px_cnn.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(sigmoid, naive)
{
    px_matrix_dim_t input_dim = {2, 2};
    px_matrix_t* input = px_create_matrix(input_dim);
    px_set_matrix_value(input, 0, 0, 0.5f);
    px_set_matrix_value(input, 0, 1, 0.2f);
    px_set_matrix_value(input, 1, 0, 0.3f);
    px_set_matrix_value(input, 1, 1, 0.1f);

    px_matrix_t* output = px_create_matrix(input_dim);
    px_forward_sigmoid_layer_for_matrix(input, output);

    px_matrix_t* expected_output = px_create_matrix(input_dim);
    px_set_matrix_value(expected_output, 0, 0, 0.622459f);
    px_set_matrix_value(expected_output, 0, 1, 0.549834f);
    px_set_matrix_value(expected_output, 1, 0, 0.574443f);
    px_set_matrix_value(expected_output, 1, 1, 0.524979f);

    EXPECT_TRUE(px_matrix_almost_equal(expected_output, output, 1e-5));

    px_destroy_matrix(input);
    px_destroy_matrix(expected_output);
    px_destroy_matrix(output);
}
