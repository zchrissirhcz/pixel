#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

// vext_type
// vextq_type
TEST(register_data_rearrange, vext)
{
    int16x8_t v1 = {1,  2,  3,  4,  5,  6,  7,  8};
    int16x8_t v2 = {9, 10, 11, 12, 13, 14, 15, 16};
    // 取 v2 的低3个元素(9, 10, 11)， 放到新 vector 的高3个元素位置
    // 新 vector 的剩余元素取自 v1 的最高几个元素
    // 当 v1 等于 v2， 可以用来实现 vector 内元素位置的移动
    int16x8_t v_out = vextq_s16(v1, v2, 3);
    int16_t expected_out[8] = {4,5,6,7,8, 9,10,11};
    
    int16_t out[8];
    vst1q_s16(out, v_out);
    for (int i=0; i<8; i++)
    {
        EXPECT_TRUE(expected_out[i] == out[i]);
    }
}

// Table vector Lookup
// vtbl1_s8
// vtbl1_u8
TEST(register_data_rearrange, vtbl1)
{
    // vtb1_type: 第二个 vector 是索引， 根据索引去第一个 vector （相当于数组） 中搜索相应的元素
    // 并输出新的 vector， 超过范围的索引返回的是 0.
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {0, 0, 1, 1, 2, 2, 7, 8};
    int8x8_t v_out = vtbl1_s8(v1, v2);
    int8_t expected_out[8] = {1, 1, 2, 2, 3, 3, 8, 0};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        EXPECT_TRUE(expected_out[i] == out[i]);
    }
}

TEST(register_data_rearrange, vtb2)
{
    // vtbl2_type: 数组长度扩大到2个 vector
    // 仍然是用第二个数组作为索引， 从第一个数组中取元素； 如果索引超过范围则返回0元素
    int8x8x2_t v_src = {
        1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
    };
    int8x8_t v_src2 = {0, 0, 1, 1, 2, 8, 10, 16};
    int8x8_t v_out = vtbl2_s8(v_src, v_src2);
    int8_t expected_out[8] = {1, 1, 2, 2, 3, 9, 11, 0};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        EXPECT_TRUE(expected_out[i] == out[i]);
    }
}

// Table vector lookup extension
TEST(register_data_rearrange, vtbx1)
{
    // v3 的每个元素表示的是索引
    // 根据 v3 的每个元素， 如果是在索引范围内（[0, v2的长度-1]）， 则取v2的对应位置元素
    // 如果 v3 的当前元素超过 v2 的索引范围， 则取 v1 的对应位置元素
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18};
    int8x8_t v3 = {0, 0, 1, 1, 2, 2, 30, 9}; // indices
    int8x8_t v_out = vtbx1_s8(v1, v2, v3);
    int8_t expected_out[8] = {11, 11, 12, 12, 13, 13, 7, 8};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        //fprintf(stderr, "i=%d\n", i);
        //fprintf(stderr, "%d, ", out[i]);
        ASSERT_EQ(expected_out[i], out[i]);
    }
    fprintf(stderr, "\n");
}

