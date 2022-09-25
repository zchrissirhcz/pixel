#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "gtest/gtest.h"
#include "px_image.h"
#include "px_image_io.h"
#include "px_compare.h"

TEST(image_io, read)
{
    const int height = 256;
    const int width = 250;
    for (int channel : {1, 3, 4})
    {
        //for (std::string ext : {"bmp", "jpg", "png"}) // jpg sucks if channel=1
        for (std::string ext : {"bmp", "png"})
        {
            if (channel == 4 && ext == "bmp")
            {
                continue;
            }
            cv::Mat mat(height, width, CV_8UC(channel));
            for (int i = 0; i < mat.rows; i++)
            {
                uint8_t* sp = mat.data + i * mat.step1();
                for (int j = 0; j < mat.cols; j++)
                {
                    sp[0] = i;
                    if (channel > 1)
                        sp[1] = j;
                    if (channel > 2)
                        sp[2] = (i + j);
                    if (channel > 3)
                        sp[3] = 255;
                    sp += channel;
                }
            }

            std::string image_path = "read." + ext;
            if (ext == "png" && channel == 4)
            {
                std::vector<int> compression_params;
                compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
                compression_params.push_back(9);
                cv::imwrite(image_path, mat, compression_params);
            }
            else
            {
                cv::imwrite(image_path, mat);
            }
            px_image_t* image = px_read_image(image_path.c_str());

            EXPECT_EQ(mat.rows, image->height);
            EXPECT_EQ(mat.cols, image->width);
            ASSERT_EQ(mat.channels(), image->channel) << "channel=" << channel << ", ext=" << ext;

            px_image_t* new_image = px_create_image_header(mat.rows, mat.cols, channel);
            new_image->data = mat.data;
            EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

            px_destroy_image(image);
            px_destroy_image_header(new_image);
        }
    }
}

TEST(image_io, write)
{
    const int height = 256;
    const int width = 250;
    for (int channel : {1, 3, 4})
    {
        //for (std::string ext : {"bmp", "jpg", "png"}) // jpg sucks if channel=1
        for (std::string ext : {"bmp", "png"})
        {
            if (channel == 4 && ext == "bmp")
            {
                continue;
            }
            px_image_t* image = px_create_image(height, width, channel);
            for (int i = 0; i < image->height; i++)
            {
                uint8_t* sp = image->data + i * image->stride;
                for (int j = 0; j < image->width; j++)
                {
                    sp[0] = i;
                    if (channel > 1)
                        sp[1] = j;
                    if (channel > 2)
                        sp[2] = (i + j);
                    if (channel > 3)
                        sp[3] = 255;
                    sp += channel;
                }
            }
            std::string image_path = "write-" + std::to_string(channel) + "." + ext;
            px_write_image(image, image_path.c_str());

            px_image_t* new_image = px_read_image(image_path.c_str());
            std::string save_path = "save-" + std::to_string(channel) + "." + ext;
            px_write_image(new_image, save_path.c_str());

            px_dump_image_meta(image, "image");
            px_dump_image_meta(new_image, "new_image");
            EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

            px_destroy_image(image);
            px_destroy_image(new_image);
        }
    }
}

TEST(image_io, ppm)
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
    px_write_image(image, filename);

    px_image_t* new_image = px_read_image(filename);

    EXPECT_EQ(image->height, new_image->height);
    EXPECT_EQ(image->width, new_image->width);

    EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

    px_destroy_image(image);
    px_destroy_image(new_image);
}

TEST(image_io, pgm)
{
    const int w = 256;
    const int h = 250;
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
    px_write_image(image, filename);

    px_image_t* new_image = px_read_image(filename);

    EXPECT_EQ(image->height, new_image->height);
    EXPECT_EQ(image->width, new_image->width);

    EXPECT_TRUE(px_image_almost_equal(image, new_image, 0));

    px_destroy_image(image);
    px_destroy_image(new_image);
}
