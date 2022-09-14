#include "px_bmp.h"
#include "px_log.h"
#include "px_arithm.h"
#include "px_timer.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "gtest/gtest.h"
#include "px_opencv.hpp"

void test_read_image_no_custom(const char* filename)
{
    unsigned char* buffer = NULL;
    int height;
    int width;
    int channel;
    px_read_bmp(filename, &height, &width, &channel, &buffer);

    cv::Size size;
    size.height = height;
    size.width = width;
    cv::Mat actual(size, CV_8UC(channel), buffer);

    cv::Mat expected = cv::imread(filename, cv::IMREAD_UNCHANGED);
    EXPECT_TRUE(almostEqual(expected, actual));

    free(buffer);
}

TEST(read_bmp, no_align)
{
    // bpp=24, BGR image, not aligned
    const char* bgr_image_path = "mingren.bmp"; //width=450, not multiple of 4
    test_read_image_no_custom(bgr_image_path);

    // gray image (bpp=8, using palette to fill color), not aligned
    const char* gray_image_path = "ring.bmp";
    test_read_image_no_custom(gray_image_path);
}

void test_read_bmp_with_custom(const char* filename, int align, bool swap_color)
{
    unsigned char* buffer = NULL;
    int height;
    int width;
    int channel;
    px_read_bmp_custom(filename, &height, &width, &channel, &buffer, align, swap_color);
    
    cv::Size size;
    size.height = height;
    size.width = width;
    int step = px_align_up(width*channel, align); //!! important
    cv::Mat actual(size, CV_8UC(channel), buffer, step);

    cv::Mat im0 = cv::imread(filename, cv::IMREAD_UNCHANGED);
    std::vector<uint8_t> tmp_buf(step * height);
    cv::Mat expected = cv::Mat(im0.size(), im0.type(), tmp_buf.data(), step);
    for (int i = 0; i < expected.rows; i++)
    {
        expected.row(i) = im0.row(i);
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < channel; k++)
            {
                expected.ptr(i, j)[k] = im0.ptr(i, j)[k];
            }
        }
    }

    EXPECT_EQ(expected.rows, actual.rows);
    EXPECT_EQ(expected.cols, actual.cols);
    EXPECT_EQ(expected.channels(), actual.channels());
    EXPECT_EQ(expected.step1(), actual.step1());

    EXPECT_TRUE(almostEqual(expected, actual));

    free(buffer);
}

TEST(read_bmp, align)
{
    const int align = 4;
    const bool swap_color = false;

    // bpp=24, BGR image, aligned
    const char* bgr_image_path = "mingren.bmp";
    test_read_bmp_with_custom(bgr_image_path, align, swap_color);

    // gray image (bpp=8, using palette to fill color), align = 4
    const char* gray_image_path = "mingren_gray.bmp";
    test_read_bmp_with_custom(gray_image_path, align, swap_color);
}

void test_write_bmp_no_custom(const char* filepath)
{
    cv::Mat mat = cv::imread(filepath, cv::IMREAD_UNCHANGED);
    unsigned char* buffer = mat.data;
    cv::Size size = mat.size();
    int height = size.height;
    int width = size.width;
    int channels = mat.channels();
    const char* save_path = "result1.bmp";
    px_write_bmp(save_path, height, width, channels, buffer);

    cv::Mat mat2 = cv::imread(save_path, cv::IMREAD_UNCHANGED);
    EXPECT_EQ(mat.rows, mat2.rows);
    EXPECT_EQ(mat.cols, mat2.cols);
    EXPECT_EQ(mat.channels(), mat2.channels());
    EXPECT_EQ(mat.step1(), mat2.step1());
    EXPECT_TRUE(almostEqual(mat, mat2));
}

TEST(write_bmp, no_custom)
{
    // bpp=24, no align, save image
    const char* bgr_image_path = "mingren.bmp";
    test_write_bmp_no_custom(bgr_image_path);

    // bpp=1, save image, no align
    const char* gray_image_path = "ring.bmp";
    test_write_bmp_no_custom(gray_image_path);
}

