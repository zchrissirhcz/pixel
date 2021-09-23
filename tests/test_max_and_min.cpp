#include "gtest/gtest.h"

#include <arm_neon.h>

// max of two vectors, element by element:
TEST(max, element_wise)
{
    float32x4_t v_in0 = { 5.0, 2.0, 3.0, 4.0 };
    float32x4_t v_in1 = { 1.0, 6.0, 7.0, 8.0 };
    float32x4_t v_out = vmaxq_f32(v_in0, v_in1);
    float expected_out[4] = { 5.0, 6.0, 7.0, 8.0 };

    float out[4];
    vst1q_f32(out, v_out);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_out[i] == out[i]);
    }
}

// max of vector elements, using folding maximum:
TEST(max, foding)
{
    float32x4_t v0 = { 1.0, 2.0, 3.0, 4.0 };
    float32x2_t maxOfHalfs = vpmax_f32(vget_low_f32(v0), vget_high_f32(v0));
    float32x2_t maxOfMaxOfHalfs = vpmax_f32(maxOfHalfs, maxOfHalfs);
    float maxValue = vget_lane_f32(maxOfMaxOfHalfs, 0);
    float expected_maxValue = 4.0;

    EXPECT_TRUE(expected_maxValue == maxValue);
}

// min of two vectors, element by element:
TEST(min, element_wise)
{
    float32x4_t v_in0 = { 5.0, 2.0, 3.0, 4.0 };
    float32x4_t v_in1 = { 1.0, 6.0, 7.0, 8.0 };
    float32x4_t v_out = vminq_f32(v_in0, v_in1);
    float expected_out[4] = { 1.0, 2.0, 3.0, 4.0 };

    float out[4];
    vst1q_f32(out, v_out);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_out[i] == out[i]);
    }
}

// min of vector elements, using folding minimum:
TEST(min, folding)
{
    float32x4_t v0 = { 1.0, 2.0, 3.0, 4.0 };
    float32x2_t minOfHalfs = vpmin_f32(vget_low_f32(v0), vget_high_f32(v0));
    float32x2_t minOfMinOfHalfs = vpmin_f32(minOfHalfs, minOfHalfs);
    float minValue = vget_lane_f32(minOfMinOfHalfs, 0);
    float expected_minValue = 1.0;
    
    EXPECT_TRUE(expected_minValue == minValue);
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
