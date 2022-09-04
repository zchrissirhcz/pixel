#pragma once

#include "boxfilter.h"
#include "border_clip.h"

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

void boxfilter_naive(unsigned char* src, unsigned char* dst, int height, int width, int channels, int kernel_h, int kernel_w, int anchor_y, int anchor_x, bool norm, BorderType border_type);
//void boxfilter_xydir(unsigned char* src, unsigned char* dst, int height, int width, int channels, int kernel_h, int kernel_w, int anchor_y, int anchor_x, bool norm, BorderType border_type);

#ifdef __cplusplus
}
#endif
