#include "px_arithm.h"

int px_clamp(int x, int minval, int maxval)
{
    if (minval > maxval)
    {
        int tmp = minval;
        minval = maxval;
        maxval = tmp;
    }
    if (x < minval)
    {
        return minval;
    }
    else if (x > maxval)
    {
        return maxval;
    }
    return x;
}

int px_align_up(int x, int power_of_2)
{
    const int n = power_of_2;
    return ((x + n - 1) / n) * n;
}