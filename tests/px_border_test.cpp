#include "px_image.h"
#include "gtest/gtest.h"

TEST(border, clip_constant)
{
    const int size = 5;
    for (int default_value: {0, 1, -1})
    {
        for (int x : {-2, -1, size, size + 1, size + 2, 2 * size, 2 * size + 1})
        {
            int actual = px_border_clip_constant(x, size, default_value);
            int expected = default_value;
            EXPECT_EQ(expected, actual);
        }

        for (int x = 0; x < size; x++)
        {
            int actual = px_border_clip_constant(x, size, default_value);
            int expected = x;
            EXPECT_EQ(expected, actual);
        }
    }
}

TEST(border, clip_replicate)
{
    const int size = 5;
    {
        int expected = 0;
        for (int x : {-2, -1})
        {
            int actual = px_border_clip_replicate(x, size);
            EXPECT_EQ(expected, actual);
        }
    }

    {
        int expected = size - 1;
        for (int x : {size, size + 1, size + 2, 2 * size, 2 * size + 1})
        {
            int actual = px_border_clip_replicate(x, size);
            EXPECT_EQ(expected, actual);
        }
    }

    for (int x = 0; x < size; x++)
    {
        int actual = px_border_clip_replicate(x, size);
        int expected = x;
        EXPECT_EQ(expected, actual);
    }
}

TEST(border, clip_reflect)
{
    const int size = 8;

    for (int x = -6; x <= -1; x++)
    {
        int actual = px_border_clip_reflect(x, size);
        int expected = x * (-1) - 1;
        EXPECT_EQ(expected, actual) << "x=" << x;
    }

    for (int x = 0; x < size; x++)
    {
        int actual = px_border_clip_reflect(x, size);
        int expected = x;
        EXPECT_EQ(expected, actual);
    }

    for (int i = 0; i < size; i++)
    {
        int x = i + size;
        int expected = size - 1 - i;
        int actual = px_border_clip_reflect(x, size);
        EXPECT_EQ(expected, actual);
    }
}

TEST(border, border_clip_wrap)
{
    const int size = 8;
    for (int x = -6; x <= -1; x++)
    {
        int actual = px_border_clip_wrap(x, size);
        int expected = x + size;
        EXPECT_EQ(expected, actual);
    }

    for (int x = 0; x < size; x++)
    {
        int actual = px_border_clip_wrap(x, size);
        int expected = x;
        EXPECT_EQ(expected, actual);
    }
    
    for (int i = 0; i < size; i++)
    {
        int x = i + size;
        int actual = px_border_clip_wrap(x, size);
        int expected = i;
        EXPECT_EQ(expected, actual);
    }
}

TEST(border, border_clip_reflect101)
{
    const int size = 8;
    for (int x = -6; x <= -1; x++)
    {
        int actual = px_border_clip_reflect101(x, size);
        int expected = x * (-1);
        EXPECT_EQ(expected, actual);
    }
    
    for (int x = 0; x < size; x++)
    {
        int actual = px_border_clip_reflect101(x, size);
        int expected = x;
        EXPECT_EQ(expected, actual);
    }

    for (int i = 0; i < size - 1; i++)
    {
        int x = size + i;
        int actual = px_border_clip_reflect101(x, size);
        int expected = (size - 2) - i;
        EXPECT_EQ(expected, actual);
    }
}