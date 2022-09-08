#include "px_blas.h"
#include <stdint.h>

float array_mean_u8(unsigned char* data, int len)
{
    uint64_t sum = 0; // note: if use float type for `sum`, when sum>=16777216 and as integer, sum would be incorrect
    for (int i = 0; i < len; i++)
    {
        sum += data[i];
    }
    return sum*1.0f / len;
}