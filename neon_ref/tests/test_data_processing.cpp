#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>



// r = vpadd(a, b)
TEST(add, vpadd)
{
    // vpadd_type: 两个输入向量拼接后，拼接向量里每两个相邻元素作为pair，每个pair相加作为结果向量的元素
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18};
    int8x8_t v_out = vpadd_s8(v1, v2);
    int8_t expected_out[8] = {3, 7, 11, 15, 23, 27, 31, 35};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        //fprintf(stderr, "%d, ", out[i]);
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

// r = vadd(a)
TEST(add, vpaddl)
{
    // vadd_type: 输入向量a的每两个相邻（不重叠）元素作为pair，每个pair相加作为结果元素，并且类型拓宽 。
    uint8x8_t v1 = {255, 1, 255, 2, 255, 3, 255, 4};
    uint16x4_t v_out = vpaddl_u8(v1);
    uint16_t expected_out[4] = {256, 257, 258, 259};

    uint16_t out[4];
    vst1_u16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(add, vpadal)
{
    // vpadal: Signed Add and Accumulate Long Pairwise
    // r0 = a0 + (b0 + b1), ..., r3 = a3 + (b6 + b7);
    uint16x4_t v1 = {100, 200, 300, 400};
    uint8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    uint16x4_t v_out = vpadal_u8(v1, v2);
    uint16_t expected_out[4] = {103, 207, 311, 415};
    
    uint16_t out[4];
    vst1_u16(out, v_out);
    for (int i=0; i<4; i++)
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