static void test_write_bmp_custom(const char* filepath, int align, bool swap_color)
{
    unsigned char* buffer = NULL;
    int height;
    int width;
    int channel;
    px_read_bmp_custom(filepath, &height, &width, &channel, &buffer, align, swap_color);
    
    int linebytes = px_align_up(width*channel, align);
    px_write_bmp_custom("result2.bmp", height, width, channel, buffer, linebytes, swap_color);
}

TEST(write_bmp, custom_align)
{
    const int align = 4;
    const bool swap_color = false;

    // bpp=24, align, save image
    const char* bgr_image_path = "mingren.bmp";
    test_write_bmp_custom(bgr_image_path, align, swap_color);

    // bpp=1, save image, align=4
    const char* gray_image_path = "mingren_gray.bmp";
    test_write_bmp_custom(gray_image_path, align, swap_color);
}

TEST(write_bmp, custom_color_order)
{
    const int align = 1;
    const bool swap_color = true;

    // bpp=24, no align, save image, rgb/bgr swap
    const char* bgr_image_path = "mingren.bmp";
    test_write_bmp_custom(bgr_image_path, align, swap_color);
}

TEST(write_bmp, custom_align_and_color_order)
{
    const int align = 4;
    const bool swap_color = true;

    // bpp=24, align, save image, rgb/bgr swap
    const char* bgr_image_path = "mingren.bmp";
    test_write_bmp_custom(bgr_image_path, align, swap_color);

    // bpp=1, save image, align=4
    const char* gray_image_path = "mingren_gray.bmp";
    test_write_bmp_custom(gray_image_path, align, swap_color);
}

TEST(write_bmp, bgra_4_channels)
{
    cv::Mat src(256, 256, CV_8UC4);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            src.ptr(i, j)[0] = i;
            src.ptr(i, j)[1] = j;
            src.ptr(i, j)[2] = (i + j) / 2;
            src.ptr(i, j)[3] = 0; // alpha
        }
    }

    //cv::imwrite("bgra_by_opencv.bmp", src);

    const char* save_path = "bgra_by_pixel.bmp";
    px_write_bmp(save_path, src.rows, src.cols, src.channels(), src.data);
}

TEST(read_bmp, bgra_4_channels)
{
    cv::Mat src(256, 256, CV_8UC4);
    const char* image_path = "bgra_by_opencv.bmp";
    cv::imwrite(image_path, src);

    int height;
    int width;
    int channels;
    uint8_t* buffer;
    px_read_bmp(image_path, &height, &width, &channels, &buffer);
    cv::Mat shadow(height, width, CV_8UC(channels), buffer);
    EXPECT_EQ(channels, 3);
    EXPECT_EQ(height, src.rows);
    EXPECT_EQ(width, src.cols);

    free(buffer);
}

TEST(read_bmp, gray)
{
    cv::Mat src(256, 256, CV_8UC1);
    const char* image_path = "gray_by_opencv.bmp";
    cv::imwrite(image_path, src);

    int height;
    int width;
    int channels;
    uint8_t* buffer;
    px_read_bmp(image_path, &height, &width, &channels, &buffer);
    cv::Mat shadow(height, width, CV_8UC(channels), buffer);

    EXPECT_EQ(channels, 1);
    EXPECT_EQ(height, src.rows);
    EXPECT_EQ(width, src.cols);

    free(buffer);
}


int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    // Run a specific test only
    //testing::GTEST_FLAG(filter) = "MyLibrary.TestReading"; // I'm testing a new feature, run something quickly
    testing::GTEST_FLAG(filter) = "read_bmp.no_align";

    // Exclude a specific test
    //testing::GTEST_FLAG(filter) = "-cvtColorTwoPlane.yuv420sp_to_rgb:-cvtColorTwoPlane.rgb_to_yuv420sp"; // The writing test is broken, so skip it

    return RUN_ALL_TESTS();
}
