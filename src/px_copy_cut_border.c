#include "px_image.h"
#include "px_assert.h"
#include <stdlib.h>
#include <string.h>

void px_copy_cut_border(px_image_t* src, px_image_t* dst, px_pad_t pad)
{
    const int dst_height = src->height - pad.top - pad.bottom;
    const int dst_width = src->width - pad.left - pad.right;
    const int channels = src->channel;
    PX_ASSERT(dst->width == dst_width);
    PX_ASSERT(dst->height == dst_height);

    int dst_linebytes = dst->width * channels * sizeof(unsigned char);
    int src_linebytes = src->width * channels * sizeof(unsigned char);
    int left_bytes = pad.left * channels * sizeof(unsigned char);
    const int m_linebytes = dst_width * channels * sizeof(unsigned char);

    unsigned char* sp = src->data + left_bytes + pad.top * src_linebytes;
    unsigned char* dp = dst->data;
    for (int i = 0; i < dst_height; i++)
    {
        memcpy(dp, sp, m_linebytes);
        dp += dst_linebytes;
        sp += src_linebytes;
    }
}