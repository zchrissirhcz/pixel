#include "px_image.h"
#include "px_assert.h"
#include "px_arithm.h"
#include "px_compare.h"

void px_resize_nearest(px_image_t* src, px_image_t* dst, px_size_t dsize)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_size_equal(dst, dsize));
    PX_ASSERT(src->channel == dst->channel);

    const int channel = dst->channel;

    const double scale_height = src->height * 1.0 / dst->height;
    const double scale_width = src->width * 1.0 / dst->width;

    for (int i = 0; i < dst->height; i++)
    {
        for (int j = 0; j < dst->width; j++)
        {
            int di = i;
            int dj = j;

            // interpolation
            int si = (di * scale_height) + 0.5;
            int sj = (dj * scale_width) + 0.5;

            si = px_clamp(si, 0, src->height - 1);
            sj = px_clamp(sj, 0, src->width - 1);

            // dst[i, j, k] = src[si, sj, k]
            for (int k = 0; k < channel; k++)
            {
                const int dst_idx = di * dst->stride + dj * channel + k;
                const int src_idx = si * src->stride + sj * channel + k;
                dst->data[dst_idx] = src->data[src_idx];
            }
        }
    }
}