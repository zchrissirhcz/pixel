#ifndef PIXEL_IMPROC_FLIP_H
#define PIXEL_IMPROC_FLIP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void flip_horiz_rgb_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_horiz_rgb_idxopt(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_horiz_rgb_asimd(unsigned char* src, size_t height, size_t width, unsigned char* dst);

//----------------------------------------------------------------------

void flip_horiz_gray_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_horiz_gray_asimd(unsigned char* src, size_t height, size_t width, unsigned char* dst);

//----------------------------------------------------------------------

void flip_vert_rgb_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_vert_rgb_bylines(unsigned char* src, size_t height, size_t width, unsigned char* dst);

//----------------------------------------------------------------------

void flip_vert_gray_naive(unsigned char* src, size_t height, size_t width, unsigned char* dst);
void flip_vert_gray_bylines(unsigned char* src, size_t height, size_t width, unsigned char* dst);

//----------------------------------------------------------------------//TODO

void flip_horiz_rgb_inplace(unsigned char* src, size_t height, size_t width);

#ifdef __cplusplus
}
#endif


#endif