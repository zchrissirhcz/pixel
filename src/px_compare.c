#include "px_image.h"
#include "px_cnn.h"
#include "px_compare.h"
#include "px_log.h"
#include <math.h>
#include <stdlib.h>

bool px_image_almost_equal(px_image_t* expected, px_image_t* actual, int eps)
{
    if (expected == NULL && actual == NULL)
    {
        return true;
    }
    else if ( (expected == NULL && actual != NULL) || (expected != NULL && actual == NULL) )
    {
        return false;
    }
    else if (expected->height != actual->height)
    {
        PX_LOGE("height not match: expected(%d) != actual(%d)\n", expected->height, actual->height);
        return false;
    }
    else if (expected->width != actual->width)
    {
        PX_LOGE("width not match: expected(%d) != actual(%d)\n", expected->width, actual->width);
        return false;
    }
    else if (expected->channel != actual->channel)
    {
        PX_LOGE("channel not match: expected(%d) != actual(%d)\n", expected->channel, actual->channel);
        return false;
    }
    const int h = expected->height;
    const int cn = expected->channel;
    const int rowlen = expected->width * cn;
    for (int i = 0; i < h; i++)
    {
        uint8_t* sp = expected->data + i * expected->stride;
        uint8_t* dp = actual->data + i * actual->stride;
        for (int j = 0; j < rowlen; j++)
        {
            const int diff = abs(sp[i] - dp[i]);
            if (diff > eps)
            {
                const int x = i / cn;
                const int c = i % cn;
                PX_LOGE("actual[%d,%d,%d] (%d) != expected[%d,%d,%d] (%d), diff = %d, EPS = %d\n",
                    i, x, c, dp[j],
                    i, x, c, sp[j],
                    diff, eps
                );

                return false;
            }
        }
    }
    return true;
}

bool px_array_almost_equal(px_array_t* expected, px_array_t* actual, float eps)
{
    if (expected == NULL && actual == NULL)
    {
        return true;
    }
    else if ( (expected == NULL && actual != NULL) || (expected != NULL && actual == NULL) )
    {
        return false;
    }
    else if (expected->len != actual->len)
    {
        PX_LOGE("len not match: expected(%d) != actual(%d)\n", expected->len, actual->len);
        return false;
    }
    const int len = expected->len;
    for (int i = 0; i < len; i++)
    {
        const float diff = fabs(expected->data[i] - actual->data[i]);
        if (diff > eps)
        {
            PX_LOGE("actual[%d] (%f) != expected[%d] (%f), diff = %f, EPS = %f\n",
                i, actual->data[i],
                i, expected->data[i],
                diff, eps
            );

            return false;
        }
    }

    return true;
}