#include "px_compare.h"
#include "px_image.h"
#include "px_image_io.h"
#include "px_png_libspng.h"
#include "px_log.h"
#include "px_arithm.h"
#include "gtest/gtest.h"

static void test_read_write_png(int channel)
{
    const int height = 256;
    const int width = 250;
    const int cn = channel;
    px_image_t* image = px_create_image(height, width, cn);
    for (int i = 0; i < height; i++)
    {
        uint8_t* sp = image->data + i * image->stride;
        for (int j = 0; j < width; j++)
        {
            sp[0] = i;
            if (cn > 1)
                sp[1] = j;
            if (cn > 2)
                sp[2] = (i + j) / 2;
            if (cn > 3)
                sp[3] = 255;
            sp += cn;
        }
    }
    const char* filename = "bgr.png";
    px_write_png(filename, image->height, image->width, image->channel, image->data);

    int new_height;
    int new_width;
    int new_channel;
    uint8_t* new_data = NULL;
    px_read_png(filename, &new_height, &new_width, &new_channel, &new_data);
    px_image_t* new_image = px_create_image_header(new_height, new_width, new_channel);
    new_image->data = new_data;

    EXPECT_EQ(image->height, new_image->height);
    EXPECT_EQ(image->width, new_image->width);
    EXPECT_EQ(image->channel, new_image->channel);
    EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

    px_destroy_image(image);
    px_destroy_image(new_image);
}

TEST(read_png, rgb)
{
    test_read_write_png(3);
}

TEST(read_png, gray)
{
    test_read_write_png(1);
}

TEST(read_png, rgba)
{
    test_read_write_png(4);
}