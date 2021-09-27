#include "gtest/gtest.h"

#include <arm_neon.h>

#include "pixel_neon.hpp"
#include "vadd.hpp"
#include "vaddl.hpp"
#include "vaddw.hpp"
#include "vdup_n.hpp"

// vadd_type
// vaddq_type
TEST(add, vadd_s8)
{
    // 对应位元素相加: ri = ai + bi
    // 对于 signed 类型， 例如s8, 超过127后， 要减去256 从而得到正确结果
    int8_t input1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {10, 20, 30, 40, 50, 60, 70, 127};
    int8x8_t v_out = vadd_s8(v1, v2);
    int8_t expected_out[8] = {11, 22, 33, 44, 55, 66, 77, -121};

    pxl::int8x8_t pv1 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int8x8_t pv2 = {10, 20, 30, 40, 50, 60, 70, 127};
    pxl::int8x8_t pv_out = pxl::vadd_s8(pv1, pv2);

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(add, vaddl_u8)
{
    // vaddl_type: 变长加法运算， 为了防止溢出
    uint8x8_t v1 = {200, 200, 200, 200, 200, 200, 200, 200};
    uint8x8_t v2 = { 50,  51,  52,  53,  54,  55,  56,  57};
    uint16x8_t v_out = vaddl_u8(v1, v2);
    uint16_t expected_out[8] = {250, 251, 252, 253, 254, 255, 256, 257};

    pxl::uint8x8_t pv1 = {200, 200, 200, 200, 200, 200, 200, 200};
    pxl::uint8x8_t pv2 = { 50,  51,  52,  53,  54,  55,  56,  57};
    pxl::uint16x8_t pv_out = pxl::vaddl_u8(pv1, pv2);

    uint16_t out[8];
    vst1q_u16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

TEST(add, vaddw_s8)
{
    //vaddw_type: 第一个vector元素宽度2倍于第二个vector元素；结果元素也是2倍宽度于第二个vector元素
    //WT v_out = vaddw_type(WT v1, T v2)
    int16x8_t v1 = vdupq_n_s16(300);
    int8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    int16x8_t v_out = vaddw_s8(v1, v2);
    int16_t expected_out[8] = {301, 302, 303, 304, 305, 306, 307, 308};

    pxl::int16x8_t pv1 = pxl::vdupq_n_s16(300);
    pxl::int8x8_t pv2 = {1, 2, 3, 4, 5, 6, 7, 8};
    pxl::int16x8_t pv_out = pxl::vaddw_s8(pv1, pv2);

    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        ASSERT_EQ(pv_out[i], out[i]);
    }
}

// Reference implementation:
// int8x8_t vaddhn_u16(int16x8_t v1, int16x8_t v2)
// {
//     int8x8_t res;
//     for (int i=0; i<8; i++)
//     {
//         temp[i] = (v1[i] + v2[i])>>8;
//     }
//     return res;
// }

// vaddhn_s16
// vaddhn_s32
// vaddhn_s64
// vaddhn_u16
// vaddhn_u32
// vaddhn_u64
TEST(add, vaddhn_u16)
{
    //vaddhn_type: 结果vector元素的类型大小是输入vector元素的一半
    // 具体到 vaddhn_u16, 输入的两个参数v1, v2是u16类型的
    // 先算出u16类型的 v1+v2的结果 v_mid
    // 然后砍掉 v_mid 的低8位， 也就是高八位右移，得到最终结果
    uint16x8_t v1 = vdupq_n_u16(250);
    //uint16x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    uint16x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 264};
    //251:  0 ........
    //252:  0 ........
    //253:  0 ........
    //254:  0 ........
    //255:  0 11111111
    //256:  1 00000000
    //257:  1 00000001
    //258:  1 00000010
    //514: 10 00000010  514 = 250 + 264
    uint8x8_t v_out = vaddhn_u16(v1, v2);
    uint8_t expected_out[8] = {0, 0, 0, 0, 0, 1, 1, 2};

    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
    }
    //fprintf(stderr, "\n");
}

//saturating add
TEST(add, vqadd)
{
    // vqadd_type: ri = sat(ai + bi) 饱和指令，相加结果超出元素的最大值时，元素就取最大值。
    uint8x8_t v1 = vdup_n_u8(250);
    uint8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8x8_t v_out = vqadd_u8(v1, v2);
    uint8_t expected_out[8] = {251, 252, 253, 254, 255, 255, 255, 255};

    uint8_t out[8];
    vst1_u8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(add, vhadd)
{
    // 相加结果再除2。ri = (ai + bi) >> 1;
    // 不能整除2的话， 如果是正数结果， 则向下取整(3.5 => 3)
    //                 如果是负数结果， 则也是向下取整(-3.5 => -4)
    int8x8_t v1 = {1, 2, 3, 4, 5,  6,  7,   8};
    int8x8_t v2 = {1, 3, 5, 7, 9, 11, 13, -15};
    int8x8_t v_out = vhadd_s8(v1, v2);
    int8_t expected_out[8] = {1, 2, 4, 5, 7, 8, 10, -4};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(add, vrhadd)
{
    // vrhadd_type: 相加结果再除2(四舍五入)。ri = (ai + bi + 1) >> 1
    // 结果也是向下取整。
    int8x8_t v1 = {1, 2, 3, 4, 5,  6,  7,   1};
    int8x8_t v2 = {1, 3, 5, 7, 9, 11, 13, -15};
    int8x8_t v_out = vrhadd_s8(v1, v2);
    int8_t expected_out[8] = {1, 3, 4, 6, 7, 9, 10, -7};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

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
