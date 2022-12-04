#include "px_image.h"
#include "px_assert.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void px_sobel3x3(px_image_t* src, px_image_t* dst)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(src->channel == 1);
    PX_ASSERT(dst->channel == 1);
    PX_ASSERT(src->width == dst->width);
    PX_ASSERT(src->height == dst->height);

    int8_t kx[9] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    int8_t ky[9] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    };

    int8_t* kx0 = kx + 0;
    int8_t* kx1 = kx + 3;
    int8_t* kx2 = kx + 6;

    int8_t* ky0 = ky + 0;
    int8_t* ky1 = ky + 3;
    int8_t* ky2 = ky + 6;

    const int h = src->height;
    const int w = src->width;
    const int bufsize = h * w * sizeof(int16_t);
    int16_t* grad = (int16_t*)malloc(bufsize);
    memset(grad, 0, bufsize);
    for (int i = 1; i < h - 1; i++)
    {
        int16_t* dp = grad + i * dst->stride;
        uint8_t* sp0 = src->data + (i - 1) * src->stride;
        uint8_t* sp1 = src->data + (i + 0) * src->stride;
        uint8_t* sp2 = src->data + (i + 1) * src->stride;
        for (int j = 1; j < w - 1; j++)
        {
            int16_t gx = sp0[0] * kx0[0] + sp0[1] * kx0[1] + sp0[2] * kx0[2]
                         + sp1[0] * kx1[0] + sp1[1] * kx1[1] + sp1[2] * kx1[2]
                         + sp2[0] * kx2[0] + sp2[1] * kx2[1] + sp2[2] * kx2[2];

            int16_t gy = sp0[0] * ky0[0] + sp0[1] * ky0[1] + sp0[2] * ky0[2]
                         + sp1[0] * ky1[0] + sp1[1] * ky1[1] + sp1[2] * ky1[2]
                         + sp2[0] * ky2[0] + sp2[1] * ky2[1] + sp2[2] * ky2[2];
            int16_t res = sqrt(gx * gx + gy * gy);
            //int16_t res = fabs(gx) + fabs(gy);
            sp0++;
            sp1++;
            sp2++;
            dp[j] = res;
        }
    }

    int16_t minval = INT16_MAX;
    int16_t maxval = INT16_MIN;
    for (int i = 0; i < h; i++)
    {
        int16_t* sp = grad + i * dst->stride;
        for (int j = 0; j < w; j++)
        {
            if (sp[j] > maxval)
            {
                maxval = sp[j];
            }
            if (sp[j] < minval)
            {
                minval = sp[j];
            }
        }
    }

    float scale;
    if (maxval == minval)
    {
        if (maxval == 0)
        {
            scale = 1.0f;
        }
        else
        {
            scale = 255.f / maxval;
        }
    }
    else
    {
        scale = 255.0f / (maxval - minval);
    }

    for (int i = 0; i < h; i++)
    {
        int16_t* sp = grad + i * dst->stride;
        uint8_t* dp = dst->data + i * dst->stride;
        for (int j = 0; j < w; j++)
        {
            dp[j] = sp[j] * scale;
        }
    }
}
