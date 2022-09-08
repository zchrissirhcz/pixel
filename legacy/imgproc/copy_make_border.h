#pragma once

#include "border_clip.h"

#ifdef __cplusplus
extern "C" {
#endif

void copy_make_border_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right);
void copy_make_border2_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right, BorderType border_type);

#ifdef __cplusplus
}
#endif
