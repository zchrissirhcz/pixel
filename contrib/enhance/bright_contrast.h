#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void bright_contrast(unsigned char* src_rgb, size_t height, size_t width, int bright, int contrast, unsigned char* dst_rgb);

#ifdef __cplusplus
}
#endif
