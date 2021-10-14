#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

TEST(shift, vshr_n_s8)
{
    const int n = 1;

#if __ARM_NEON
    int8x8_t v1 = {3, 4, -3, -4, 127, 64, 63, 32};
    int8x8_t v_out = vshr_n_s8(v1, n);
    int8_t expected_out[8] = {1, 2, -8, 0, 31, 16, 15, 8};
#else
    pxl::int8x8_t pv1 = {3, 4, -3, -4, 127, 64, 63, 32};
    pxl::int8x8_t pv_out = pxl::vshr_n_s8(v1, n);
#endif

    int8_t a = 3;
    int8_t res = a >> 1;
    fprintf(stderr, ">>> temp, res=%d\n", res);

    res = a / 2;
    fprintf(stderr, ">>> temp, res=%d\n", res);

    a = -3;
    res = a >> 1;
    fprintf(stderr, ">>> temp, res=%d\n", res);

    a = -3;
    res = a / 2;
    fprintf(stderr, ">>> temp, res=%d\n", res);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        //ASSERT_EQ(expected_out[i], out[i]);
        //ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d, %d], ", out[i], pv_out[i]);
        fprintf(stderr, "  %d, ", out[i]);
        //fprintf(stderr, "%d, ", pv_out[i]);
    }
}

TEST(shift, fff)
{
    uint16x8_t vc_1 = vdupq_n_u16((uint16_t)-1);
    uint16_t out[8];
    vst1q_u16(out, vc_1);
    for (int i=0; i<8; i++) {
        fprintf(stderr, "%u, ", out[i]);
    }
    printf("\n");
}

TEST(shift, vqshrun_n_s16)
{
    const int n = 5;
#if __ARM_NEON
    int16x8_t v1 = {1080, 8192, 8160, 3, 4, 5, 6, 7};
    uint8x8_t v_out = vqshrun_n_s16(v1, n);
    uint8_t expected_out[8] = {33, 255, 255, 0, 0, 0, 0, 0};
#else
    pxl::int16x8_t pv1 = {1080, 8192, 8160, 3, 4, 5, 6, 7};
    pxl::uint8x8_t pv_out = vqshrun_n_s16(pv1, n);
#endif

    // temp dump
    int16_t a = 8192;
    uint8_t b = ( a + (1<<4) ) >> 5;
    fprintf(stderr, ">>> b=%d\n", b);

    uint8_t out[8];
    vst1_u8(out, v_out);
    fprintf(stderr, ">>>");
    for (int i=0; i<8; i++) {
        fprintf(stderr, "%d, ", v_out[i]);
    }
}

TEST(shift, vshl_s8)
{
    int8x8_t v1 = {12, 34, 56, 78, 101, 123, -12, -120};
    int8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v_out = vshl_s8(v1, v2);
    int8_t expected_out[8] = {24, -120, -64, -32, -96, -64, 0, 0};

    pxl::int8x8_t pv1 = {12, 34, 56, 78, 101, 123, -12, -120};
    pxl::int8x8_t pv2 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv_out = pxl::vshl_s8(pv1, pv2);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++) {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d, %d], ", out[i], pv_out[i]);
        //fprintf(stderr, "  %d, ", out[i]);
        //fprintf(stderr, "%d, ", pv_out[i]);
    }
    
    int8x8_t vshl_s8(int8x8_t M, int8x8_t N);
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
