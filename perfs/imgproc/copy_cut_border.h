#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void copy_cut_border(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right);

#ifdef __cplusplus
}
#endif
