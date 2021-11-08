#ifndef PIXEL_IMPROC_COPY_CUT_BORDER_H
#define PIXEL_IMPROC_COPY_CUT_BORDER_H

#ifdef __cplusplus
extern "C" {
#endif

void copy_cut_border(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_IMPROC_COPY_CUT_BORDER_H