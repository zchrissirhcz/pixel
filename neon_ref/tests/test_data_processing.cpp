#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>



// r = vpadd(a, b)
TEST(add, vpadd)
{
    // vpadd_type: 两个输入向量拼接后，拼接向量里每两个相邻元素作为pair，每个pair相加作为结果向量的元素
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18};
    int8x8_t v_out = vpadd_s8(v1, v2);
    int8_t expected_out[8] = {3, 7, 11, 15, 23, 27, 31, 35};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        //fprintf(stderr, "%d, ", out[i]);
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

// r = vadd(a)
TEST(add, vpaddl)
{
    // vadd_type: 输入向量a的每两个相邻（不重叠）元素作为pair，每个pair相加作为结果元素，并且类型拓宽 。
    uint8x8_t v1 = {255, 1, 255, 2, 255, 3, 255, 4};
    uint16x4_t v_out = vpaddl_u8(v1);
    uint16_t expected_out[4] = {256, 257, 258, 259};

    uint16_t out[4];
    vst1_u16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(add, vpadal)
{
    // vpadal: Signed Add and Accumulate Long Pairwise
    // r0 = a0 + (b0 + b1), ..., r3 = a3 + (b6 + b7);
    uint16x4_t v1 = {100, 200, 300, 400};
    uint8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    uint16x4_t v_out = vpadal_u8(v1, v2);
    uint16_t expected_out[4] = {103, 207, 311, 415};
    
    uint16_t out[4];
    vst1_u16(out, v_out);
    for (int i=0; i<4; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(add, vabs)
{
    int8x8_t v1 = {-1, 2, -3, 4, -5, 6, -7, 8};
    int8x8_t v_out = vabs_s8(v1);
    int8_t expected_out[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    
    pxl::int8x8_t pv1 = {-1, 2, -3, 4, -5, 6, -7, 8};
    pxl::int8x8_t pv_out = pxl::vabs_s8(pv1);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}


TEST(add, vqabs)
{
    int8x8_t v1 = {-128, 2, -3, 4, -5, 6, -7, 8};
    int8x8_t v_out = vqabs_s8(v1);
    int8_t expected_out[8] = {127, 2, 3, 4, 5, 6, 7, 8};
    
    pxl::int8x8_t pv1 = {-128, 2, -3, 4, -5, 6, -7, 8};
    pxl::int8x8_t pv_out = pxl::vqabs_s8(pv1);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(add, vabd)
{
    {
        int8x8_t v1 = {-1, 2, -3, 4, -5, 6, -7, 8};
        int8x8_t v2 = {3,7 ,4, 9, 1, 2, 6, 8};
        int8x8_t v_out = vabd_s8(v1, v2);
        int8_t expected_out[8] = {4, 5, 7, 5, 6, 4, 13, 0};
        
        pxl::int8x8_t pv1 = {-1, 2, -3, 4, -5, 6, -7, 8};
        pxl::int8x8_t pv2 = {3,7 ,4, 9, 1, 2, 6, 8};
        pxl::int8x8_t pv_out = pxl::vabd_s8(pv1, pv2);

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "%d, ", pv_out[i]);
        }
    }

    {
        uint32x4_t v1 = {100, 4294967295, 233, 456};
        uint32x4_t v2 = {4294967295, 100, 456, 233};
        uint32x4_t v_out = vabdq_u32(v1, v2);
        uint32_t expected_out[4] = {4294967195, 4294967195, 223, 223};
        
        pxl::uint32x4_t pv1 = {100, 4294967295, 233, 456};
        pxl::uint32x4_t pv2 = {4294967295, 100, 456, 233};
        pxl::uint32x4_t pv_out = pxl::vabdq_u32(pv1, pv2);
        
        uint32_t out[4];
        vst1q_u32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
        }
    }
}

TEST(add, vabdl)
{
    {
        int8x8_t v1 = {-2, -120, 1, 2, 3, 4, 5, 6};
        int8x8_t v2 = {127, 100, 9, -2, 5, -6, 7, 3};
        int16x8_t v_out = vabdl_s8(v1, v2);
        int16_t expected_out[8] = {129, 220, 8, 4, 2, 10, 2, 3};
        
        pxl::int8x8_t pv1 = {-2, -120, 1, 2, 3, 4, 5, 6};
        pxl::int8x8_t pv2 = {127, 100, 9, -2, 5, -6, 7, 3};
        pxl::int16x8_t pv_out = pxl::vabdl_s8(pv1, pv2);

        int16_t out[8];
        vst1q_s16(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
        }
    }

    {
        uint8x8_t v1 = {2,  120, 1, 2, 3, 4, 5, 6};
        uint8x8_t v2 = {127, 100, 9, 2, 5, 6, 7, 3};
        uint16x8_t v_out = vabdl_u8(v1, v2);
        uint16_t expected_out[8] = {125, 20, 8, 0, 2, 2, 2, 3};
        
        pxl::uint8x8_t pv1 = {2,  120, 1, 2, 3, 4, 5, 6};
        pxl::uint8x8_t pv2 = {127, 100, 9, 2, 5, 6, 7, 3};
        pxl::uint16x8_t pv_out = pxl::vabdl_u8(pv1, pv2);

        uint16_t out[8];
        vst1q_u16(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
        }
    }
}

TEST(add, vaba)
{
    {
        int8x8_t v1 = {127, 0, 1, 2, 3, 4, 5, 6};
        int8x8_t v2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        int8x8_t v3 = {3,7 ,4, 9, 1, 2, 6, 8};
        int8x8_t v_out = vaba_s8(v1, v2, v3);
        int8_t expected_out[8] = {-125, 5, 8, 7, 9, 8, 18, 6};
        
        pxl::int8x8_t pv1 = {127, 0, 1, 2, 3, 4, 5, 6};
        pxl::int8x8_t pv2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        pxl::int8x8_t pv3 = {3,7 ,4, 9, 1, 2, 6, 8};
        pxl::int8x8_t pv_out = pxl::vaba_s8(pv1, pv2, pv3);

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
            //fprintf(stderr, "%d, ", pv_out[i]);
        }
    }
}

TEST(add, vabal)
{
    {
        int16x8_t v1 = {127, 0, 1, 2, 3, 4, 5, 6};
        int8x8_t v2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        int8x8_t v3 = {3,7 ,4, 9, 1, 2, 6, 8};
        int16x8_t v_out = vabal_s8(v1, v2, v3);
        int16_t expected_out[8] = {131, 5, 8, 7, 9, 8, 18, 6};
        
        pxl::int16x8_t pv1 = {127, 0, 1, 2, 3, 4, 5, 6};
        pxl::int8x8_t pv2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        pxl::int8x8_t pv3 = {3,7 ,4, 9, 1, 2, 6, 8};
        pxl::int16x8_t pv_out = pxl::vabal_s8(pv1, pv2, pv3);

        int16_t out[8];
        vst1q_s16(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
            //fprintf(stderr, "%d, ", out[i]);
        }
    }
}


TEST(add, vmax)
{
    {
        int8x8_t v1 = {127, 0, 1, 2, 3, 4, 5, 6};
        int8x8_t v2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        int8x8_t v_out = vmax_s8(v1, v2);
        int8_t expected_out[8] = {127, 2, 1, 4, 3, 6, 5, 8};
        
        pxl::int8x8_t pv1 = {127, 0, 1, 2, 3, 4, 5, 6};
        pxl::int8x8_t pv2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        pxl::int8x8_t pv_out = pxl::vmax_s8(pv1, pv2);

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
            //fprintf(stderr, "%d, ", out[i]);
        }
    }
}

