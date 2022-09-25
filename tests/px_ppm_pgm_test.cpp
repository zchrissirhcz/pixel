#include "px_compare.h"
#include "px_image.h"
#include "px_ppm_pgm.h"
#include "gtest/gtest.h"

TEST(ppm, io)
{
    const int w = 256;
    const int h = 250;
    const int cn = 3;
    px_image_t* image = px_create_image(h, w, cn);
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = image->data + i * image->stride;
        for (int j = 0; j < w; j++)
        {
            sp[0] = i;
            sp[1] = j;
            sp[2] = (i + j) / 2;
            sp += cn;
        }
    }

    const char* filename = "test.ppm";
    px_write_ppm(filename, image->data, image->height, image->width);
    int new_height;
    int new_width;
    uint8_t* data = px_read_ppm(filename, &new_height, &new_width);

    EXPECT_EQ(image->height, new_height);
    EXPECT_EQ(image->width, new_width);
    px_image_t* new_image = px_create_image_header(new_height, new_width, cn);
    new_image->data = data;

    EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

    px_destroy_image(image);
    px_destroy_image(new_image);
}


TEST(pgm, io)
{
    const int w = 256;
    const int h = 256;
    const int cn = 1;
    px_image_t* image = px_create_image(h, w, cn);
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = image->data + i * image->stride;
        for (int j = 0; j < w; j++)
        {
            sp[0] = i;
            sp += cn;
        }
    }

    const char* filename = "test.pgm";
    px_write_pgm(filename, image->data, image->height, image->width);

    //px_sleep(2); // work around for QEMU fscanf() failure. However, it still fail sometimes. I doubt it QEMU has bug (v7.1.0)

    int new_height;
    int new_width;
    uint8_t* data = px_read_pgm(filename, &new_height, &new_width);

    EXPECT_EQ(image->height, new_height);
    EXPECT_EQ(image->width, new_width);
    px_image_t* new_image = px_create_image_header(new_height, new_width, cn);
    new_image->data = data;
    EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

    px_destroy_image(image);
    px_destroy_image(new_image);
}