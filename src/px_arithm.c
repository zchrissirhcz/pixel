#include "px_arithm.h"
#include "px_log.h"

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

// square root by Carmack
float px_fast_sqrt(float num)
{
    if (num < 0)
    {
        PX_LOGE("%s only support num >= 0 case", __FUNCTION__);
        return -1;
    }
    int i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = num * 0.5F;
    y  = num;
    i  = * ( int * ) &y;
    i  = 0x5f375a86 - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );
    y  = y * ( threehalfs - ( x2 * y * y ) );
    return num*y;
}