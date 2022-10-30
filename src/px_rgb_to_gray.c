#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"
#include "px_arithm.h"

static void px_rgb_like_to_gray(px_image_t* src, px_image_t* dst, int bIdx)
{
    PX_ASSERT(px_is_valid_rgb_image(src));
    PX_ASSERT(px_is_valid_gray_image(dst));
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
            double r = sp[2 - bIdx];
            double g = sp[1];
            double b = sp[bIdx];
            double gray = (0.299 * r + 0.587 * g + 0.114 * b) + 0.5;
            *dp++ = gray;
            sp += cn;
        }
    }
}

void px_rgb_to_gray(px_image_t* src, px_image_t* dst)
{
    const int b_idx = 2;
    px_rgb_like_to_gray(src, dst, b_idx);
}

void px_bgr_to_gray(px_image_t* src, px_image_t* dst)
{
    const int b_idx = 0;
    px_rgb_like_to_gray(src, dst, b_idx);
}

void px_rgb_to_gray_fixed(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(px_is_valid_rgb_image(src));
    PX_ASSERT(px_is_valid_gray_image(dst));
    PX_ASSERT(px_image_equal_in_size(src, dst));

    //ref: icvCvt_BGR2Gray_8u_C3C1R(), from OpenCV 3.4.9
    //opencv/modules/imgcodecs/src/utils.cpp
    #define  SCALE  14
    #define  cR  (int)(0.299*(1 << SCALE) + 0.5)
    #define  cG  (int)(0.587*(1 << SCALE) + 0.5)
    #define  cB  ((1 << SCALE) - cR - cG)
    #define  descale(x,n)  (((x) + (1 << ((n)-1))) >> (n))

    const int bIdx = 2;
    const int h = src->height;
    const int w = src->width;
    const int cn = src->channel;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = src->data + i * src->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0; j < w; j++)
        {
            uint8_t r = sp[2 - bIdx];
            uint8_t g = sp[1];
            uint8_t b = sp[bIdx];
            int gray = descale(cR * r + cG * g + cB * b, SCALE);
            *dp++ = px_clamp(gray, 0, 255);
            sp += cn;
        }
    }

    #undef SCALE
    #undef cR
    #undef cG
    #undef cB
    #undef descale
}