#include "gtest/gtest.h"

#include <arm_neon.h>

// load vector: vld1q_f32 or vld1q_f64
TEST(load, vector)
{
    float raw_data[5] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    float32x4_t v_data = vld1q_f32(raw_data);
    float expected_data[4] = { 1.0, 2.0, 3.0, 4.0 };

    float data[4];
    vst1q_f32(data, v_data);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_data[i]==data[i]);
    }
}

// load same value for all lanes: vld1q_dup_f32 or vld1q_dup_f64
TEST(load, vector_with_same_value)
{
    float val = 3.0;
    float32x4_t v_data = vld1q_dup_f32(&val);
    float expected_data[4] = { 3.0, 3.0, 3.0, 3.0 };

    float data[4];
    vst1q_f32(data, v_data);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_data[i] == data[i]);
    }
}

// set all lanes to a hardcoded value: vmovq_n_f16 or vmovq_n_f32 or vmovq_n_f64
TEST(load, lanes)
{
    float32x4_t v_data = vmovq_n_f32(1.5);
    float expected_data[4] = { 1.5, 1.5, 1.5, 1.5 };

    float data[4];
    vst1q_f32(data, v_data);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_data[i] == data[i]);
    }
}

//----------------------------------------------------------------------

// store vector: vst1q_f32 or vst1q_f64
TEST(store, vector)
{
    float32x4_t v_data = { 1.0, 2.0, 3.0, 4.0 };
    float data[5];
    vst1q_f32(data, v_data);
    
    float expected_data[5] = { 1.0, 2.0, 3.0, 4.0, -233}; //note: the last element should be compared. it's undef
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_data[i] == data[i]);
    }
}

// store lane of array of vectors: vst4q_lane_f16 or vst4q_lane_f32 or vst4q_lane_f64 (change to vst1... / vst2... / vst3... for other array lengths);
TEST(store, lanes)
{
    float32x4_t v0 = { 1.0, 2.0, 3.0, 4.0 }, v1 = { 5.0, 6.0, 7.0, 8.0 }, v2 = { 9.0, 10.0, 11.0, 12.0 }, v3 = { 13.0, 14.0, 15.0, 16.0 };
    float32x4x4_t u = { v0, v1, v2, v3 };
    float buff[4];
    vst4q_lane_f32(buff, u, 0);
    float expected_buff[4] = { 1.0, 5.0, 9.0, 13.0 };

    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_buff[i] == buff[i]);
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
