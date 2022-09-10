#include "gtest/gtest.h"
#include "px_arithm.h"

TEST(arithm, clamp)
{
    int x = 10;

    EXPECT_EQ(10, px_clamp(x, 5, 15));
    EXPECT_EQ(11, px_clamp(x, 11, 15));
    EXPECT_EQ(9, px_clamp(x, 5, 9));
    EXPECT_EQ(-1, px_clamp(x, -3, -1));

    EXPECT_EQ(10, px_clamp(x, 15, 5));
    EXPECT_EQ(11, px_clamp(x, 15, 11));
    EXPECT_EQ(9, px_clamp(x, 9, 5));
    EXPECT_EQ(-1, px_clamp(x, -1, -3));
}