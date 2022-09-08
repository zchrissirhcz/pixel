#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void histogram_gray_naive(unsigned char* gray, size_t height, size_t width, unsigned int* hist);

void histogram_gray_asimd(unsigned char* gray, size_t height, size_t width, unsigned int* hist);

#ifdef __cplusplus
}
#endif
