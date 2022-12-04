#include "px_image.h"
#include "px_assert.h"

void px_negative_image(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(src->channel == 1);
    PX_ASSERT(dst->channel == 1);
    PX_ASSERT(src->width == dst->width);
    PX_ASSERT(src->height == dst->height);

    const int w = src->width;
    const int h = src->height;
    const int cn = src->channel;

    for (int i = 0; i < h; i++)
    {
        const uint8_t* sp = src->data + i * src->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0; j < w; j++)
        {
            for (int k = 0; k < cn; k++)
            {
                dp[j * cn + k] = 255 - sp[j * cn + k];
            }
        }
    }
}