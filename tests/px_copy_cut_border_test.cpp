#include "gtest/gtest.h"
#include "px_compare.h"
#include "px_image.h"
#include "px_typebase.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

namespace cv {
    void copyCutBorder(cv::Mat src, cv::Mat& dst, int top, int bottom, int left, int right)
    {
        cv::Size src_size = src.size();
        cv::Size dst_size;
        dst_size.height = src_size.height - top - bottom;
        dst_size.width = src_size.width - left - right;

        cv::Rect rect;
        rect.x = left;
        rect.y = top;
        rect.width = dst_size.width;
        rect.height = dst_size.height;

        dst = cv::Mat(src, rect).clone();
    }
}

TEST(copy_cut_border, 1_channel)
{
    int height = 3;
    int width = 3;
    px_image_t* src = px_create_image(height, width, 1);

    // 0 1 2
    // 3 4 5
    // 6 7 8
    for (int i = 0; i < height * width; i++)
    {
        src->data[i] = i;
    }

    {
        px_pad_t pad;
        pad.top = 1;
        pad.bottom = 1;
        pad.left = 1;
        pad.right = 1;

        const int new_height = height - pad.top - pad.bottom;
        const int new_width = width - pad.left - pad.right;
        px_image_t* dst = px_create_image(new_height, new_width, 1);
        
        px_copy_cut_border(src, dst, pad);
        EXPECT_TRUE(dst->data[0] = 4);
        px_destroy_image(dst);
    }

    {
        px_pad_t pad;
        pad.top = 0;
        pad.bottom = 1;
        pad.left = 0;
        pad.right = 0;

        const int new_height = height - pad.top - pad.bottom;
        const int new_width = width - pad.left - pad.right;
        px_image_t* dst = px_create_image(new_height, new_width, 1);
        
        px_copy_cut_border(src, dst, pad);
        uint8_t expected[] = {0, 1, 2, 3, 4, 5};
        for (int i = 0; i < new_height * new_width; i++)
        {
            EXPECT_EQ(expected[i], dst->data[i]);
        }
        px_destroy_image(dst);
    }

    px_destroy_image(src);
}

TEST(copy_cut_border, 3_channel)
{
    int height = 3;
    int width = 3;
    px_image_t* src = px_create_image(height, width, 3);

    //  0  1  2  3  4  5  6  7  8
    //  9 10 11 12 13 14 15 16 17
    // 18 19 20 21 22 23 24 25 26
    for (int i = 0; i < height * width * src->channel; i++)
    {
        src->data[i] = i;
    }

    {
        px_pad_t pad;
        pad.top = 1;
        pad.bottom = 1;
        pad.left = 1;
        pad.right = 1;

        const int new_height = height - pad.top - pad.bottom;
        const int new_width = width - pad.left - pad.right;
        px_image_t* dst = px_create_image(new_height, new_width, src->channel);
        
        px_copy_cut_border(src, dst, pad);
        uint8_t expected[] = {12, 13, 14};
        for (int i = 0; i < new_height * new_width * dst->channel; i++)
        {
            EXPECT_EQ(expected[i], dst->data[i]);
        }
        px_destroy_image(dst);
    }

    {
        px_pad_t pad;
        pad.top = 0;
        pad.bottom = 1;
        pad.left = 0;
        pad.right = 0;

        const int new_height = height - pad.top - pad.bottom;
        const int new_width = width - pad.left - pad.right;
        px_image_t* dst = px_create_image(new_height, new_width, src->channel);
        
        px_copy_cut_border(src, dst, pad);
        uint8_t expected[] = {
            0, 1,  2,  3,  4,  5,  6,  7,  8,
            9, 10, 11, 12, 13, 14, 15, 16, 17
        };
        for (int i = 0; i < new_height * new_width * dst->channel; i++)
        {
            EXPECT_EQ(expected[i], dst->data[i]);
        }
        px_destroy_image(dst);
    }

    px_destroy_image(src);
}