TEST(add, vpmax)
{
    {
        int8x8_t v1 = {127, 0, 1, 2, 3, 4, 5, 6};
        int8x8_t v2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        int8x8_t v_out = vpmax_s8(v1, v2);
        int8_t expected_out[8] = {127, 2, 4, 6, 2, 4, 6, 8};
        
        pxl::int8x8_t pv1 = {127, 0, 1, 2, 3, 4, 5, 6};
        pxl::int8x8_t pv2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        pxl::int8x8_t pv_out = pxl::vpmax_s8(pv1, pv2);

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
            //fprintf(stderr, "%d, ", out[i]);
        }
    }
}


TEST(add, vmin)
{
    {
        int8x8_t v1 = {127, 0, 1, 2, 3, 4, 5, 6};
        int8x8_t v2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        int8x8_t v_out = vmin_s8(v1, v2);
        int8_t expected_out[8] = {-1, 0, -3, 2, -5, 4, -7, 6};
        
        pxl::int8x8_t pv1 = {127, 0, 1, 2, 3, 4, 5, 6};
        pxl::int8x8_t pv2 = {-1, 2, -3, 4, -5, 6, -7, 8};
        pxl::int8x8_t pv_out = pxl::vmin_s8(pv1, pv2);

        int8_t out[8];
        vst1_s8(out, v_out);
        for (int i=0; i<8; i++)
        {
            ASSERT_EQ(expected_out[i], out[i]);
            ASSERT_EQ(pv_out[i], out[i]);
            //fprintf(stderr, "[%u, %u], ", pv_out[i], out[i]);
            //fprintf(stderr, "%d, ", out[i]);
        }
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
