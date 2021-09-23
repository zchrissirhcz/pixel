#include "gtest/gtest.h"

#include <arm_neon.h>

// access to values: val[n]
TEST(arrays, access_to_values)
{
    float32x4_t v0 = { 1.0, 2.0, 3.0, 4.0 }, v1 = { 5.0, 6.0, 7.0, 8.0 }, v2 = { 9.0, 10.0, 11.0, 12.0 }, v3 = { 13.0, 14.0, 15.0, 16.0 };
    float32x4x4_t ary = { v0, v1, v2, v3 };
    float32x4_t v_data = ary.val[2];
    float expected_data[4] = { 9.0, 10.0, 11.0, 12.0 };

    float data[4];
    vst1q_f32(data, v_data);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_data[i] == data[i]);
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
