#include "px_image.h"
#include "px_assert.h"
#include "px_arithm.h"
#include "px_compare.h"
#include <math.h>

void px_resize_linear(px_image_t* src, px_image_t* dst, px_size_t dsize)
{
    PX_ASSERT(src != NULL && dst != NULL);
    PX_ASSERT(px_image_size_equal(dst, dsize));
    PX_ASSERT(src->channel == dst->channel);

    const int channel = dst->channel;

    double scale_height = src->height * 1.0 / dst->height;
    double scale_width = src->width * 1.0 / dst->width;

    int Ai, Aj, Bi, Bj, Ci, Cj, Di, Dj;
    for (int i = 0; i < dst->height; i++)
    {
        for (int j = 0; j < dst->width; j++)
        {
            int di = i;
            int dj = j;

            // interpolation: + 0.5 then - 0.5: align to geometric center
            double si = (di + 0.5) * scale_height - 0.5;
            double sj = (dj + 0.5) * scale_width - 0.5;
            double y = si - (int)si;
            double x = sj - (int)sj;

            //   |-- x --|-- 1-x --|
            // - A       |         B
            // |         |
            // |         |
            // y         |
            // |         |
            // |         |
            // ------(si, sj)
            // |
            // |
            //1-y
            // |
            // |
            // - C                 D

            Ai = floor(si);
            Aj = floor(sj);

            Bi = floor(si);
            Bj = ceil(sj);

            Ci = ceil(si);
            Cj = floor(sj);

            Di = ceil(si);
            Dj = ceil(sj);

            for (int k = 0; k < channel; k++)
            {
                uint8_t A = px_get_pixel(src, Ai, Aj, k);
                uint8_t B = px_get_pixel(src, Bi, Bj, k);
                uint8_t C = px_get_pixel(src, Ci, Cj, k);
                uint8_t D = px_get_pixel(src, Di, Dj, k);

                double coeffA = (1 - x) * (1 - y);
                double coeffB = x * (1 - y);
                double coeffC = (1 - x) * y;
                double coeffD = x * y;
                double M = coeffA * A + coeffB * B + coeffC * C + coeffD * D;

                const int dst_idx = di * dst->stride + dj * channel + k;
                dst->data[dst_idx] = (uint8_t)(M + 0.5); // rounding
            }
        }
    }
}