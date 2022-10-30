#include "px_image.h"
#include "gtest/gtest.h"

TEST(px_resize_linear, out_of_border)
{
    px_image_t* im0 = px_create_image(512, 512, 3);
    const int height = 640;
    const int width = 320;
    px_image_t* im1 = px_create_image(height, width, im0->channel);
    px_size_t dsize;
    dsize.width = width;
    dsize.height = height;
    px_resize_linear(im0, im1, dsize);

    px_destroy_image(im0);
    px_destroy_image(im1);
}