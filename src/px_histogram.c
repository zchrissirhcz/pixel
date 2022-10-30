#include "px_image.h"
#include "px_log.h"
#include "px_assert.h"
#include "px_compare.h"
#include <stdlib.h>
#include <string.h>

static px_image_t* histogram_lut_to_image(int gray_lut[256])
{
    int max_gray = 0;
    for(int i = 0; i < 256; i++)
    {
        if (gray_lut[i] > max_gray)
        {
            max_gray = gray_lut[i];
        }
    }

    int width = 256;
    int height = 100;
    int channel = 1;
    px_image_t* hist = px_create_image(height, width, channel);
    memset(hist->data, 255, hist->height * hist->stride);

    int linebytes = hist->stride;
    for(int i = 0; i < hist->width; i++)
    {
        int sum = gray_lut[i] * hist->height / max_gray;
        for(int j = 0; j < sum; j++)
        {
            int idx = (hist->height-1-j)*linebytes + i*hist->channel;
            hist->data[idx] = 128;
        }
    }

    return hist;
}

px_image_t* px_histogram_rgb(px_image_t* src, PX_HISTOGRAM_MODE mode)
{
    PX_ASSERT(px_is_valid_rgb_image(src));

    const int cn = 3;
    PX_ASSERT(src->channel == cn);

    const int height = src->height;
    const int width = src->width;

    int gray_lut[256] = {0};
    if (mode == PX_HISTOGRAM_GRAY)
    {
        for (int i = 0; i < height; i++)
        {
            uint8_t* ptr = src->data + i * src->stride;
            for (int j = 0; j < width; j++)
            {
                int gray = (ptr[0] + ptr[1] + ptr[2]) / 3;
                gray_lut[gray]++;
                ptr += cn;
            }
        }
    }
    else
    {
        int offset = 0;
        if (mode == PX_HISTOGRAM_R)
        {
            offset = 0;
        }
        else if (mode == PX_HISTOGRAM_G)
        {
            offset = 1;
        }
        else if (mode == PX_HISTOGRAM_B)
        {
            offset = 2;
        }
        else
        {
            PX_LOGE("Un-supported histogram mode: %d", mode);
            return NULL;
        }

        for (int i = 0; i < height; i++)
        {
            uint8_t* ptr = src->data + i * src->stride + offset;
            for (int j = 0; j < width; j++)
            {
                gray_lut[*ptr]++;
                ptr += cn;
            }
        }
    }

    px_image_t* hist = histogram_lut_to_image(gray_lut);
    return hist;
}

px_image_t* px_histogram(px_image_t* im)
{
    return px_histogram_rgb(im, PX_HISTOGRAM_GRAY);
}