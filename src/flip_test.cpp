#include "flip.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"
#include "common/pixel_cpu_affinity.h"

#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>

//cv::flip(src, dst, mode);
//mode=0, 上下颠倒  vertically
//mode=1，左右颠倒  horizontally
//mode=-1, against center point

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

class FlipHorizonRgbFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        cv::flip(src, expected, 1);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;

        PIXEL_LOGD("image info: height=%zu, width=%zu\n", height, width);
    }

    virtual void TearDown() override {
        EXPECT_TRUE(isImageNearlyEqual(expected, res, 0));
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

TEST_F(FlipHorizonRgbFixture, naive)
{
    flip_horiz_rgb_naive(src_buf, height, width, dst_buf);
}

TEST_F(FlipHorizonRgbFixture, idxopt)
{
    flip_horiz_rgb_idxopt(src_buf, height, width, dst_buf);
}

TEST_F(FlipHorizonRgbFixture, asimd)
{
    flip_horiz_rgb_asimd(src_buf, height, width, dst_buf);
}

TEST_F(FlipHorizonRgbFixture, opencv)
{
    cv::flip(src, res, 1);
}


class FlipHorizonGrayFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        cv::cvtColor(src, expected, cv::COLOR_BGR2GRAY);
        cv::flip(expected, expected, 1);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;

        PIXEL_LOGD("image info: height=%zu, width=%zu\n", height, width);
    }

    virtual void TearDown() override {
        EXPECT_TRUE(isImageNearlyEqual(expected, res, 0));
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

TEST_F(FlipHorizonGrayFixture, naive)
{
    flip_horiz_gray_naive(src_buf, height, width, dst_buf);
}

TEST_F(FlipHorizonGrayFixture, asimd)
{
    flip_horiz_gray_asimd(src_buf, height, width, dst_buf);
}

TEST_F(FlipHorizonGrayFixture, opencv)
{
    cv::flip(src, res, 1);
}

class FlipVerticalRgbFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        cv::flip(src, expected, 0);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;

        PIXEL_LOGD("image info: height=%zu, width=%zu\n", height, width);
    }

    virtual void TearDown() override {
        EXPECT_TRUE(isImageNearlyEqual(expected, res, 0));
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

TEST_F(FlipVerticalRgbFixture, naive)
{
    flip_vert_rgb_naive(src_buf, height, width, dst_buf);
}

TEST_F(FlipVerticalRgbFixture, opencv)
{
    cv::flip(src, res, 0);
}

TEST_F(FlipVerticalRgbFixture, opencv_inplace)
{
    res = src;
    cv::flip(res, res, 0);
}

TEST_F(FlipVerticalRgbFixture, bylines)
{
    flip_vert_rgb_bylines(src_buf, height, width, dst_buf);
}

class FlipVerticalGrayFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        cv::cvtColor(src, expected, cv::COLOR_BGR2GRAY);
        cv::flip(expected, expected, 0);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;

        PIXEL_LOGD("image info: height=%zu, width=%zu\n", height, width);
    }

    virtual void TearDown() override {
        EXPECT_TRUE(isImageNearlyEqual(expected, res, 0));
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

TEST_F(FlipVerticalGrayFixture, opencv)
{
    cv::flip(src, res, 0);
}

TEST_F(FlipVerticalGrayFixture, naive)
{
    flip_vert_gray_naive(src_buf, height, width, dst_buf);
}

TEST_F(FlipVerticalGrayFixture, bylines)
{
    flip_vert_gray_bylines(src_buf, height, width, dst_buf);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
