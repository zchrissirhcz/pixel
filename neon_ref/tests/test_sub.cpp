#include "gtest/gtest.h"

#include <arm_neon.h>

#include "pixel_neon.hpp"

TEST(sub, vsub)
{
    // vsub_type: ri = ai - bi
    uint8x8_t v1 = {11, 12, 13, 14, 15, 16, 17, 18};
    uint8x8_t v2 = vdup_n_u8(10);
    uint8x8_t v_out = vsub_u8(v1, v2);
    uint8_t expected_out[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    pxl::uint8x8_t pv1 = {11, 12, 13, 14, 15, 16, 17, 18};
    pxl::uint8x8_t pv2 = pxl::vdup_n_u8(10);
    pxl::uint8x8_t pv_out = pxl::vsub_u8(pv1, pv2);
    
    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(sub, vsubl)
{
    // vsubl_type: Signed Subtract Long.
    int8x8_t v1 = vdup_n_s8(127);
    int8x8_t v2 = {0, 1, 2, 3, -1, -2, -3, -4};
    int16x8_t v_out = vsubl_s8(v1, v2);
    int16_t expected_out[8] = {127, 126, 125, 124, 128, 129, 130, 131};

    pxl::int8x8_t pv1 = pxl::vdup_n_s8(127);
    pxl::int8x8_t pv2 = {0, 1, 2, 3, -1, -2, -3, -4};
    pxl::int16x8_t pv_out = pxl::vsubl_s8(pv1, pv2);

    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(sub, vsubw)
{
    // vsubw_type: r = vsubw_type(a, b),  a和r是宽类型，b是窄类型
    int16x8_t v1 = {101, 102, 103, 104, 105, 106, 107, 108};
    int8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    int16x8_t v_out = vsubw_s8(v1, v2);
    int16_t expected_out[8] = {100, 100, 100, 100, 100, 100, 100, 100};

    pxl::int16x8_t pv1 = {101, 102, 103, 104, 105, 106, 107, 108};
    pxl::int8x8_t pv2 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int16x8_t pv_out = vsubw_s8(pv1, pv2);

    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(sub, vsubhn)
{
    // vsubhn_type: sub, half narrow
    // r = vsubhn_type(a, b), a和b是宽类型，r是(a-b)右移n/2位后的结果（高n/2位）
    int16x8_t v1 = {100, 200, 300, 400, 500, 600, 700, 800};
    int16x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v_out = vsubhn_s16(v1, v2);
    int8_t expected_out[8] = {0, 0, 1, 1, 1, 2, 2, 3};

    pxl::int16x8_t pv1 = {100, 200, 300, 400, 500, 600, 700, 800};
    pxl::int16x8_t pv2 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv_out = pxl::vsubhn_s16(pv1, pv2);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(sub, vqsub)
{
    // vqsub_type: ri = sat(ai + bi) 饱和指令，相减结果超出元素的最大值时，元素就取最大值。
    // 相减结果如果小于当前类型最小值， 则元素就取类型最小值。
    {
        uint8x8_t v1 = vdup_n_u8(250);
        uint8x8_t v2 = {1, 2, 3, 4, 251, 252, 253, 254};
        uint8x8_t v_out = vqsub_u8(v1, v2);
        uint8_t expected_out[8] = {249, 248, 247, 246, 0, 0, 0, 0};

        pxl::uint8x8_t pv1 = pxl::vdup_n_u8(250);
        pxl::uint8x8_t pv2 = {1, 2, 3, 4, 251, 252, 253, 254};
        pxl::uint8x8_t pv_out = pxl::vqsub_u8(pv1, pv2);

        uint8_t out[8];
        vst1_u8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            // fprintf(stderr, "[%d, ", out[i]);
            // fprintf(stderr, "%d, ]", pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        int8x8_t v1 = vdup_n_s8(120);
        int8x8_t v2 = {-11, 2, 3, 4, 121, 122, 123, 124};
        int8x8_t v_out = vqsub_s8(v1, v2);
        int8_t expected_out[8] = {127, 118, 117, 116, -1, -2, -3, -4};

        pxl::int8x8_t pv1 = pxl::vdup_n_s8(120);
        pxl::int8x8_t pv2 = {-11, 2, 3, 4, 121, 122, 123, 124};
        pxl::int8x8_t pv_out = pxl::vqsub_s8(pv1, pv2);

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            // fprintf(stderr, "[%d, ", out[i]);
            // fprintf(stderr, "%d, ]", pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }
}

TEST(sub, vhsub)
{
    int8x8_t v1 = vdup_n_s8(120);
    int8x8_t v2 = {1, 2, 3, 4, 120, 121, 122, 123};
    int8x8_t v_out = vhsub_s8(v1, v2);
    int8_t expected_out[8] = {59, 59, 58, 58, 0, -1, -1, -2};

    pxl::int8x8_t pv1 = pxl::vdup_n_s8(120);
    pxl::int8x8_t pv2 = {1, 2, 3, 4, 120, 121, 122, 123};
    pxl::int8x8_t pv_out = pxl::vhsub_s8(pv1, pv2);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        // fprintf(stderr, "[%d, ", out[i]);
        // fprintf(stderr, "%d, ]", pv_out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(sub, vrsubhn)
{
    int16x8_t v1 = vdupq_n_u16(850);
    int16x8_t v2 = {200, 400, 600, 800, 1000, 1200, 1400, 1600};
    int8x8_t v_out = vrsubhn_s16(v1, v2);
    int8_t expected_out[8] = {3, 2, 1, 0, -1, -1, -2, -3};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    // Run a specific test only
    //testing::GTEST_FLAG(filter) = "MyLibrary.TestReading"; // I'm testing a new feature, run something quickly
    //testing::GTEST_FLAG(filter) = "pyrDown.PyrDownVec_16u8u";

    // Exclude a specific test
    //testing::GTEST_FLAG(filter) = "-cvtColorTwoPlane.yuv420sp_to_rgb:-cvtColorTwoPlane.rgb_to_yuv420sp"; // The writing test is broken, so skip it

    return RUN_ALL_TESTS();
}
