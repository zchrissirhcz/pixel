#include "px_cnn.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(naive, relu)
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
