#include "px_cnn.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(matrix, create)
{
    int h = 3;
    int w = 5;
    px_matrix_dim_t dim = {0};
    dim.height = h;
    dim.width = w;
    px_matrix_t* matrix = px_make_matrix(dim);
    EXPECT_EQ(matrix->height, h);
    EXPECT_EQ(matrix->width, w);
    EXPECT_TRUE(matrix->data != NULL);

    px_release_matrix(matrix);
}

TEST(matrix, get_value)
{
    px_matrix_dim_t dim {2, 3};
    px_matrix_t* matrix = px_make_matrix(dim);
    float expected = 233.f;
    px_set_matrix_value(matrix, 1, 2, expected);
    float actual = px_get_matrix_value(matrix, 1, 2);
    EXPECT_EQ(expected, actual);

    px_release_matrix(matrix);
}

TEST(matrix, get_value_invalid_idx)
{
    px_matrix_dim_t dim {2, 3};
    px_matrix_t* matrix = px_make_matrix(dim);
    EXPECT_DEATH(px_get_matrix_value(matrix, dim.height, dim.width), "");

    px_release_matrix(matrix);
}

TEST(matrix, get_value_invalid_null)
{
    px_matrix_dim_t dim {2, 3};
    px_matrix_t* matrix = NULL;
    EXPECT_DEATH(px_get_matrix_value(matrix, dim.height, dim.width), "");

    px_release_matrix(matrix);
}

TEST(matrix, set_value_invalid_idx)
{
    px_matrix_dim_t dim {2, 3};
    px_matrix_t* matrix = px_make_matrix(dim);
    EXPECT_DEATH(px_set_matrix_value(matrix, dim.height, dim.width, 233.0f), "");

    px_release_matrix(matrix);
}

TEST(matrix, set_value_null)
{
    px_matrix_dim_t dim {2, 3};
    px_matrix_t* matrix = NULL;
    EXPECT_DEATH(px_set_matrix_value(matrix, dim.height, dim.width, 233.0f), "");

    px_release_matrix(matrix);
}