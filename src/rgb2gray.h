#ifndef PIXEL_IMPROC_RGB2GRAY
#define PIXEL_IMPROC_RGB2GRAY

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void pixel_rgb2gray_naive(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf);

void pixel_rgb2gray_fixed(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf);

void pixel_rgb2gray_fixed_asimd(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf);

void pixel_rgb2gray_fixed_asm(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_IMPROC_RGB2GRAY