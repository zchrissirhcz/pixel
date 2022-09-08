#include "px_image.h"
#include "px_assert.h"
#include "px_arithm.h"

void px_resize_nearest(px_image_t* src, px_image_t* dst, px_size_t dsize)
{
    // TODO: implement by calling interpolation function

    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_size_equal(dsize, dst->size));
    PX_ASSERT(src->channel == dst->channel);

    const int height = dst->height;
    const int width = dst->width;
    const int channel = dst->channel;

    const float scale_height = src->height * 1.0 / dst->height;
    const float scale_width = src->width * 1.0 / dst->width;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int dst_i = i;
            int dst_j = j;
            int src_i = (dst_i * scale_height) + 0.5f;
            int src_j = (dst_j * scale_width) + 0.5f;

            src_i = px_clamp(src_i, 0, src->height - 1);
            src_j = px_clamp(src_j, 0, src->width - 1);
            
            for (int k = 0; k < channel; k++)
            {
                const int dst_idx = dst_i * dst->stride + dst_j * channel + k;
                const int src_idx = src_i * src->stride + src_j * channel + k;
                dst->data[dst_idx] = src->data[src_idx];
            }
        }
    }
}