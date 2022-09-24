#include "px_image.h"
#include "px_cnn.h"
#include "px_compare.h"
#include "px_log.h"
#include "px_assert.h"
#include <math.h>
#include <stdlib.h>

bool px_image_almost_equal(px_image_t* expected, px_image_t* actual, int eps)
{
    if (expected == NULL && actual == NULL)
    {
        return true;
    }
    else if ((expected == NULL && actual != NULL) || (expected != NULL && actual == NULL))
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
            const int diff = abs(sp[j] - dp[j]);
            if (diff > eps)
            {
                const int x = i / cn;
                const int c = i % cn;
                PX_LOGE("actual[%d,%d,%d] (%d) != expected[%d,%d,%d] (%d), diff = %d, EPS = %d\n",
                        i, x, c, dp[j],
                        i, x, c, sp[j],
                        diff, eps);

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
    else if ((expected == NULL && actual != NULL) || (expected != NULL && actual == NULL))
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
                    diff, eps);

            return false;
        }
    }

    return true;
}

bool px_matrix_almost_equal(px_matrix_t* expected, px_matrix_t* actual, float eps)
{
    if (expected == NULL && actual == NULL)
    {
        return true;
    }
    else if ((expected == NULL && actual != NULL) || (expected != NULL && actual == NULL))
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

    const int h = expected->height;
    const int w = expected->width;
    for (int i = 0; i < h; i++)
    {
        float* sp = expected->data + i * expected->width;
        float* dp = actual->data + i * actual->width;
        for (int j = 0; j < w; j++)
        {
            const float diff = fabs(sp[j] - dp[j]);
            if (diff > eps)
            {
                PX_LOGE("actual[%d,%d] (%f) != expected[%d,%d] (%f), diff = %f, EPS = %f\n",
                        i, j, dp[j],
                        i, j, sp[j],
                        diff, eps);

                return false;
            }
        }
    }
    return true;
}

void px_dump_matrix(const px_matrix_t* matrix)
{
    for (int i = 0; i < matrix->height; i++)
    {
        for (int j = 0; j < matrix->width; j++)
        {
            int idx = i * matrix->width + j;
            PX_LOGE("%f, ", matrix->data[idx]);
        }
        PX_LOGE("\n");
    }
}

void px_dump_cube(const px_cube_t* cube)
{
    PX_LOGE(">>> cube.dims: h=%d, w=%d, c=%d\n", cube->height, cube->width, cube->channel);
    for (int k = 0; k < cube->channel; k++)
    {
        for (int i = 0; i < cube->height; i++)
        {
            for (int j = 0; j < cube->width; j++)
            {
                int idx = k * cube->width * cube->height + i * cube->width + j;
                PX_LOGE("%f, ", cube->data[idx]);
            }
            PX_LOGE("\n");
        }
        PX_LOGE("\n----------\n");
    }
    PX_LOGE("\n");
}

bool px_image_equal_in_shape(px_image_t* expected, px_image_t* actual, bool compare_stride)
{
    PX_ASSERT(expected != NULL && actual != NULL);
    if (expected->height != actual->height)
    {
        return false;
    }
    else if (expected->width != actual->width)
    {
        return false;
    }
    else if (expected->channel != actual->channel)
    {
        return false;
    }
    else if (compare_stride && expected->stride != actual->stride)
    {
        return false;
    }
    return true;
}

bool px_size_equal(px_size_t expected, px_size_t actual)
{
    return (expected.width == actual.width) && (expected.height == actual.height);
}

px_size_t px_get_image_size(px_image_t* image)
{
    px_size_t size;
    size.height = image->height;
    size.width = image->width;
    return size;
}

bool px_image_size_equal(px_image_t* image, px_size_t expected_size)
{
    px_size_t image_size = px_get_image_size(image);
    return px_size_equal(expected_size, image_size);
}

void px_dump_image_meta(const px_image_t* image, const char* msg)
{
    if (msg)
    {
        PX_LOGE("%s:", msg);
    }
    PX_LOGE("width=%d, height=%d, channel=%d, data=%p, stride=%d",
            image->width, image->height, image->channel, image->data, image->stride);
}

bool px_image_equal_in_size(px_image_t* expected, px_image_t* actual)
{
    px_size_t sz_expected = px_get_image_size(expected);
    px_size_t sz_actual = px_get_image_size(actual);
    return px_size_equal(sz_expected, sz_actual);
}

bool px_is_valid_rgb_image(px_image_t* image)
{
    return (image != NULL && image->data != NULL && image->channel == 3);
}

bool px_is_valid_rgba_image(px_image_t* image)
{
    return (image != NULL && image->data != NULL && image->channel == 4);
}

bool px_is_valid_gray_image(px_image_t* image)
{
    return (image != NULL && image->data != NULL && image->channel == 1);
}

bool px_is_valid_yuv420sp_image_pair(px_image_t* y_plane, px_image_t* uv_plane)
{
    if (y_plane == NULL || y_plane->data == NULL)
    {
        return false;
    }
    else if (uv_plane == NULL || uv_plane->data == NULL)
    {
        return false;
    }
    else if (y_plane->height != 2 * uv_plane->height)
    {
        return false;
    }
    else if (y_plane->width != 2 * uv_plane->width)
    {
        return false;
    }
    else if (y_plane->channel != 1)
    {
        return false;
    }
    else if (uv_plane->channel != 2)
    {
        return false;
    }
    return true;
}