#include "improc_private.h"
#include "fc_log.h"
#include <stdio.h>

namespace fc {

void fc_upside_down_image(const Image* im, Image* im_upsd)
{
    *im_upsd = make_image(im->width, im->height, im->channel);
    long t_start;

#if 0
    t_start = fc_gettime();
    for (size_t h = 0; h < im->height; h++) {
        for (size_t w = 0; w < im->width; w++) {
            for (size_t c = 0; c < im->channel; c++) {
                size_t src_idx = h*im->width*im->channel + w*im->channel + c;
                size_t dst_idx = (im->height-h)*im->w*im->channel + w*im->channel + c;
                im_upsd->data[dst_idx] = im->data[src_idx];
            }
        }
    }
    long t1 = fc_gettime() - t_start;

    t_start = fc_gettime();
    for (size_t h = 0; h < im->height; h++) {
        for (size_t wc = 0; wc < im->width*im->channel; wc++) {
            size_t src_idx = h * im->width*im->channel + wc;
            size_t dst_idx = (im->height - h)*im->width*im->channel + wc;
            im_upsd->data[dst_idx] = im->data[src_idx];
        }
    }
    long t2 = fc_gettime() - t_start;
#endif

    t_start = fc_gettime();
    unsigned char* dst = im_upsd->data + im_upsd->linebytes*(im->height-1);
    unsigned char* src = im->data;
    for (int h = 0; h < im->height; h++) {
        for (int wc = 0; wc < im->width*im->channel; wc++) {
            size_t src_idx = h*im->linebytes + wc;
            size_t dst_idx = (im->height-1-h)*im_upsd->linebytes + wc;
            im_upsd->data[dst_idx] = im->data[src_idx];
        }
        memcpy(dst, src, im_upsd->linebytes);
        dst -= im_upsd->linebytes;
        src += im->linebytes;
    }
    long t3 = fc_gettime() - t_start;

    //printf("--- upside down function time cost, t1=%lu ms, t2=%lu ms, t3=%lu ms\n", t1, t2, t3);
}


}
