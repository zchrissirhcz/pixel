#include "gtest/gtest.h"

#include <arm_neon.h>

#include "pixel_neon.hpp"

TEST(mul, vmul_s8)
{
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {2, 100, -10, -100, 2, 3, 4, 5};
    int8x8_t v_out = vmul_s8(v1, v2);
    int8_t expected_out[8] = {2, -56, -30, 112, 10, 18, 28, 40};

    pxl::int8x8_t pv1 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv2 = {2, 100, -10, -100, 2, 3, 4, 5};
    pxl::int8x8_t pv_out = pxl::vmul_s8(pv1, pv2);

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

TEST(mul, vmul_n_s16)
{
    int16x4_t v1 = {1, 2, 3, 4};
    int16_t s = 2;
    int16x4_t v_out = vmul_n_s16(v1, s);
    int16_t expected_out[4] = {2, 4, 6, 8};

    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv_out = pxl::vmul_n_s16(pv1, s);

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
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
