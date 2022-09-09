#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

void px_rgb2bgr(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_shape_equal(src, dst, false));
    PX_ASSERT(src->channel == 3);
    const int h = src->height;
    const int w = src->width;
    const int cn = src->channel;
    const int rowlen = w * cn;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = src->data + i * src->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0; j < rowlen; j += cn)
        {
            dp[j + 0] = sp[j + 2];
            dp[j + 1] = sp[j + 1];
            dp[j + 2] = sp[j + 0];
        }
    }
}

