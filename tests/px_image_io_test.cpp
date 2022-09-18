#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "gtest/gtest.h"
#include "px_image_io.h"

TEST(image_io, read)
{
    for (int channel : {1, 3, 4})
    {
        for (std::string ext : {"bmp", "jpg", "png"})
        {
            cv::Mat mat(10, 10, CV_8UC(channel));
            std::string image_path = "read." + ext;
            cv::imwrite(image_path, mat);
            px_image_t* image = px_read_image(image_path.c_str());

            EXPECT_EQ(mat.rows, image->height);
            EXPECT_EQ(mat.cols, image->width);
            ASSERT_EQ(mat.channels(), image->channel) << "channel=" << channel << ", ext=" << ext;

            px_destroy_image(image);
        }
    }
}

TEST(image_io, write)
{
    for (int channel : {1, 3, 4})
    {
        for (std::string ext : {"bmp", "jpg", "png"})
        {
            px_image_t* image = px_create_image(10, 10, channel);
            std::string image_path = "write." + ext;
            px_write_image(image, image_path.c_str());

            cv::Mat mat = cv::imread(image_path, cv::IMREAD_UNCHANGED);

            EXPECT_EQ(image->height, mat.rows);
            EXPECT_EQ(image->width, mat.cols);
            ASSERT_EQ(image->channel, mat.channels()) << "channel=" << channel << ", ext=" << ext;

            px_destroy_image(image);
        }
    }
}


// int main(int argc, char* argv[])
// {
//     testing::InitGoogleTest(&argc, argv);

//     // Run a specific test only
//     //testing::GTEST_FLAG(filter) = "MyLibrary.TestReading"; // I'm testing a new feature, run something quickly
//     testing::GTEST_FLAG(filter) = "image_io.write";

//     // Exclude a specific test
//     //testing::GTEST_FLAG(filter) = "-cvtColorTwoPlane.yuv420sp_to_rgb:-cvtColorTwoPlane.rgb_to_yuv420sp"; // The writing test is broken, so skip it

//     return RUN_ALL_TESTS();
// }
