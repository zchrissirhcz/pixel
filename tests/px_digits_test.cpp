#include "gtest/gtest.h"
#include <limits>
#include <type_traits>
#include "px_digits.h"

TEST(digits, 2s_complement)
{
    {
        std::vector<int> bits = {1, 1, 0, 0, 0, 1, 1, 1};
        std::vector<int> actual(8, 0);
        px_get_2s_complement(bits.data(), 8, actual.data());

        std::vector<int> expected = {0, 0, 1, 1, 1, 0, 0, 1};
        EXPECT_EQ(expected, actual);
    }

    {
        std::vector<int> bits = {0, 0, 1, 1, 1, 0, 0, 1};
        std::vector<int> actual(8, 0);
        px_get_2s_complement(bits.data(), 8, actual.data());

        std::vector<int> expected = {1, 1, 0, 0, 0, 1, 1, 1};
        EXPECT_EQ(expected, actual);
    }

    {
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 0};
        std::vector<int> actual(8, 0);
        px_get_2s_complement(bits.data(), 8, actual.data());

        std::vector<int> expected = {1, 0, 0, 0, 0, 0, 0, 0};
        EXPECT_EQ(expected, actual);
    }
}

template<int len>
void test_decimal_and_binary_signed(int n, std::vector<int>& bits)
{
    {
        std::vector<int> actual(len, -1);
        px_decimal_to_binary_signed(n, actual.data(), len);
        EXPECT_EQ(bits, actual);
    }

    {
        int actual = px_binary_to_decimal_signed(bits.data(), len);
        EXPECT_EQ(n, actual);
    }
}

template<int len>
void test_decimal_and_binary_unsigned(unsigned n, std::vector<int>& bits)
{
    {
        std::vector<int> actual(len, -1);
        px_decimal_to_binary_unsigned(n, actual.data(), len);
        EXPECT_EQ(bits, actual);
    }

    {
        unsigned actual = px_binary_to_decimal_unsigned(bits.data(), len);
        EXPECT_EQ(n, actual);
    }
}

static void test_len_u8(int n, std::vector<int>& bits)
{
    return test_decimal_and_binary_unsigned<8>(n, bits);
}

static void test_len_s16(int n, std::vector<int>& bits)
{
    return test_decimal_and_binary_signed<16>(n, bits);
}

static void test_len_u16(int n, std::vector<int>& bits)
{
    return test_decimal_and_binary_unsigned<16>(n, bits);
}

static void test_len_s5(int n, std::vector<int>& bits)
{
    return test_decimal_and_binary_signed<5>(n, bits);
}

static void test_len_s8(int n, std::vector<int>& bits)
{
    return test_decimal_and_binary_signed<8>(n, bits);
}

TEST(digits, u8)
{
    {
        unsigned n = 0;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 0};
        test_len_u8(n, bits);
    }

    {
        unsigned n = 128;
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 0};
        test_len_u8(n, bits);
    }

    {
        unsigned n = 57;
        std::vector<int> bits = {0, 0, 1, 1, 1, 0, 0, 1};
        test_len_u8(n, bits);
    }

    {
        unsigned n = 71;
        std::vector<int> bits = {0, 1, 0, 0, 0, 1, 1, 1};
        test_len_u8(n, bits);
    }

    {
        unsigned n = 255;
        std::vector<int> bits = {1, 1, 1, 1, 1, 1, 1, 1};
        test_len_u8(n, bits);
    }
}

TEST(digits, s16)
{
    {
        int n = 0;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        test_len_s16(n, bits);
    }

    {
        int n = -1;
        std::vector<int> bits = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        test_len_s16(n, bits);
    }

    {
        int n = -32768;
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        test_len_s16(n, bits);
    }

    {
        int n = -32767;
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
        test_len_s16(n, bits);
    }

    {
        int n = 32767;
        std::vector<int> bits = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        test_len_s16(n, bits);
    }

    {
        int n = -57;
        std::vector<int> bits = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};
        test_len_s16(n, bits);
    }

    {
        int n = -257;
        std::vector<int> bits = {1, 1, 1, 1,  1, 1, 1, 0,  1, 1, 1, 1,  1, 1, 1, 1};
        test_len_s16(n, bits);
    }

    {
        int n = 257;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1};
        test_len_s16(n, bits);
    }
}

