#include "px_image.h"
#include "px_log.h"
#include "px_assert.h"

void px_histogram_rgb(px_image_t* src, px_image_t* hist, PX_HISTOGRAM_MODE mode)
{
    const int cn = 3;
    PX_ASSERT(src->channel == cn);

    const int height = src->height;
    const int width = src->width;

    if (mode == PX_HISTOGRAM_GRAY)
    {
        for (int i = 0; i < height; i++)
        {
            uint8_t* ptr = src->data + i * src->stride;
            for (int j = 0; j < width; j++)
            {
                int gray = (ptr[0] + ptr[1] + ptr[2])/3;
                hist->data[gray]++;
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
            PX_LOGE("not supported histogram mode");
        }

        for (int i = 0; i < height; i++)
        {
            uint8_t* ptr = src->data + i * src->stride + offset;
            for (int j = 0; j < width; j++)
            {
                hist->data[*ptr]++;
                ptr += cn;
            }
        }
    }
}
