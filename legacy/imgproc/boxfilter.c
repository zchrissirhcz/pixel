#include "boxfilter.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void boxfilter_naive(unsigned char* src, unsigned char* dst, int height, int width, int channels, int kernel_h, int kernel_w, int anchor_y, int anchor_x, bool norm, BorderType border_type)
{
    // param checking
    assert(border_type==PX_BORDER_REPLICATE ||
           border_type==PX_BORDER_REFLECT ||
           border_type==PX_BORDER_REFLECT101);
    assert(kernel_h>0 && kernel_w>0);

    int kernel_size = kernel_h * kernel_w;
    int* kernel = (int*)malloc(sizeof(int)*kernel_size);
    for (int i=0; i<kernel_size; i++) {
        kernel[i] = 1;
    }

    int denominator = 1;
    if (norm) {
        denominator = kernel_size;
    }
    int radius = kernel_size / 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel[3] = { 0 };
            for (int ki = 0; ki < kernel_h; ki++) {
                for (int kj = 0; kj < kernel_w; kj++) {
                    int ti = (i + ki - anchor_y);
                    int tj = (j + kj - anchor_x);
                    ti = border_clip(border_type, ti, height);
                    tj = border_clip(border_type, tj, width);
                    if (channels==3) {
                        pixel[0] += src[ti * width * 3 + tj * 3];
                        pixel[1] += src[ti * width * 3 + tj * 3 + 1];
                        pixel[2] += src[ti * width * 3 + tj * 3 + 2];
                    }
                    else if (channels == 1) {
                        pixel[0] += src[ti * width + tj];
                    }
                }
            }
            if (channels == 3) {
                pixel[0] = (pixel[0]+radius) / denominator;
                pixel[1] = (pixel[1]+radius) / denominator;
                pixel[2] = (pixel[2]+radius) / denominator;
                if (pixel[0] > 255) pixel[0] = 255;
                if (pixel[1] > 255) pixel[1] = 255;
                if (pixel[2] > 255) pixel[2] = 255;
                dst[i * width * 3 + j * 3] = pixel[0];
                dst[i * width * 3 + j * 3 + 1] = pixel[1];
                dst[i * width * 3 + j * 3 + 2] = pixel[2];
            }
            else if (channels == 1) {
                pixel[0] = (pixel[0] + radius) / denominator;
                if (pixel[0] > 255) pixel[0] = 255;
                dst[i * width + j] = pixel[0];
            }
        }
    }
    free(kernel);
}
