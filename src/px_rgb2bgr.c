#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

void px_rgb2bgr(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_equal_in_shape(src, dst, false));
    PX_ASSERT(src->channel == 3);
    const int h = src->height;
    const int w = src->width;
    const int cn = src->channel;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = src->data + i * src->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0; j < w; j++)
        {
            dp[0] = sp[2];
            dp[1] = sp[1];
            dp[2] = sp[0];
            dp += cn;
            sp += cn;
        }
    }
}

