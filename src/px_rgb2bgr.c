#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

void px_rgb2bgr(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(px_is_valid_rgb_image(src));
    PX_ASSERT(px_is_valid_rgb_image(dst));
    PX_ASSERT(px_image_equal_in_size(src, dst));

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

void px_rgb2bgr_inplace(px_image_t* image)
{
    PX_ASSERT(px_is_valid_rgb_image(image));

    const int h = image->height;
    const int w = image->width;
    const int cn = image->channel;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = image->data + i * image->stride;
        for (int j = 0; j < w; j++)
        {
            sp[0] ^= sp[2] ^= sp[0] ^= sp[2];
            sp += cn;
        }
    }
}