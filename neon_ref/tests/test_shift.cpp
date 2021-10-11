#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

TEST(shift, vshr_n_s8)
{
    const int n = 2;

#if __ARM_NEON
    int8x8_t v1 = {-128, -64, -32, 0, 127, 64, 63, 32};
    int8x8_t v_out = vshr_n_s8(v1, n);
    int8_t expected_out[8] = {-32, -16, -8, 0, 31, 16, 15, 8};
#else
    pxl::int8x8_t pv1 = {-128, -64, -32, 0, 127, 64, 63, 32};
    pxl::int8x8_t pv_out = pxl::vshr_n_s8(v1, n);
#endif

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d, %d], ", out[i], pv_out[i]);
        //fprintf(stderr, "%d, ", out[i]);
        //fprintf(stderr, "%d, ", pv_out[i]);
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
