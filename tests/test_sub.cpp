#include "gtest/gtest.h"

#include <arm_neon.h>

TEST(sub, vsub)
{
    // vsub_type: ri = ai - bi
    uint8x8_t v1 = {11, 12, 13, 14, 15, 16, 17, 18};
    uint8x8_t v2 = vdup_n_u8(10);
    uint8x8_t v_out = vsub_u8(v1, v2);
    uint8_t expected_out[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    
    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(sub, vsubl)
{
    // vsubl_type: Signed Subtract Long.
    int8x8_t v1 = vdup_n_s8(127);
    int8x8_t v2 = {0, 1, 2, 3, -1, -2, -3, -4};
    int16x8_t v_out = vsubl_s8(v1, v2);
    int16_t expected_out[8] = {127, 126, 125, 124, 128, 129, 130, 131};
    
    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(sub, vsubw)
{
    // vsubw_type: r = vsubw_type(a, b),  a和r是宽类型，b是窄类型
    int16x8_t v1 = {101, 102, 103, 104, 105, 106, 107, 108};
    int8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    int16x8_t v_out = vsubw_s8(v1, v2);
    int16_t expected_out[8] = {100, 100, 100, 100, 100, 100, 100, 100};

    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(sub, vsubhn)
{
    // vsubhn_type: sub, half narrow
    // r = vsubhn_type(a, b), a和b是宽类型，r是(a-b)右移n/2位后的结果（高n/2位）
    int16x8_t v1 = {131, 131, 103, 104, 105, 106, 107, 108};
    int16x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v_out = vsubhn_s16(v1, v2);
    int8_t expected_out[8] = {3, 2, 0, 0, 0, 0, 0, 0};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
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

        uint8_t out[8];
        vst1_u8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
        }
    }

    {
        int8x8_t v1 = vdup_n_u8(120);
        int8x8_t v2 = {-11, 2, 3, 4, 121, 122, 123, 124};
        int8x8_t v_out = vqsub_u8(v1, v2);
        int8_t expected_out[8] = {127, 118, 117, 116, 0, 0, 0, 0};

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
        }
    }
}

TEST(sub, vhsub)
{
    //TODO
}

TEST(sub, vrsubhn)
{
    //TODO
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
