#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

void rgb2bgr_naive(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf);

void rgb2bgr_idxopt(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf);

void rgb2bgr_asimd(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf);

void rgb2bgr_asm(unsigned char* src_buf, size_t height, size_t width, unsigned char* dst_buf);


void rgb2bgr_inplace_naive(unsigned char* buf, size_t height, size_t width);

void rgb2bgr_inplace_naive2(unsigned char* buf, size_t height, size_t width);

void rgb2bgr_inplace_asm(unsigned char* buf, size_t height, size_t width);

#ifdef __cplusplus
}
#endif
