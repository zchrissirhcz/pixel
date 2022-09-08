#include "copy_cut_border.h"
#include <string.h>

void copy_cut_border(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right)
{
    int dst_height = src_height - top - bottom;
    int dst_width = src_width - left - right;

    int dst_linebytes = dst_width * channels * sizeof(unsigned char);
    int src_linebytes = src_width * channels * sizeof(unsigned char);
    int left_bytes = left * channels * sizeof(unsigned char);
    for (int i=0; i<dst_height; i++) {
        int src_i = i + top;
        memcpy(dst+i*dst_linebytes, src+src_i*src_linebytes+left_bytes, dst_linebytes);
    }
}