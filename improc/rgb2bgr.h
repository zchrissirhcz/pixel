#ifndef PIXEL_RGB2BGR_H
#define PIXEL_RGB2BGR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void rgb2bgr_naive(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes);

void rgb2bgr_idxopt(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes);

void rgb2bgr_asimd(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes);

void rgb2bgr_asm(unsigned char* src_buf, size_t height, size_t width, size_t src_linebytes, unsigned char* dst_buf, size_t dst_linebytes);


void rgb2bgr_inplace_naive(unsigned char* buf, size_t height, size_t width, size_t linebytes);

void rgb2bgr_inplace_naive2(unsigned char* buf, size_t height, size_t width, size_t linebytes);

void rgb2bgr_inplace_asm(unsigned char* buf, size_t height, size_t width, size_t linebytes);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_RGB2BGR_H