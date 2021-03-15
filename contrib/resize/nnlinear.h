#ifndef PIXEL_RESIZE_NNLINEAR_H
#define PIXEL_RESIZE_NNLINEAR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void nnlinear_rgb(unsigned char* src_buf, size_t src_h, size_t src_w, unsigned char* dst_buf, size_t dst_h, size_t dst_w);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_RESIZE_NNLINEAR_H
