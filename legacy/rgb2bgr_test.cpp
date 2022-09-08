#include "px_rgb2bgr.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"
#include "common/pixel_cpu_affinity.h"

#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>

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

class Rgb2bgrFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        expected = src.clone();
        cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;
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

TEST_F(Rgb2bgrFixture, naive)
{
    rgb2bgr_naive(src_buf, height, width, dst_buf);
}

TEST_F(Rgb2bgrFixture, idxopt)
{
    rgb2bgr_idxopt(src_buf, height, width, dst_buf);
}

TEST_F(Rgb2bgrFixture, asimd)
{
    rgb2bgr_asimd(src_buf, height, width, dst_buf);
}

TEST_F(Rgb2bgrFixture, asm)
{
    rgb2bgr_asm(src_buf, height, width, dst_buf);
}

TEST_F(Rgb2bgrFixture, opencv)
{
    cv::cvtColor(src, res, cv::COLOR_BGR2RGB);
}

class Rgb2bgrInplaceFixture : public testing::Test {
public:
    virtual void SetUp() override
    {
        src = cv::imread("sky.jpg");
        ASSERT_TRUE(!src.empty());
        expected = src.clone();
        cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
    }

    virtual void TearDown() override {
        EXPECT_TRUE(isImageNearlyEqual(expected, src, 0));
    }

    cv::Mat src;
    unsigned char* src_buf;
    size_t height;
    size_t width;
    cv::Mat expected;
};

TEST_F(Rgb2bgrInplaceFixture, naive)
{
    rgb2bgr_inplace_naive(src_buf, height, width);
    // for (int hi=0; hi<height; hi++) {
    //     rgb2bgr_inplace_naive(src_buf_line, 1, width);
    //     src_buf_line += width * 3;
    // }
}

TEST_F(Rgb2bgrInplaceFixture, naive2)
{
    rgb2bgr_inplace_naive2(src_buf, height, width);
}

TEST_F(Rgb2bgrInplaceFixture, asm)
{
    rgb2bgr_inplace_asm(src_buf, height, width);
}

TEST_F(Rgb2bgrInplaceFixture, opencv)
{
    cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
}

TEST_F(Rgb2bgrInplaceFixture, opencv_trick)
{
    cv::Mat shadow = src;
    cv::cvtColor(src, shadow, cv::COLOR_BGR2RGB);
}

// 最初用来探索着写 arm neon 汇编代码的 snippet
#ifdef __ARM_NEON

#include <arm_neon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap_u8();

int main1() {
    swap_u8();

    return 0;
}

void swap_u8() {
#ifdef __aarch64__
    #define NUM 48
    uint8_t* src = (uint8_t*)malloc(sizeof(uint8_t)*NUM);
    for (size_t i=0; i<NUM; i++) {
        src[i] = i;
    }
    uint8_t* dst = (uint8_t*)malloc(sizeof(uint8_t)*NUM);

    // 这里的汇编来自 https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a/coding-for-neon/single-page
    // 注意：armv8和armv7的汇编写法不一样！
    // 经过测试，armv8里的大小写并不敏感，比如LD3换成ld3，可以替换使用
    // 另外 https://aijishu.com/a/1060000000116427 这篇也参考到了
    __asm__ volatile(
        "ld3    { v0.16b, v1.16b, v2.16b }, [%1]\n"
        "mov    v3.16b, v0.16b       \n"
        "mov    v0.16b, v2.16b       \n"
        "mov    v2.16b, v3.16b        \n"
        "st3    { v0.16b, v1.16b, v2.16b }, [%0]"
        : "=r"(dst), //%0
        "=r"(src)    //%1
        : "0"(dst),
        "1"(src)
        : "cc", "memory", "v0", "v1", "v2"
    );
#else
    #define NUM 24
    uint8_t* src = (uint8_t*)malloc(sizeof(uint8_t)*NUM);
    for (size_t i=0; i<NUM; i++) {
        src[i] = i;
    }
    uint8_t* dst = (uint8_t*)malloc(sizeof(uint8_t)*NUM);

    __asm__ volatile(
        "vld3.u8    {d0-d2}, [%1]       \n"
        "vswp.u8       d0, d2               \n"
        "vst3.u8    {d0-d2}, [%0]       \n"
        : "=r"(dst), //%0
        "=r"(src)    //%1
        : "0"(dst),
        "1"(src)
        : "cc", "memory", "q0", "q1", "q2"
    );
#endif
    for (size_t i=0; i<NUM; i++) {
        printf("dst[%zu]=%d\n", i, (int)dst[i]);
    }
    free(src);
    free(dst);
}

#endif


int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
