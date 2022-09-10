#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

void px_threshold(px_image_t* gray, px_image_t* binary, uint8_t thresh, uint8_t minval, uint8_t maxval)
{
    PX_ASSERT(px_is_valid_gray_image(gray));
    PX_ASSERT(px_is_valid_gray_image(binary));
    PX_ASSERT(px_image_equal_in_size(gray, binary));

    const int h = gray->height;
    const int w = gray->width;
    for (int i = 0; i < h; i++)
    {
        const uint8_t* sp = gray->data + i * gray->stride;
        uint8_t* dp = binary->data + i * binary->stride;
        for (int j = 0; j < w; j++)
        {
            if (sp[j] > thresh)
            {
                dp[j] = maxval;
            }
            else
            {
                dp[j] = minval;
            }
        }
    }
}