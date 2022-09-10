#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

void px_rgb2gray(px_image_t* src, px_image_t* dst)
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
            double r = sp[0];
            double g = sp[1];
            double b = sp[2];
            double gray = (0.299 * r + 0.587 * g + 0.114 * b) + 0.5;
            *dp++ = gray;
            sp += cn;
        }
    }
}