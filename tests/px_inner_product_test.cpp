#include "px_cnn.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(innerproduct, naive)
{
    const int len = 3;
    px_array_t* v1 = px_create_array(len);
    v1->data[0] = 1.0f;
    v1->data[1] = 2.0f;
    v1->data[2] = 3.0f;

    px_array_t* v2 = px_create_array(len);
    v2->data[0] = 0.5f;
    v2->data[1] = 0.1f;
    v2->data[2] = 1.0f;

    float expected = 3.7f;
    float actual = px_inner_product(v1, v2);
    EXPECT_EQ(expected, actual);

    px_destroy_array(v1);
    px_destroy_array(v2);
}
