#include "copy_make_border.h"
#include <string.h>

void copy_make_border_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right)
{
    int dst_height = src_height + top + bottom;
    int dst_width = src_width + left + right;

    int src_linebytes = src_width * channels * sizeof(unsigned char);
    int dst_linebytes = dst_width * channels * sizeof(unsigned char);
    for (int i=0; i<top; i++) {
        // for (int j=0; j<dst_width; j++){
        // }
        memset(dst+i*dst_linebytes, 0, dst_linebytes);
    }

    int left_bytes = left * sizeof(unsigned char) * channels;
    int right_bytes = right * sizeof(unsigned char) * channels;
    for (int i=0; i<src_height; i++) {
        // for (int j=0; j<left; j++) {
        // }
        int dst_i = i+top;
        memset(dst+dst_i*dst_linebytes, 0, left_bytes);

        int src_i = i;
        memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

        // for (int j=0; j<left; j++) {
        // }
        memset(dst+dst_i*dst_linebytes + left_bytes + src_linebytes, 0, right_bytes);
    }

    for (int i=0; i<bottom; i++) {
        // for (int j=0; j<dst_width; j++){
        // }
        int dst_i = i + top + src_height;
        memset(dst+dst_i*dst_linebytes, 0, dst_linebytes);
    }
}