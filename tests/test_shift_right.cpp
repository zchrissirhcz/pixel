#include "gtest/gtest.h"
#include <stdio.h>

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

/// compare vshrn_n_u16 and vrshrn_n_u16 result
TEST(shift_right, round_or_not)
{
    unsigned short input = 65280 + 128; //65408
    uint16x8_t v_input = vdupq_n_u16(input);
    // 65408/256.0 = 255.5
    uint8x8_t v_out1 = vshrn_n_u16(v_input, 8);
    uint8x8_t v_out2 = vrshrn_n_u16(v_input, 8);
    uint8_t out1[8];
    uint8_t out2[8];
    vst1_u8(out1, v_out1);
    vst1_u8(out2, v_out2);

    // fprintf(stderr, "out1[0]=%d\n", out1[0]); // 255
    // fprintf(stderr, "out2[0]=%d\n", out2[0]); // 0
    EXPECT_TRUE(out1[0] == 255);
    EXPECT_TRUE(out2[0] == 0); // round版本的右移结果等于0，引发显著计算误差

    // unsigned char output = input >> 8;
    // fprintf(stderr, "output = %d\n", output); // 255

    // unsigned char dump = std::round(255.5);
    // fprintf(stderr, "dump = %d\n", output); // 255
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
