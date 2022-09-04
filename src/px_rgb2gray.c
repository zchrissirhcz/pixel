#include "px_image.h"
#include "px_assert.h"

void px_rgb2gray(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(dst != NULL && dst->channel == 1);
    PX_ASSERT(src != NULL && src->channel == 3);
    PX_ASSERT(src->height == dst->height && src->width == dst->width);

    const int h = src->height;
    const int w = src->width;
    const int cn = src->channel;
    const int rowlen = cn * w;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = src->data + i * src->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0, dj = 0; j < rowlen; j+=cn, dj++)
        {
            float r = sp[j + 0];
            float g = sp[j + 1];
            float b = sp[j + 2];
            float gray = (0.299f * r + 0.587f * g + 0.114f * b) + 0.5f;
            dp[dj] = gray;
        }
    }

}