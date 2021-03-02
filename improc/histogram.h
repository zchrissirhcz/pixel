#ifndef PIXEL_IMPROC_HISTOGRAM_H
#define PIXEL_IMPROC_HISTOGRAM_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void histogram_gray_naive(unsigned char* gray, size_t height, size_t width, unsigned char* hist);

void histogram_rgb_naive(unsigned char* rgb, size_t height, size_t width, unsigned char* hist, int mode);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_IMPROC_HISTOGRAM_H