TEST(digits, u16)
{
    {
        unsigned n = 0;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        test_len_u16(n, bits);
    }

    {
        unsigned n = 65535;
        std::vector<int> bits = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        test_len_u16(n, bits);
    }

    {
        unsigned n = 32768;
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        test_len_u16(n, bits);
    }

    {
        unsigned n = 32767;
        std::vector<int> bits = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        test_len_u16(n, bits);
    }

    {
        unsigned n = 57;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1};
        test_len_u16(n, bits);
    }

    {
        unsigned n = 257;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1};
        test_len_u16(n, bits);
    }
}

TEST(digits, s5)
{
    const int len = 5;
    {
        int n = 0;
        std::vector<int> bits = {0, 0, 0, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 1;
        std::vector<int> bits = {0, 0, 0, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 2;
        std::vector<int> bits = {0, 0, 0, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 3;
        std::vector<int> bits = {0, 0, 0, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 4;
        std::vector<int> bits = {0, 0, 1, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 5;
        std::vector<int> bits = {0, 0, 1, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 6;
        std::vector<int> bits = {0, 0, 1, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 7;
        std::vector<int> bits = {0, 0, 1, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 8;
        std::vector<int> bits = {0, 1, 0, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 9;
        std::vector<int> bits = {0, 1, 0, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 10;
        std::vector<int> bits = {0, 1, 0, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 11;
        std::vector<int> bits = {0, 1, 0, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 12;
        std::vector<int> bits = {0, 1, 1, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 13;
        std::vector<int> bits = {0, 1, 1, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = 14;
        std::vector<int> bits = {0, 1, 1, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = 15;
        std::vector<int> bits = {0, 1, 1, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -16;
        std::vector<int> bits = {1, 0, 0, 0, 0};
        test_len_s5(n, bits);
    }


    {
        int n = -15;
        std::vector<int> bits = {1, 0, 0, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -14;
        std::vector<int> bits = {1, 0, 0, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -13;
        std::vector<int> bits = {1, 0, 0, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -12;
        std::vector<int> bits = {1, 0, 1, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -11;
        std::vector<int> bits = {1, 0, 1, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -10;
        std::vector<int> bits = {1, 0, 1, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -9;
        std::vector<int> bits = {1, 0, 1, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -8;
        std::vector<int> bits = {1, 1, 0, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -7;
        std::vector<int> bits = {1, 1, 0, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -6;
        std::vector<int> bits = {1, 1, 0, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -5;
        std::vector<int> bits = {1, 1, 0, 1, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -4;
        std::vector<int> bits = {1, 1, 1, 0, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -3;
        std::vector<int> bits = {1, 1, 1, 0, 1};
        test_len_s5(n, bits);
    }

    {
        int n = -2;
        std::vector<int> bits = {1, 1, 1, 1, 0};
        test_len_s5(n, bits);
    }

    {
        int n = -1;
        std::vector<int> bits = {1, 1, 1, 1, 1};
        test_len_s5(n, bits);
    }
}


TEST(digits, s8)
{
    const int len = 8;
    {
        int n = 0;
        std::vector<int> bits = {0, 0, 0, 0, 0, 0, 0, 0};
        test_len_s8(n, bits);
    }

    {
        int n = 126;
        std::vector<int> bits = {0, 1, 1, 1, 1, 1, 1, 0};
        test_len_s8(n, bits);
    }

    {
        int n = 127;
        std::vector<int> bits = {0, 1, 1, 1, 1, 1, 1, 1};
        test_len_s8(n, bits);
    }

    {
        int n = -128;
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 0};
        test_len_s8(n, bits);
    }

    {
        int n = -127;
        std::vector<int> bits = {1, 0, 0, 0, 0, 0, 0, 1};
        test_len_s8(n, bits);
    }

    {
        int n = -1;
        std::vector<int> bits = {1, 1, 1, 1, 1, 1, 1, 1};
        test_len_s8(n, bits);
    }

    {
        int n = -57;
        std::vector<int> bits = {1, 1, 0, 0, 0, 1, 1, 1};
        test_len_s8(n, bits);
    }

    {
        int n = 105;
        std::vector<int> bits = {0, 1, 1, 0, 1, 0, 0, 1};
        test_len_s8(n, bits);
    }
}


TEST(digits, binary_add)
{
    int len = 5;
    {
        std::vector<int> bits1 = {0, 1, 0, 1, 1}; // 11
        std::vector<int> bits2 = {0, 0, 0, 1, 1}; // 3
        std::vector<int> bits_result(len, -1);
        px_binary_add(bits1.data(), bits2.data(), bits_result.data(), len);
        std::vector<int> expected = {0, 1, 1, 1, 0}; // 14
        EXPECT_EQ(expected, bits_result);
    }

    {
        std::vector<int> bits1 = {0, 1, 1, 1, 0}; // 14
        std::vector<int> bits2 = {1, 0, 1, 1, 1}; // -9
        std::vector<int> bits_result(len, -1);
        px_binary_add(bits1.data(), bits2.data(), bits_result.data(), len);
        std::vector<int> expected = {0, 0, 1, 0, 1}; // 5
        EXPECT_EQ(expected, bits_result);
    }
}

void test_f32(float N, std::vector<int>& bits)
{
    // decimal to binary
    {
        std::vector<int> actual(32, -1);
        px_decimal_to_binary_f32(N, actual.data());
        EXPECT_EQ(bits, actual);
    }

    // binary to decmimal
    {
        float res = px_binary_to_decimal_f32(bits.data());
        EXPECT_EQ(N, res);
    }
}

TEST(digits, f32)
{
    {
        float N = -6.625f;
        std::vector<int> bits = {
            1, // sign, 1 bit
            1, 0, 0, 0, 0, 0, 0, 1, // exponent, 指数，8bit
            1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // fraction, 尾数, 23bit
        };
        test_f32(N, bits);
    }

    {
        float N = 0.421f;
        std::vector<int> bits = {
            0,
            0, 1, 1, 1, 1, 1, 0, 1,
            1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0
        };
        test_f32(N, bits);
    }

    {
        float N = 0.6875f;
        std::vector<int> bits = {
            0,
            0, 1, 1, 1, 1, 1, 1, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        test_f32(N, bits);
    }

    {
        float N = 13.6875f;
        std::vector<int> bits = {
            0,
            1, 0, 0, 0, 0, 0, 1, 0,
            1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        test_f32(N, bits);
    }
}

TEST(digits, overflow)
{
    {
        for (int i = INT8_MIN; i < 0; i++)
        {
            EXPECT_TRUE(px_determine_add_overflow<int8_t>(INT8_MIN, i));
        }
        
        for (int i = 0; i <= INT8_MAX; i++)
        {
            EXPECT_FALSE(px_determine_add_overflow<int8_t>(INT8_MIN, i));
        }

        for (int i = 1; i <= INT8_MAX; i++)
        {
            EXPECT_TRUE(px_determine_add_overflow<int8_t>(INT8_MAX, i));
        }
    }

    {
        for (int i = INT16_MIN; i < 0; i++)
        {
            EXPECT_TRUE(px_determine_add_overflow<int16_t>(INT16_MIN, i));
        }
        
        for (int i = 0; i <= INT16_MAX; i++)
        {
            EXPECT_FALSE(px_determine_add_overflow<int16_t>(INT16_MIN, i));
        }

        for (int i = 1; i <= INT16_MAX; i++)
        {
            EXPECT_TRUE(px_determine_add_overflow<int16_t>(INT16_MAX, i));
        }
    }

    {
        for (int i = 0; i < 128; i++)
        {
            EXPECT_FALSE(px_determine_add_overflow<uint8_t>(128, i));
        }

        for (int i = 128; i < 256; i++)
        {
            EXPECT_TRUE(px_determine_add_overflow<uint8_t>(128, i));
        }
    }
}

static void dump_float32_ieee(int* bits)
{
    for (int i = 0; i < 32; i++)
    {
        printf("%d, ", bits[i]);
        if (i == 0 || i == 8 || i == 31)
        {
            printf("\n");
        }
    }
}

TEST(digits, tt)
{
    {
        std::vector<int> bits(32, -1);
        px_decimal_to_binary_f32(3.75f, bits.data());
        dump_float32_ieee(bits.data());
    }
}