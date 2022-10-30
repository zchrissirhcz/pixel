#include "px_image.h"
#include "px_assert.h"
#include "px_log.h"

#include <stdlib.h>
#include <string.h>

void px_copy_make_border(px_image_t* src, px_image_t* dst, px_pad_t pad, px_border_type border_type)
{
    PX_ASSERT(border_type == PX_BORDER_CONSTANT ||
            border_type == PX_BORDER_REFLECT  ||
            border_type == PX_BORDER_WRAP     ||
            border_type == PX_BORDER_REFLECT  ||
            border_type == PX_BORDER_REPLICATE);

    int dst_height = src->height + pad.top + pad.bottom;
    int dst_width = src->width + pad.left + pad.right;
    PX_ASSERT(dst_height == dst->height);
    PX_ASSERT(dst_width == dst->width);

    const int src_width = src->width;
    const int src_height = src->height;
    const int channels = src->channel;

    int left_bytes = pad.left * sizeof(unsigned char) * channels;
    int right_bytes = pad.right * sizeof(unsigned char) * channels;

    int src_linebytes = src_width * channels * sizeof(unsigned char);
    int dst_linebytes = dst_width * channels * sizeof(unsigned char);
    for (int i = 0; i < pad.top; i++)
    {
        if (border_type == PX_BORDER_CONSTANT)
        {
            memset(dst+i*dst_linebytes, 0, dst_linebytes);
        }
        else {
            int src_i = px_border_clip(border_type, -pad.top+i, src_height);

            for (int j = 0; j < pad.left; j++) {
                int src_j = px_border_clip(border_type, -pad.left+j, src_width);
                for (int k=0; k<channels; k++) {
                    *(dst+i*dst_linebytes+j*channels+k) = *(src+src_i*src_linebytes+src_j*channels+k);
                }
            }

            // 0  -top
            // 1  -top+1
            // 2  -top+2
            
            memcpy(dst+i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

            for (int j = 0; j < pad.right; j++)
            {
                int src_j = px_border_clip(border_type, src_width+j, src_width);
                for (int k=0; k<channels; k++)
                {
                    *(dst+i*dst_linebytes+left_bytes+src_linebytes+j*channels + k) = *(src+src_i*src_linebytes+src_j*channels + k);
                }
            }
        }
    }

    for (int i = 0; i < src_height; i++)
    {
        int dst_i = i+pad.top;
        if (border_type==PX_BORDER_CONSTANT)
        {
            memset(dst+dst_i*dst_linebytes, 0, left_bytes);
        }
        else
        {
            for (int j = 0; j < pad.left; j++)
            {
                int src_j = px_border_clip(border_type, j-pad.left, src_width);
                for (int k=0; k<channels; k++)
                {
                    *(dst+dst_i*dst_linebytes+j*channels + k) = *(src+i*src_linebytes+src_j*channels + k);
                }
            }
        }

        int src_i = i;
        memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

        if (border_type==PX_BORDER_CONSTANT)
        {
            memset(dst+dst_i*dst_linebytes + left_bytes + src_linebytes, 0, right_bytes);
        }
        else
        {
            for (int j = 0; j < pad.right; j++)
            {
                int src_j = px_border_clip(border_type, src_width+j, src_width);
                for (int k = 0; k < channels; k++)
                {
                    *(dst+dst_i*dst_linebytes + left_bytes + src_linebytes + j*channels + k) = *(src+i*src_linebytes+src_j*channels + k);
                }
            }
        }
    }

    for (int i = 0; i < pad.bottom; i++)
    {
        int dst_i = i + pad.top + src_height;
        if (border_type==PX_BORDER_CONSTANT)
        {
            memset(dst+dst_i*dst_linebytes, 0, dst_linebytes);
        }
        else
        {
            int src_i = px_border_clip(border_type, src_height+i, src_height);

            //memset(dst+dst_i*dst_linebytes, 0, left_bytes);
            for (int j = 0; j < pad.left; j++)
            {
                int src_j = px_border_clip(border_type, j-pad.left, src_width);
                for (int k=0; k<channels; k++)
                {
                    *(dst+dst_i*dst_linebytes+j*channels+k) = *(src+src_i*src_linebytes+src_j*channels+k);
                }
            }

            // 0 - size
            // 1 - size+1
            // 2 - size+2
            memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

            //memset(dst+dst_i*dst_linebytes+left_bytes+src_linebytes, 0, right_bytes);
            for (int j = 0; j < pad.right; j++)
            {
                int src_j = px_border_clip(border_type, src_width+j, src_width);
                for (int k = 0; k < channels; k++)
                {
                    *(dst+dst_i*dst_linebytes+left_bytes+src_linebytes+j*channels+k) = *(src+src_i*src_linebytes+src_j*channels+k);
                }
            }
        }
    }
}