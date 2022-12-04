#include "px_image.h"
#include "px_assert.h"
#include "px_log.h"

void px_image_rotate(px_image_t* src, px_image_t* dst, PX_ROTATE_MODE mode)
{
    if (mode == PX_ROTATE_CLOCK_WISE_90 || mode == PX_ROTATE_CLOCK_WISE_270)
    {
        PX_ASSERT(dst->width == src->height);
        PX_ASSERT(dst->height == src->width);
    }
    else if (mode == PX_ROTATE_CLOCK_WISE_180)
    {
        PX_ASSERT(dst->width == src->width);
        PX_ASSERT(dst->height == src->height);
    }
    PX_ASSERT(dst->channel == src->channel);

    const int channel = src->channel;

    for (int i = 0; i < src->height; i++)
    {
        for (int j = 0; j < src->width; j++)
        {
            for (int k = 0; k < channel; k++)
            {
                int src_idx;
                int dst_idx;
                if (mode == PX_ROTATE_CLOCK_WISE_90)
                {
                    src_idx = i * src->stride + j * channel + k;
                    dst_idx = j * dst->stride + (dst->width - 1 - i) * channel + k;
                    dst->data[dst_idx] = src->data[src_idx];
                }
                else if (mode == PX_ROTATE_CLOCK_WISE_270)
                {
                    src_idx = i * src->stride + j * channel + k;
                    dst_idx = (dst->height - 1 - j) * dst->stride + i * channel + k;
                    dst->data[dst_idx] = src->data[src_idx];
                }
                else if (mode == PX_ROTATE_CLOCK_WISE_180)
                {
                    src_idx = i * src->stride + j * channel + k;
                    dst_idx = (dst->height - 1 - i) * dst->stride + (dst->width - 1 - j) * channel + k;
                    dst->data[dst_idx] = src->data[src_idx];
                }
            }
        }
    }
}
