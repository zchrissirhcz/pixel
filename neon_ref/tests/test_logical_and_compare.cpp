#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

TEST(logical_and_compare, vceq)
{
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {1, 3, 2, 4, 5, 7, 6, 8};
    uint8x8_t v_out = vceq_s8(v1, v2);
    uint8_t expected_out[8] = {255, 0, 0, 255, 255, 0, 0, 255};

    pxl::int8x8_t pv1 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv2 = {1, 3, 2, 4, 5, 7, 6, 8};
    pxl::uint8x8_t pv_out = vceq_s8(pv1, pv2);

    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(logical_and_compare, vcge)
{
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {1, 2, 2, 4, 5, 7, 6, 8};
    uint8x8_t v_out = vcge_s8(v1, v2);
    uint8_t expected_out[8] = {255, 255, 255, 255, 255, 0, 255, 255};

    pxl::int8x8_t pv1 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv2 = {1, 2, 2, 4, 5, 7, 6, 8};
    pxl::uint8x8_t pv_out = vcge_s8(pv1, pv2);

    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d ", out[i]);
    }
    //fprintf(stderr, "\n");
}


TEST(logical_and_compare, vcle)
{
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {1, 2, 2, 4, 5, 7, 6, 8};
    uint8x8_t v_out = vcle_s8(v1, v2);
    uint8_t expected_out[8] = {255, 255, 0, 255, 255, 255, 0, 255};

    pxl::int8x8_t pv1 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv2 = {1, 2, 2, 4, 5, 7, 6, 8};
    pxl::uint8x8_t pv_out = vcle_s8(pv1, pv2);

    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d ", out[i]);
    }
    //fprintf(stderr, "\n");
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
