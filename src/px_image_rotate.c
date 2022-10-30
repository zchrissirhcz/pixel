#include "px_image.h"
#include "px_assert.h"
#include "px_log.h"

void px_image_rotate90(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(dst->width == src->height);
    PX_ASSERT(dst->height == src->width);
    PX_ASSERT(dst->channel == src->channel);

    const int channel = src->channel;

    for (int i = 0; i < src->height; i++)
    {
        for (int j = 0; j < src->width; j++)
        {
            for (int k = 0; k < channel; k++)
            {
                const int src_idx = i * src->stride + j * channel + k;
                const int dst_idx = j * dst->stride + (dst->width - 1 - i) * channel + k;
                dst->data[dst_idx] = src->data[src_idx];
            }
        }
    }
}

void px_image_rotate270(px_image_t* src, px_image_t* dst)
{

}