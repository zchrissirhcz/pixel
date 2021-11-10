#include "rgb2gray.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"
#include "common/pixel_cpu_affinity.h"

#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>


bool isGrayImageEqual(const uchar* expected, const uchar* actual, const size_t h, const size_t w)
{
    size_t len = h * w;
    return memcmp(expected, actual, len)==0;
}

bool isImageNearlyEqual(const cv::Mat expected, const cv::Mat actual, const uint32_t tolerance=0)
{
    if ( (expected.rows!=actual.rows) ||
         (expected.cols!=actual.cols) ||
         (expected.channels()!=actual.channels())
    )
    {
        return false;
    }

    const int h = expected.rows;
    const int w = expected.cols;
    const int c = expected.channels();
    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j++)
        {
            for (int k=0; k<c; k++)
            {
                const int idx = i * expected.step + j * c + k;
                uint32_t diff = abs(expected.data[idx] - actual.data[idx]);
                if (diff > tolerance) {
                    PIXEL_LOGE("pixel not equal, (%d,%d,%d)[%d]!=(%d,%d,%d)[%d]\n",
                        i, j, k, expected.data[idx],
                        i, j, k, actual.data[idx]
                    );
                    return false;
                }
            }
        }
    }
    return true;
}

class Rgb2grayFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        rgb_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC1);
        gray_buf = res.data;
        cv::cvtColor(src, expected, cv::COLOR_RGB2GRAY);

        //TODO: bind big core??
    }

    virtual void TearDown() override {
        EXPECT_TRUE(isImageNearlyEqual(expected, res, 1));
    }

    cv::Mat src;
    unsigned char* rgb_buf;
    unsigned char* gray_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

TEST_F(Rgb2grayFixture, naive)
{
    pixel_rgb2gray_naive(rgb_buf, height, width, gray_buf);
    //cv::imwrite("sky_gray_naive.bmp", res);
    
}

TEST_F(Rgb2grayFixture, fixed)
{
    pixel_rgb2gray_fixed(rgb_buf, height, width, gray_buf);
    //cv::imwrite("sky_gray_naive.bmp", res);
}

TEST_F(Rgb2grayFixture, fixed_asimd)
{
    pixel_rgb2gray_fixed_asimd(rgb_buf, height, width, gray_buf);
    //cv::imwrite("sky_gray_asimd.bmp", gray_fixed_asimd);
}

TEST_F(Rgb2grayFixture, opencv)
{
    cv::Mat dst;
    cv::cvtColor(src, dst, cv::COLOR_RGB2GRAY);
    //cv::imwrite("sky_gray_opencv.bmp", gray_opencv);
    res = dst;
}

TEST_F(Rgb2grayFixture, asm)
{
    pixel_rgb2gray_fixed_asm(rgb_buf, height, width, gray_buf);
    //cv::imwrite("sky_gray_fixed_asm.bmp", gray_fixed_asm);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
