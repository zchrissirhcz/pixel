#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

#include <stdio.h>
#include <cmath>

/// compare vshrn_n_u16 and vrshrn_n_u16 result
TEST(shift_right, round_or_not)
{
    unsigned short input = 65280 + 128; //65408
    uint16x8_t v_input = vdupq_n_u16(input);
    // 65408/256.0 = 255.5
    uint8x8_t v_out1 = vshrn_n_u16(v_input, 8);
    uint8x8_t v_out2 = vrshrn_n_u16(v_input, 8);
    uint8_t out1[8];
    uint8_t out2[8];
    vst1_u8(out1, v_out1);
    vst1_u8(out2, v_out2);

    // fprintf(stderr, "out1[0]=%d\n", out1[0]); // 255
    // fprintf(stderr, "out2[0]=%d\n", out2[0]); // 0
    EXPECT_TRUE(out1[0] == 255);
    EXPECT_TRUE(out2[0] == 0); // round版本的右移结果等于0，引发显著计算误差

    // unsigned char output = input >> 8;
    // fprintf(stderr, "output = %d\n", output); // 255

    // unsigned char dump = std::round(255.5);
    // fprintf(stderr, "dump = %d\n", output); // 255
}

TEST(shift_right, round_s8)
{
    //--------------------------------------------
    // neon 的右移位 rounding 策略：假设要右移 n 位置
    // 则 (vi + n/2) 后，再向右移 n 位。
    // 不要和 std::round 系列混淆掉。请忘记 std::round().
    //-------------------------------------------
    // 对于正数vi， rounding 结果： vrshr_n_type(v1, n)=(v1 + (1<<n)) >> n
    // 对于负数vi， rounding 结果： vrshr_n_type(v1, n)=(v1 + (1<<n)) >> n
    // 这里的rounding策略和C++标准库的lround()等函数，对于负数的处理则不一样

    int16x4_t v1 = {
        64,  // 00000000 01000000      00000000 0
        -64, // 11111111 11000000      11111111 1
        63,  // 00000000 00111111
        -63  // 11111111 11000001
    };
    int16x4_t v_out = vrshr_n_s16(v1, 7);
    //6: 1, -1, 0, 0, 
    //7: 1,  0, 0, 0
    int16_t expected_out[4] = {1, 0, 0, 0};
    
    // 64                               -64
    // orig:  00000000 01000000 (64)    11111111 11000000 (-64)
    // r>>1:  00000000 00100000 (32)    11111111 11100000 (-32)
    // r>>2:  00000000 00010000 (16)    11111111 11110000 (-16)
    // r>>3:  00000000 00001000 (8)     11111111 11111000 (-8)
    // r>>4:  00000000 00000100 (4)     11111111 11111100 (-4)
    // r>>5:  00000000 00000010 (2)     11111111 11111110 (-2)
    // r>>6:  00000000 00000001 (1)     11111111 11111111 (-1)
    // r>>7:  00000000 00000001 (1)

    //          63     -63     64     -64
    // r>>1     32     -31     32     -32
    // r>>2    -16     -16     16     -16
    // r>>3     8      -8       8      -8
    // r>>4     4      -4       4      -4
    // r>>5     2      -2       2      -2
    // r>>6     1      -1       1      -1
    // r>>7     0       0       1       0

    int16_t out[4];
    vst1_s16(out, v_out);
    for (int i=0; i<4; i++)
    {
        //fprintf(stderr, "%d, ", out[i]);
        ASSERT_EQ(expected_out[i], out[i]);
    }
    // fprintf(stderr, "\n");

    // fprintf(stderr, "std::round(1.23)=%f\n", std::round(1.23));    // 1.0
    // fprintf(stderr, "std::round(-1.23)=%f\n", std::round(-1.23));  // -1.0
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
