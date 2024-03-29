#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    void mean_stddev_u8_naive(unsigned char* src, size_t len, float* mean, float* stddev);
    void mean_stddev_u8_asimd1(unsigned char* src, size_t len, float* mean, float* stddev);
    
    void mean_stddev_u8_fast(unsigned char* src, size_t len, float* mean, float* stddev);
    void mean_stddev_u8_asimd2(unsigned char* src, size_t len, float* mean, float* stddev);
#ifdef __cplusplus
}
#endif

