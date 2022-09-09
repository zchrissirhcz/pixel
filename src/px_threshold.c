#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"

void px_threshold(px_image_t* gray, px_image_t* binary, uint8_t thresh, uint8_t minval, uint8_t maxval)
{
    PX_ASSERT(gray != NULL && binary != NULL);
    PX_ASSERT(gray->data != NULL && binary->data != NULL);
    PX_ASSERT(gray->channel == 1 && binary->channel == 1);
    PX_ASSERT(gray->height == binary->height && gray->width == binary->width);

    const int h = gray->height;
    const int w = gray->width;
    const int srcstride = gray->stride;
    const int dststride = binary->stride;
    for (int i = 0; i < h; i++)
    {
        const uint8_t* sp = gray->data + i * srcstride;
        uint8_t* dp = binary->data + i * dststride;
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