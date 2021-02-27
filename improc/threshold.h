#ifndef PIXEL_IMPROC_THRESHOLD_H
#define PIXEL_IMPROC_THRESHOLD_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void threshold_gray(unsigned char* gray_buf, size_t height, size_t width, unsigned char* output_buf, unsigned char thresh);

void threshold_rgb(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh);

void threshold_gray_inplace(unsigned char* gray_buf, size_t height, size_t width, unsigned char thresh);

#ifdef __cplusplus
}
#endif

#endif