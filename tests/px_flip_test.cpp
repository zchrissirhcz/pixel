#include "px_image.h"
#include "gtest/gtest.h"
#include "px_compare.h"

#include <opencv2/opencv.hpp>

TEST(flip_gray, naive)
{
    px_image_t* src = px_create_image(2, 2, 1);
    // A  B
    // C  D
    src->data[0] = 'A';
    src->data[1] = 'B';
    src->data[2] = 'C';
    src->data[3] = 'D';

    px_image_t* expected = px_create_image(2, 2, 1);
    px_image_t* actual = px_create_image(2, 2, 1);

    {
        // C  D
        // A  B
        PX_FLIP_MODE flipmode = PX_FLIP_VERTICAL;
        expected->data[0] = 'C';
        expected->data[1] = 'D';
        expected->data[2] = 'A';
        expected->data[3] = 'B';
        px_flip(src, actual, flipmode);

        EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));
    }

    {
        // B  A
        // D  C
        PX_FLIP_MODE flipmode = PX_FLIP_HORIZONTAL;
        expected->data[0] = 'B';
        expected->data[1] = 'A';
        expected->data[2] = 'D';
        expected->data[3] = 'C';
        px_flip(src, actual, flipmode);

        // printf("debug:\n");
        // printf("%d, %d\n%d, %d\n", actual->data[0], actual->data[1], actual->data[2], actual->data[3]);

        EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));
    }

    {
        // D  C
        // B  A
        PX_FLIP_MODE flipmode = PX_FLIP_BOTH;
        expected->data[0] = 'D';
        expected->data[1] = 'C';
        expected->data[2] = 'B';
        expected->data[3] = 'A';
        px_flip(src, actual, flipmode);

        // printf("debug:\n");
        // printf("%c, %c\n%c, %c\n", actual->data[0], actual->data[1], actual->data[2], actual->data[3]);

        EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));
    }

    px_destroy_image(src);
    px_destroy_image(actual);
    px_destroy_image(expected);
}

TEST(flip_rgb, naive)
{
    px_image_t* src = px_create_image(2, 2, 3);
    // ABC  BCD
    // CDE  DEF
    src->data[0] = 'A';
    src->data[1] = 'B';
    src->data[2] = 'C';

    src->data[3] = 'B';
    src->data[4] = 'C';
    src->data[5] = 'D';

    src->data[6] = 'C';
    src->data[7] = 'D';
    src->data[8] = 'E';

    src->data[9] = 'D';
    src->data[10] = 'E';
    src->data[11] = 'F';

    px_image_t* expected = px_create_image(2, 2, 3);
    px_image_t* actual = px_create_image(2, 2, 3);

    {
        // CDE  DEF
        // ABC  BCD
        PX_FLIP_MODE flipmode = PX_FLIP_VERTICAL;
        expected->data[0] = 'C';
        expected->data[1] = 'D';
        expected->data[2] = 'E';

        expected->data[3] = 'D';
        expected->data[4] = 'E';
        expected->data[5] = 'F';

        expected->data[6] = 'A';
        expected->data[7] = 'B';
        expected->data[8] = 'C';

        expected->data[9] = 'B';
        expected->data[10] = 'C';
        expected->data[11] = 'D';

        px_flip(src, actual, flipmode);

        EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));
    }

    {
        // BCD  ABC
        // DEF  CDE
        PX_FLIP_MODE flipmode = PX_FLIP_HORIZONTAL;
        expected->data[0] = 'B';
        expected->data[1] = 'C';
        expected->data[2] = 'D';

        expected->data[3] = 'A';
        expected->data[4] = 'B';
        expected->data[5] = 'C';

        expected->data[6] = 'D';
        expected->data[7] = 'E';
        expected->data[8] = 'F';

        expected->data[9] = 'C';
        expected->data[10] = 'D';
        expected->data[11] = 'E';

        px_flip(src, actual, flipmode);

        // printf("debug:\n");
        // printf("%d, %d\n%d, %d\n", actual->data[0], actual->data[1], actual->data[2], actual->data[3]);

        EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));
    }

    {
        // DEF  CDE
        // BCD  ABC
        PX_FLIP_MODE flipmode = PX_FLIP_BOTH;
        expected->data[0] = 'D';
        expected->data[1] = 'E';
        expected->data[2] = 'F';

        expected->data[3] = 'C';
        expected->data[4] = 'D';
        expected->data[5] = 'E';

        expected->data[6] = 'B';
        expected->data[7] = 'C';
        expected->data[8] = 'D';

        expected->data[9] = 'A';
        expected->data[10] = 'B';
        expected->data[11] = 'C';

        px_flip(src, actual, flipmode);

        // printf("debug:\n");
        // printf("%c, %c\n%c, %c\n", actual->data[0], actual->data[1], actual->data[2], actual->data[3]);

        EXPECT_TRUE(px_image_almost_equal(expected, actual, 0));
    }

    px_destroy_image(src);
    px_destroy_image(actual);
    px_destroy_image(expected);
}

TEST(flip, opencv)
{
    cv::Mat src(2, 2, CV_8UC1);
    src.data[0] = 'A';
    src.data[1] = 'B';
    src.data[2] = 'C';
    src.data[3] = 'D';

    cv::Mat dst;
    cv::flip(src, dst, 0);
    //printf("%c, %c\n%c, %c\n", dst.data[0], dst.data[1], dst.data[2], dst.data[3]);
    // C, D
    // A, B

    cv::flip(src, dst, 1);
    //printf("%c, %c\n%c, %c\n", dst.data[0], dst.data[1], dst.data[2], dst.data[3]);
    // B, A
    // D, C

    cv::flip(src, dst, -1);
    //printf("%c, %c\n%c, %c\n", dst.data[0], dst.data[1], dst.data[2], dst.data[3]);
    // D, C
    // B, A
}

TEST(flip_inplace, rgb)
{
    px_image_t* src = px_create_image(2, 2, 3);
    // ABC  BCD
    // CDE  DEF
    src->data[0] = 'A';
    src->data[1] = 'B';
    src->data[2] = 'C';

    src->data[3] = 'B';
    src->data[4] = 'C';
    src->data[5] = 'D';

    src->data[6] = 'C';
    src->data[7] = 'D';
    src->data[8] = 'E';

    src->data[9] = 'D';
    src->data[10] = 'E';
    src->data[11] = 'F';

    px_image_t* expected = px_create_image(2, 2, 3);

    {
        // CDE  DEF
        // ABC  BCD
        PX_FLIP_MODE flipmode = PX_FLIP_VERTICAL;
        expected->data[0] = 'C';
        expected->data[1] = 'D';
        expected->data[2] = 'E';

        expected->data[3] = 'D';
        expected->data[4] = 'E';
        expected->data[5] = 'F';

        expected->data[6] = 'A';
        expected->data[7] = 'B';
        expected->data[8] = 'C';

        expected->data[9] = 'B';
        expected->data[10] = 'C';
        expected->data[11] = 'D';

        px_flip_inplace(src, flipmode);

        EXPECT_TRUE(px_image_almost_equal(expected, src, 0));
    }

    px_destroy_image(src);
    px_destroy_image(expected);
}