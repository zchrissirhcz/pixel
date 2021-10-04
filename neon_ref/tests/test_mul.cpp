#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

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

TEST(mul, vmul_lane)
{
    const int lane = 0;
    int16_t expected_out[4] = {0, 2, 4, 6};
#if __ARM_NEON
    int16x4_t v1 = {0, 1, 2, 3};
    int16x4_t v2 = {2, 4, 6, 8};
    int16x4_t v_out = vmul_lane_s16(v1, v2, lane);
#else
    pxl::int16x4_t v1 = {0, 1, 2, 3};
    pxl::int16x4_t v2 = {2, 4, 6, 8};
    pxl::int16x4_t v_out = ::pxl::vmul_lane_s16(v1, v2, lane);
#endif

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vmull)
{
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = vdup_n_s8(100);
    int16x8_t v_out = vmull_s8(v1, v2);
    int16_t expected_out[8] = {100, 200, 300, 400, 500, 600, 700, 800};

    pxl::int8x8_t pv1 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv2 = pxl::vdup_n_s8(100);
    pxl::int16x8_t pv_out = pxl::vmull_s8(pv1, pv2);

    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vmull_n)
{
    int16x4_t v1 = {1, 3, 5, 7};
    int16_t s = 10000;
    int32x4_t v_out = vmull_n_s16(v1, s);
    int32_t expected_out[4] = {10000, 30000, 50000, 70000};

    pxl::int16x4_t pv1 = {1, 3, 5, 7};
    pxl::int32x4_t pv_out = pxl::vmull_n_s16(pv1, s);

    int32_t out[4];
    vst1q_s32(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(mul, vmull_lane)
{
    const int lane = 2;
#if __ARM_NEON
    int16x4_t v1 = {1, 3, 5, 7};
    int16x4_t v2 = {10000, 20000, 20500, 30000};
    int32x4_t v_out = vmull_lane_s16(v1, v2, lane);
#else
    pxl::int16x4_t v1 = {1, 3, 5, 7};
    pxl::int16x4_t v2 = {10000, 20000, 20500, 30000};
    pxl::int32x4_t v_out = pxl::vmull_lane_s16(v1, v2, lane);
#endif
    int32_t expected_out[4] = {20500, 61500, 102500, 143500};

    int32_t out[4];
    vst1q_s32(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqdmull)
{
    int16x4_t v1 = {1, 2, 3, 32000};
    int16x4_t v2 = {2, 3, 4, 32000};
    int32x4_t v_out = vqdmull_s16(v1, v2);
    int32_t expected_out[4] = {4, 12, 24, 2048000000};

    pxl::int16x4_t pv1 = {1, 2, 3, 32000};
    pxl::int16x4_t pv2 = {2, 3, 4, 32000};
    pxl::int32x4_t pv_out = pxl::vqdmull_s16(pv1, pv2);

    int32_t out[4];
    vst1q_s32(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}


TEST(mul, vqdmull_n)
{
    int16x4_t v1 = {1, 2, 3, 32000};
    int16_t scale = 30000;
    int32x4_t v_out = vqdmull_n_s16(v1, scale);
    int32_t expected_out[4] = {60000, 120000, 180000, 1920000000};

    pxl::int16x4_t pv1 = {1, 2, 3, 32000};
    pxl::int32x4_t pv_out = pxl::vqdmull_n_s16(pv1, scale);

    int32_t out[4];
    vst1q_s32(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqdmull_lane)
{
    const int lane = 1;
    int32_t expected_out[4] = {4, 8, 12, 128000};
#if __ARM_NEON
    int16x4_t v1 = {1, 2, 3, 32000};
    int16x4_t v2 = {0, 2, 4, 6};
    int32x4_t v_out = vqdmull_lane_s16(v1, v2, lane);
#else
    pxl::int16x4_t pv1 = {1, 2, 3, 32000};
    pxl::int16x4_t pv2 = {0, 2, 4, 6};
    pxl::int32x4_t pv_out = pxl::vqdmull_lane_s16(pv1, pv2, lane);
#endif

    int32_t out[4];
    vst1q_s32(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqdmulh)
{
    int16x4_t v1 = {1, 2, 3, 4};
    int16x4_t v2 = {10000, 15000, 20000, 25000};
    int16x4_t v_out = vqdmulh_s16(v1, v2);
    int16_t expected_out[4] = {0, 0, 1, 3};

    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv2 = {10000, 15000, 20000, 25000};
    pxl::int16x4_t pv_out = pxl::vqdmulh_s16(pv1, pv2);

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d %d], ", out[i], pv_out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqdmulh_n)
{
    int16x4_t v1 = {1, 2, 3, 4};
    int16_t scale = 30000;
    int16x4_t v_out = vqdmulh_n_s16(v1, scale);
    int16_t expected_out[4] = {0, 1, 2, 3};

    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv_out = pxl::vqdmulh_n_s16(pv1, scale);

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d %d], ", out[i], pv_out[i]);
    }
    //fprintf(stderr, "\n");
}


TEST(mul, vqdmulh_lane)
{
    const int lane = 1;
#if __ARM_NEON
    int16x4_t v1 = {1, 2, 3, 4};
    int16x4_t v2 = {10000, 15000, 20000, 25000};
    int16x4_t v_out = vqdmulh_lane_s16(v1, v2, lane);
#else
    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv2 = {10000, 15000, 20000, 25000};
    pxl::int16x4_t pv_out = pxl::vqdmulh_lane_s16(pv1, pv2, lane);
#endif
    int16_t expected_out[4] = {0, 0, 1, 1};

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqrdmulh)
{
    int16x4_t v1 = {1, 2, 3, 4};
    int16x4_t v2 = {10000, 15000, 20000, 25000};
    int16x4_t v_out = vqrdmulh_s16(v1, v2);
    int16_t expected_out[4] = {0, 1, 2, 3};

    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv2 = {10000, 15000, 20000, 25000};
    pxl::int16x4_t pv_out = pxl::vqrdmulh_s16(pv1, pv2);

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d %d], ", out[i], pv_out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqrdmulh_n)
{
    int16x4_t v1 = {1, 2, 3, 4};
    int16_t scale = 20000;
    int16x4_t v_out = vqrdmulh_n_s16(v1, scale);
    int16_t expected_out[4] = {1, 1, 2, 2};

    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv_out = pxl::vqrdmulh_n_s16(pv1, scale);

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d %d], ", out[i], pv_out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vqrdmulh_lane)
{
    const int lane = 2;
#if __ARM_NEON
    int16x4_t v1 = {1, 2, 3, 4};
    int16x4_t v2 = {10000, 15000, 20000, 25000};
    int16x4_t v_out = vqrdmulh_lane_s16(v1, v2, lane);
#else
    pxl::int16x4_t v1 = {1, 2, 3, 4};
    pxl::int16x4_t v2 = {10000, 15000, 20000, 25000};
    pxl::int16x4_t v_out = pxl::vqrdmulh_lane_s16(v1, v2, lane);
#endif
    int16_t expected_out[4] = {1, 1, 2, 2};

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

TEST(mul, vmla)
{
    int16x4_t v1 = {1, 2, 3, 4};
    int16x4_t v2 = {2, 2, 2, 2};
    int16x4_t v3 = {3, 3, 3, 3};
    int16x4_t v_out = vmla_s16(v1, v2, v3);
    int16_t expected_out[4] = {5, 7, 9, 11};

    pxl::int16x4_t pv1 = {1, 2, 3, 4};
    pxl::int16x4_t pv2 = {2, 2, 2, 2};
    pxl::int16x4_t pv3 = {3, 3, 3, 3};
    pxl::int16x4_t pv_out = pxl::vmla_s16(pv1, pv2, pv3);

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(pv_out[i], out[i]);
        //fprintf(stderr, "[%d %d], ", out[i], pv_out[i]);
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
