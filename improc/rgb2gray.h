#ifndef PIXEL_IMPROC_RGB2GRAY
#define PIXEL_IMPROC_RGB2GRAY

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void rgb_to_gray_naive(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes);

void rgb_to_gray_fast(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes);
void rgb_to_gray_fast2(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes);


void f_rgb_to_gray(unsigned char* rgb_buf, size_t height, size_t width, size_t rgb_linebytes, unsigned char* gray_buf, size_t gray_linebytes, int mode);


#ifdef __cplusplus
}
#endif

#endif // PIXEL_IMPROC_RGB2GRAY