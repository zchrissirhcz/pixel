#include "px_image.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(rgb_to_bgr, naive)
{
    px_image_t* src = px_create_image(1, 2, 3);

    src->data[0] = 0;
    src->data[1] = 1;
    src->data[2] = 2;
    src->data[3] = 3;
    src->data[4] = 4;
    src->data[5] = 5;

    px_image_t* expected = px_create_image(1, 2, 3);
    expected->data[0] = 2;
    expected->data[1] = 1;
    expected->data[2] = 0;
    expected->data[3] = 5;
    expected->data[4] = 4;
    expected->data[5] = 3;

    px_image_t* actual = px_create_image(1, 2, 3);
    px_rgb_to_bgr(src, actual);

    EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));

    px_destroy_image(src);
    px_destroy_image(actual);
    px_destroy_image(expected);
}

TEST(rgb_to_bgr, inplace)
{
    px_image_t* src = px_create_image(1, 2, 3);

    src->data[0] = 0;
    src->data[1] = 1;
    src->data[2] = 2;
    src->data[3] = 3;
    src->data[4] = 4;
    src->data[5] = 5;

    px_image_t* expected = px_create_image(1, 2, 3);
    expected->data[0] = 2;
    expected->data[1] = 1;
    expected->data[2] = 0;
    expected->data[3] = 5;
    expected->data[4] = 4;
    expected->data[5] = 3;

    px_rgb_to_bgr_inplace(src);
    EXPECT_TRUE(px_image_almost_equal(expected, src, 0));

    px_destroy_image(src);
    px_destroy_image(expected);
}