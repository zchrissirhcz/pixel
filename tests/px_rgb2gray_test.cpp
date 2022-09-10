#include "px_image.h"
#include "gtest/gtest.h"
#include "px_compare.h"

TEST(rgb2gray, naive)
{
    px_image_t* src = px_create_image(1, 2, 3);

    src->data[0] = 1;
    src->data[1] = 2;
    src->data[2] = 3;
    src->data[3] = 6;
    src->data[4] = 6;
    src->data[5] = 6;

    px_image_t* expected = px_create_image(1, 2, 1);
    expected->data[0] = 2;
    expected->data[1] = 6;

    px_image_t* actual = px_create_image(1, 2, 1);
    px_rgb2gray(src, actual);

    EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));

    px_destroy_image(src);
    px_destroy_image(actual);
    px_destroy_image(expected);
}
