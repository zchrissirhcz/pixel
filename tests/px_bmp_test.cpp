#include "px_bmp.h"
#include "px_compare.h"
#include "px_log.h"
#include "px_arithm.h"
#include "px_timer.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "gtest/gtest.h"
#include "px_opencv.hpp"
#include "px_image.h"

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
    // // bpp=24, no align, save image
    // const char* bgr_image_path = "mingren.bmp";
    // test_write_bmp_no_custom(bgr_image_path);

    // // bpp=1, save image, no align
    // const char* gray_image_path = "ring.bmp";
    // test_write_bmp_no_custom(gray_image_path);

    printf("hello world");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    // Run a specific test only
    //testing::GTEST_FLAG(filter) = "MyLibrary.TestReading"; // I'm testing a new feature, run something quickly
    //testing::GTEST_FLAG(filter) = "read_bmp.no_align";

    // Exclude a specific test
    //testing::GTEST_FLAG(filter) = "-cvtColorTwoPlane.yuv420sp_to_rgb:-cvtColorTwoPlane.rgb_to_yuv420sp"; // The writing test is broken, so skip it

    return RUN_ALL_TESTS();
}
