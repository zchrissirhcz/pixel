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