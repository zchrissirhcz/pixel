#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

static void px_rgbx_to_bgrx(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src->channel >= 3);
    PX_ASSERT(dst->channel >= 3);

    const int h = src->height;
    const int w = src->width;
    const int scn = src->channel;
    const int dcn = dst->channel;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = src->data + i * src->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0; j < w; j++)
        {
            dp[0] = sp[2];
            dp[1] = sp[1];
            dp[2] = sp[0];
            if (dcn == 4 && scn == 4)
                dp[3] = sp[3];
            else if (dcn == 4 && scn == 3)
                dp[3] = 255;
            dp += dcn;
            sp += scn;
        }
    }
}

void px_rgb_to_bgr(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(px_is_valid_rgb_image(src));
    PX_ASSERT(px_is_valid_rgb_image(dst));
    PX_ASSERT(px_image_equal_in_size(src, dst));
    px_rgbx_to_bgrx(src, dst);
}

void px_rgba_to_bgra(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(px_is_valid_rgba_image(src));
    PX_ASSERT(px_is_valid_rgba_image(dst));
    PX_ASSERT(px_image_equal_in_size(src, dst));
    px_rgbx_to_bgrx(src, dst);
}

static void px_rgbx_to_bgrx_inplace(px_image_t* image)
{
    PX_ASSERT(image->channel >= 3);

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

void px_rgb_to_bgr_inplace(px_image_t* image)
{
    PX_ASSERT(px_is_valid_rgb_image(image));
    px_rgbx_to_bgrx_inplace(image);
}

void px_rgba_to_bgra_inplace(px_image_t* image)
{
    PX_ASSERT(px_is_valid_rgba_image(image));
    px_rgbx_to_bgrx_inplace(image);
}