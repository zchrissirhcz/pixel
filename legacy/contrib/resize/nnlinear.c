#include "nnlinear.h"

void nnlinear_rgb(unsigned char* src_buf, size_t src_h, size_t src_w, unsigned char* dst_buf, size_t dst_h, size_t dst_w)
{
    float scale_h = src_h*1.0 / dst_h;
    float scale_w = src_w*1.0 / dst_w;
    size_t src_linebytes = src_w * 3;
    size_t dst_linebytes = dst_w * 3;
    for (size_t h=0; h<dst_h; h++) {
        int sh = h * scale_h;
        for (size_t w=0; w<dst_w; w++) {
            int sw = w * scale_w;
            for (size_t c=0; c<3; c++) {
                size_t src_idx = sh * src_linebytes + sw * 3 + c;
                size_t dst_idx = h * dst_linebytes + w * 3 + c;
                dst_buf[dst_idx] = src_buf[src_idx];
            }
        }
    }
}

