#include "mean_stddev.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#if 0
void mean_stddev_u8_naive_wrong_by_float(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    float sum = 0; // 当 sum > 16777215 时，无法准确表示整数，会导致结果（很可能）不正确
    for (size_t i = 0; i < len; i++) {
        sum += src[i];
    }
    
    float mean = sum / len;
    
    sum = 0;
    for (size_t i=0; i<len; i++) {
        float diff = (float)(src[i]) - mean;
        sum += diff * diff;  // 这里用float类型的sum累加，也会导致不准确
    }
    float stddev = sqrt(sum);

    *_mean = mean;
    *_stddev = stddev;
}
#endif

void mean_stddev_u8_naive(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    uint64_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += src[i];
    }
    
    float mean = sum*1.0 / len;

    double dev = 0;
    for (size_t i = 0; i < len; i++) {
        float diff = (float)(src[i]) - mean;
        dev += diff * diff;
    }
    double stddev = sqrt(dev / len);

    *_mean = mean;
    *_stddev = stddev;
}

void mean_stddev_u8_asimd(unsigned char* src, size_t len, float* _mean, float* stddev)
{
    
}