TEST(register_data_rearrange, vrev16)
{
    // vrevX_s8 是以 X bit 为单位长度，每个单位长度内的顺序翻转，得到结果向量

    // vrev16_s8 是以 16bit 为单位长度， 每相邻两个 byte 互换位置
    int8x8_t v1 = {1,2,3,4,5,6,7,8};
    int8x8_t v_out = vrev16_s8(v1);
    int8_t expected_out[8] = {2,1,4,3,6,5,8,7};

    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(register_data_rearrange, vrev32)
{
    // vrev32_s8 是以 32bit 为单位长度， 每相邻4个 byte， 位置翻转， 得到结果向量
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v_out = vrev32_s8(v1);
    int8_t expected_out[8] = {4, 3, 2, 1, 8, 7, 6, 5};
    
    int8_t out[8];
    vst1_s8(out, v_out);
    for(int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(register_data_rearrange, vrev64)
{
    int8x8_t v1 = {1, 2, 3, 4, 5, 6, 7, 8};
    int8x8_t v_out = vrev64_s8(v1);
    int8_t expected_out[8] = {8, 7, 6, 5, 4, 3, 2, 1};

    int8_t out[8];
    vst1_s8(out, v_out);
    for(int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

// transpose
TEST(register_data_rearrange, vtrn)
{
    // vtrn_type: 将两个输入 vector 的元素通过转置生成一个有两个 vector 的矩阵
    int8x8_t v1 = {1,  2,  3,  4,  5,  6,  7,  8};
    int8x8_t v2 = {9, 10, 11, 12, 13, 14, 15, 16};
    int8x8x2_t v_out = vtrn_s8(v1, v2);
    
    int8_t expected_out[2][8] = {
        {1,  9, 3, 11, 5, 13, 7, 15},
        {2, 10, 4, 12, 6, 14, 8, 16}
    };
    
    int8_t out[2][8];
    vst1_s8(out[0], v_out.val[0]);
    vst1_s8(out[1], v_out.val[1]);
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<8; j++)
        {
            ASSERT_EQ(expected_out[i][j], out[i][j]);
        }
    }
}

TEST(register_data_rearrange, vzip)
{
    // vzip_type: 将两个输入 vector 的元素通过交叉生成一个有两个 vector 的矩阵
    int8x8_t v1 = { 1,  2,  3,  4,  5,  6,  7,  8};
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18};
    int8x8x2_t v_out = vzip_s8(v1, v2);
    int8_t expected_out[2][8] = {
        {1, 11, 2, 12, 3, 13, 4, 14},
        {5, 15, 6, 16, 7, 17, 8, 18}
    };

    int8_t out[2][8];
    vst1_s8(out[0], v_out.val[0]);
    vst1_s8(out[1], v_out.val[1]);
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<8; j++)
        {
            ASSERT_EQ(expected_out[i][j], out[i][j]);
        }
    }
}

TEST(register_data_rearrange, vuzp)
{
    // 将两个输入 vector 的元素通过反交叉生成一个有两个 vector 的矩阵
    // (通过这个可以实现 n-way 交织)
    int8x8_t v1 = { 1,  2,  3,  4,  5,  6,  7,  8};
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18};
    int8x8x2_t v_out = vuzp_s8(v1, v2);
    
    int8_t expected_out[2][8] = {
        {1, 3, 5, 7, 11, 13, 15, 17},
        {2, 4, 6, 8, 12, 14, 16, 18}
    };

    int8_t out[2][8];
    vst1_s8(out[0], v_out.val[0]);
    vst1_s8(out[1], v_out.val[1]);

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<8; j++)
        {
            ASSERT_EQ(expected_out[i][j], out[i][j]);
        }
    }
}

TEST(register_data_rearrange, vcombine)
{
    int8x8_t v1 = { 1,  2,  3,  4,  5,  6,  7,  8};
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18};
    int8x16_t v_out = vcombine_s8(v1, v2);
    int8_t expected_out[16] = {1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17, 18};

    int8_t out[16];
    vst1q_s8(out, v_out);
    for (int i=0; i<16; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
    }
}

TEST(register_data_rearrange, vbsl)
{
    // vbsl_type: 按位选择，参数为(mask, src1, src2).
    // mask 的某个 bit 为 1， 则选择 src1 中对应的 bit; 为 0， 则选择 src2 中对应的 bit
    // **注意** 是按 bit 取的， 而不是按照 一个byte 为1或0而取的
    // 也就是说， mask里的一个元素，是有若干bits的，根据这些bits，是1则从src1取bit，是0则从src2取bit
    // 也就是说很有可能分别从src1和src2的不同bit位取bit才得到结果，例如下面例子中最后一个maks元素
    int8x8_t v1 = { 1,  2,  3,  4,  5,  6,  7,  8}; // 8的二进制: 00001000
    int8x8_t v2 = {11, 12, 13, 14, 15, 16, 17, 18}; //18的二进制: 00010010
    uint8x8_t v_mask = {0xFF, 0, 0xFF, 0, 0, 0, 0xFF, 0x0F}; //   00001111  => 00011000 => 24
    int8x8_t v_out = vbsl_s8(v_mask, v1, v2);
    
    int8_t expected_out[8] = {1, 12, 3, 14, 15, 16, 7, 24};
    int8_t out[8];
    vst1_s8(out, v_out);
    for (int i=0; i<8; i++)
    {
        ASSERT_EQ(expected_out[i], out[i]);
        //fprintf(stderr, "%d, ", out[i]);
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