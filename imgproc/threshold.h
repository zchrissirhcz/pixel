#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// if (pixel[i]>thresh)
//      pixel[i] = maxval
// else
//      pixel[i] = minval

// for most cases, set minval=0, maxval=255, invert=false
void threshold_gray_naive(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval);

void threshold_gray_asimd(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval);

void threshold_gray_asm(unsigned char* input_gray, size_t height, size_t width, unsigned char* output_gray, unsigned char thresh, unsigned char minval, unsigned char maxval);

//--------------------------------------------------

void threshold_rgb_naive(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval);

void threshold_rgb_asimd(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval);

void threshold_rgb_asm(unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf, unsigned char thresh, unsigned char minval, unsigned char maxval);

void threshold_gray_inplace(unsigned char* gray_buf, size_t height, size_t width, unsigned char thresh, unsigned char minval, unsigned char maxval);


#ifdef __cplusplus
}
#endif

