#include "gtest/gtest.h"

#include <arm_neon.h>

// ternary operator: use vector comparison (for example vcltq_f32 for less than comparison)
TEST(conditionals, ternary)
{
    float32x4_t v1 = { 1.0, 0.0, 1.0, 0.0 };
    float32x4_t v2 = { 0.0, 1.0, 1.0, 0.0 };
    float32x4_t mask = vcltq_f32(v1, v2);  // v1 < v2
    float32x4_t ones = vmovq_n_f32(1.0), twos = vmovq_n_f32(2.0);
    float32x4_t v_out = vbslq_f32(mask, ones, twos);  // will select first if mask 0, second if mask 1
    float expected_out[4] = { 2.0, 1.0, 2.0, 2.0 };

    float out[4];
    vst1q_f32(out, v_out);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_out[i] == out[i]);
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
