#include "px_image.h"
#include "px_log.h"
#include "px_assert.h"
#include "px_compare.h"
#include <stdlib.h>
#include <string.h>

void px_flip(px_image_t* src, px_image_t* dst, PX_FLIP_MODE mode)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_equal_in_shape(src, dst, false));

    const int h = src->height;
    const int w = src->width;
    const int cn = src->channel;
    const int rowlen = src->width * cn;
    if (mode == PX_FLIP_VERTICAL)
    {
        const int bufsize = sizeof(uint8_t) * rowlen;
        uint8_t* bufline = (uint8_t*)malloc(bufsize);
        for (int i = 0; i < h; i++)
        {
            uint8_t* sp = src->data + i * src->stride;
            uint8_t* dp = dst->data + (h - 1 - i) * dst->stride;
            memcpy(dp, sp, bufsize);
        }
        free(bufline);
    }
    else if (mode == PX_FLIP_HORIZONTAL)
    {
        for (int i = 0; i < h; i++)
        {
            uint8_t* sp = src->data + i * src->stride;
            uint8_t* dp = dst->data + i * dst->stride;
            for (int j = 0; j < w; j++)
            {
                int inv_j = w - 1 - j;
                for (int k = 0; k < cn; k++)
                {
                    dp[inv_j * cn + k] = sp[j * cn + k];
                }
            }
        }
    }
    else if (mode == PX_FLIP_BOTH)
    {
        px_image_t* tmp = px_create_image(src->height, src->width, src->channel);
        px_flip(src, tmp, PX_FLIP_VERTICAL);
        px_flip(tmp, dst, PX_FLIP_HORIZONTAL);
        px_destroy_image(tmp);
    }
    else
    {
        PX_LOGE("invalid flipmode: %d\n", mode);
    }
}