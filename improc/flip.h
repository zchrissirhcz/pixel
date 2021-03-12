#ifndef PIXEL_IMPROC_FLIP_H
#define PIXEL_IMPROC_FLIP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void flip_rgb_horiz_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_rgb_horiz_idxopt(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_rgb_horiz_asimd(unsigned char* src, size_t height, size_t width, unsigned char* dst);

//----------------------------------------------------------------------

void flip_gray_horiz_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_gray_horiz_asimd(unsigned char* src, size_t height, size_t width, unsigned char* dst);

//----------------------------------------------------------------------

void flip_rgb_horiz_inplace(unsigned char* src, size_t height, size_t width);

#ifdef __cplusplus
}
#endif


#endif