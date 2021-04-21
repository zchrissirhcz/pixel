#ifndef PIXEL_IMPROC_COPY_MAKE_BORDER
#define PIXEL_IMPROC_COPY_MAKE_BORDER


#ifdef __cplusplus
extern "C" {
#endif

void copy_make_border_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_IMPROC_COYP_MAKE_BORDER