#include "gtest/gtest.h"
#include "px_digits.h"

static bool array_equal_s8(int8_t* expected, int8_t* actual)
{
    for (int i = 0; i < 8; i++)
    {
        if (expected[i] != actual[i])
            return false;
    }
    return true;
}

TEST(digits, reverse_bits)
{
    {
        std::vector<int> digits = {1, 0};
        px_reverse_bits(digits.data(), digits.size());
        std::vector<int> expected = {0, 1};
        EXPECT_EQ(expected, digits);
    }

    {
        std::vector<int> digits = {0, 1, 1};
        px_reverse_bits(digits.data(), digits.size());
        std::vector<int> expected = {1, 1, 0};
        EXPECT_EQ(expected, digits);
    }

    {
        std::vector<int> digits = {0, 1, 0, 1, 1, 1, 0, 0};
        px_reverse_bits(digits.data(), digits.size());
        std::vector<int> expected = {0, 0, 1, 1, 1, 0, 1, 0};
        EXPECT_EQ(expected, digits);
    }
}

TEST(digits, decimal_to_binary_s8)
{
    {
        int digits[8];
        px_decimal_to_binary_s8(0, digits);
        int expected[8] = {0};
        EXPECT_TRUE(array_equal_s8);
    }

    {
        int digits[8];
        px_decimal_to_binary_s8(1, digits);
        int expected[8] = {0, 0, 0, 0, 0, 0, 0, 1};
        EXPECT_TRUE(array_equal_s8);
    }

    {
        int digits[8];
        px_decimal_to_binary_s8(-1, digits);
        int expected[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        EXPECT_TRUE(array_equal_s8);
    }

    {
        int digits[8];
        px_decimal_to_binary_s8(-57, digits);
        int expected[8] = {1, 1, 0, 0, 0, 1, 1, 1};
        EXPECT_TRUE(array_equal_s8);
    }
}

TEST(digits, binary_to_decimal_s8)
{
    for (int expected : {-57, 71})
    {
        int digits[8];
        px_decimal_to_binary_s8(expected, digits);
        int8_t actual = px_binary_to_decimal_s8(digits);
        EXPECT_EQ(expected, actual);
    }
}
