#include "gtest/gtest.h"
#include "px_image.h"
#include "px_image_io.h"
#include "px_compare.h"
#include "px_log.h"

TEST(image_rotate90, 1x2_c1)
{
    //
    // 1 2    =>   1
    //             2

    const int src_height = 1;
    const int src_width = 2;
    const int channel = 1;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    src->data[0] = 1;
    src->data[1] = 2;

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate90(src, dst);

    EXPECT_TRUE(dst->data[0] == 1);
    EXPECT_TRUE(dst->data[1] == 2);
}

TEST(image_rotate90, 1x2_c3)
{
    //
    // 1 2 3 4 5 6    =>   1 2 3
    //                     4 5 6

    const int src_height = 1;
    const int src_width = 2;
    const int channel = 3;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    const int total = src_height * src_width * channel;
    for (int i = 0; i < total; i++)
    {
        src->data[i] = i + 1;
    }

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate90(src, dst);

    for (int i = 0; i < total; i++)
    {
        EXPECT_TRUE(dst->data[i] == i + 1);
    }
}

static bool array_equal_u8(uint8_t* expected, uint8_t* actual, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (expected[i] != actual[i])
        {
            PX_LOGE("not equal: i=%d, expected[%d] (%d) != actual[%d] (%d)\n",
                i,
                i, expected[i],
                i, actual[i]
            );
            return false;
        }
    }
    return true;
}

TEST(image_rotate90, 2x2_c1)
{
    //
    // 1 2    =>   3 1
    // 3 4         4 2

    const int src_height = 2;
    const int src_width = 2;
    const int channel = 1;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    const int total = src_height * src_width * channel;
    for (int i = 0; i < total; i++)
    {
        src->data[i] = i + 1;
    }

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate90(src, dst);

    uint8_t expected[] = {
        3, 1,
        4, 2
    };

    EXPECT_TRUE(array_equal_u8(expected, dst->data, total));
}


TEST(image_rotate90, 2x2_c3)
{
    //
    // 11 12 13  21 22 23   =>   31 32 33  11 12 13
    // 31 32 33  41 42 43        41 42 43  21 22 23

    const int src_height = 2;
    const int src_width = 2;
    const int channel = 3;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    const int total = src_height * src_width * channel;

    uint8_t input[] = {
        11, 12, 13,  21, 22, 23,
        31, 32, 33,  41, 42, 43
    };
    for (int i = 0; i < total; i++)
    {
        src->data[i] = input[i];
    }

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate90(src, dst);

    uint8_t expected[] = {
        31, 32, 33,  11, 12, 13,
        41, 42, 43,  21, 22, 23
    };

    EXPECT_TRUE(array_equal_u8(expected, dst->data, total));
}


TEST(image_rotate270, 1x2_c1)
{
    //
    // 1 2    =>   2
    //             1

    const int src_height = 1;
    const int src_width = 2;
    const int channel = 1;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    src->data[0] = 1;
    src->data[1] = 2;

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate270(src, dst);
    const int total = src_height * src_width * channel;

    uint8_t expected[] = {
        2, 1
    };

    EXPECT_TRUE(array_equal_u8(expected, dst->data, total));
}

TEST(image_rotate270, 1x2_c3)
{
    //
    // 1 2 3 4 5 6    =>   4 5 6
    //                     1 2 3

    const int src_height = 1;
    const int src_width = 2;
    const int channel = 3;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    const int total = src_height * src_width * channel;
    for (int i = 0; i < total; i++)
    {
        src->data[i] = i + 1;
    }

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate270(src, dst);
    uint8_t expected[] = {
        4, 5, 6,
        1, 2, 3
    };

    EXPECT_TRUE(array_equal_u8(expected, dst->data, total));
}

TEST(image_rotate270, 2x2_c1)
{
    //
    // 1 2    =>   2 4
    // 3 4         1 3

    const int src_height = 2;
    const int src_width = 2;
    const int channel = 1;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    const int total = src_height * src_width * channel;
    for (int i = 0; i < total; i++)
    {
        src->data[i] = i + 1;
    }

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate270(src, dst);

    uint8_t expected[] = {
        2, 4,
        1, 3
    };

    EXPECT_TRUE(array_equal_u8(expected, dst->data, total));
}

TEST(image_rotate270, 2x2_c3)
{
    //
    // 11 12 13  21 22 23   =>   21 22 23  41 42 43
    // 31 32 33  41 42 43        11 12 13  31 32 33

    const int src_height = 2;
    const int src_width = 2;
    const int channel = 3;
    px_image_t* src = px_create_image(src_height, src_width, channel);
    const int total = src_height * src_width * channel;

    uint8_t input[] = {
        11, 12, 13,  21, 22, 23,
        31, 32, 33,  41, 42, 43
    };
    for (int i = 0; i < total; i++)
    {
        src->data[i] = input[i];
    }

    const int dst_height = src_width;
    const int dst_width = src_height;
    px_image_t* dst = px_create_image(dst_height, dst_width, channel);

    px_image_rotate270(src, dst);

    uint8_t expected[] = {
        21, 22, 23,  41, 42, 43,
        11, 12, 13,  31, 32, 33
    };

    EXPECT_TRUE(array_equal_u8(expected, dst->data, total));
}