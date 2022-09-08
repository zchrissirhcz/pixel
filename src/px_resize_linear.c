#include "px_image.h"
#include "px_assert.h"
#include "px_arithm.h"
#include <math.h>

void px_resize_linear(px_image_t* src, px_image_t* dst, px_size_t dsize)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_size_equal(dsize, dst->size));
    PX_ASSERT(src->channel == dst->channel);

    const int height = dst->height;
    const int width = dst->width;
    const int channel = dst->channel;

    double scale_height = src->height * 1.0 / dst->height;
    double scale_width = src->width * 1.0 / dst->width;

    int Ai, Aj, Bi, Bj, Ci, Cj, Di, Dj;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int dst_i = i;
            int dst_j = j;

            // + 0.5 then - 0.5: align to geometric center
            double src_i = (dst_i + 0.5) * scale_height - 0.5;
            double src_j = (dst_j + 0.5) * scale_width - 0.5;
            double y = src_i - (int)src_i;
            double x = src_j - (int)src_j;

            Ai = floor(src_i);
            Aj = floor(src_j);

            Bi = floor(src_i);
            Bj = ceil(src_j);

            Ci = ceil(src_i);
            Cj = floor(src_j);

            Di = ceil(src_i);
            Dj = ceil(src_j);

            for (int k = 0; k < channel; k++)
            {
                double A = px_get_pixel(src, Ai, Aj, k);
                double B = px_get_pixel(src, Bi, Bj, k);
                double C = px_get_pixel(src, Ci, Cj, k);
                double D = px_get_pixel(src, Di, Dj, k);

                double M = (1 - x) * (1 - y) * A + x * (1 - y) * B + y * (1 - x) * C + x * y * D;
                const int dst_idx = dst_i * dst->stride + dst_j * channel + k;
                dst->data[dst_idx] = (uint8_t)(M + 0.5); // rounding
            }
        }
    }
}