#include "zcnn_convert_color.h"

void convert_bgr_to_rgb(image_t* bgr, image_t* rgb)
{
    assert(bgr!=NULL && rgb!=NULL);
    assert(bgr->data!=NULL && rgb->data!=NULL);
    assert(bgr->height==rgb->height && bgr->width==rgb->width);
    assert(bgr->channels==3 && rgb->channels==3);

    const int height = bgr->height;
    const int width = bgr->width;
    uchar* src = bgr->data;
    uchar* dst = rgb->data;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            const int idx = i * width * 3 + j * 3;
            dst[idx + 0] = src[idx + 2];
            dst[idx + 1] = src[idx + 1];
            dst[idx + 2] = src[idx + 0];

            // im->data[idx] ^= im->data[idx + 2] ^= im->data[idx] ^= im->data[idx + 2];
        }
    }
}
