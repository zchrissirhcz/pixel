#include "gtest/gtest.h"
#include "px_compare.h"
#include "px_image.h"

TEST(compare, image_almost_equal)
{
    const int height = 2;
    const int width = 2;
    const int channel = 1;
    px_image_t* src1 = px_create_image(height, width, channel);
    px_image_t* src2 = px_create_image(height, width, channel);
    for (int i = 0; i < height * width * channel; i++)
    {
        int value = (rand() / RAND_MAX) * 255;
        src1->data[i] = value;
        src2->data[i] = value;
    }

    EXPECT_TRUE(px_image_almost_equal(src1, src2, 0));

    src1->data[0] = 233;
    src2->data[0] = 234;
    EXPECT_FALSE(px_image_almost_equal(src1, src2, 0));
    EXPECT_TRUE(px_image_almost_equal(src1, src2, 1));
    EXPECT_TRUE(px_image_almost_equal(src1, src2, 2));

    px_destroy_image(src1);
    px_destroy_image(src2);
}

TEST(compare, size_equal)
{
    px_size_t sz1{10, 20};
    px_size_t sz2{10, 20};
    EXPECT_TRUE(px_size_equal(sz1, sz2));

    px_size_t sz3{20, 10};
    EXPECT_FALSE(px_size_equal(sz1, sz3));
}

TEST(compare, image_shape_equal)
{
    px_image_t* im0 = px_create_image(10, 20, 3);
    px_image_t* im1 = px_create_image(10, 20, 3);
    EXPECT_TRUE(px_image_shape_equal(im0, im1, true));
    px_image_t* im2 = px_create_image_header(10, 20, 3);
    im2->stride = im2->stride + 1;
    EXPECT_TRUE(px_image_shape_equal(im0, im2, false));
    EXPECT_FALSE(px_image_shape_equal(im0, im2, true));

    px_destroy_image(im0);
    px_destroy_image(im1);
    px_destroy_image_header(im2);
}

TEST(compare, get_image_size)
{
    px_image_t image;
    image.height = 10;
    image.width = 20;
    px_size_t actual = px_get_image_size(&image);
    px_size_t expected;
    expected.width = 20;
    expected.height = 10;
    EXPECT_TRUE(px_size_equal(expected, actual));
}