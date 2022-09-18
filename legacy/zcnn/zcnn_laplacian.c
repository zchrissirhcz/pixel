#include "zcnn_laplacian.h"
#include "zcnn_image_io.h"
#include <stdlib.h>

/* 2-D Discreate Convolution */

void convolve(image_t* input, image_t* mask, image_t* output)
{
    int i, j, m, n, idx, jdx;
    int ms, im, val;
    unsigned char* tmp;

    /* the outer summation loop */
    for (i = 0; i < input->height; i++) {
        for (j = 0; j < input->width; j++) {
            val = 0;
            for (m = 0; m < mask->height; m++) {
                for (n = 0; n < mask->width; n++) {
                    ms = (char)*(mask->data + m * mask->width + n);
                    idx = i - m;
                    jdx = j - n;
                    if (idx >= 0 && jdx >= 0) {
                        im = *(input->data + idx * input->width + jdx);
                    }
                    val += ms * im;
                }
            }
            if (val > 255) val = 255;
            if (val < 0) val = 0;
            tmp = output->data + i * output->height + j;
            *tmp = (unsigned char)val;
        }
    }
}

int test_laplacian_filter()
{
    image_t* input = load_image("lena-gray-400.bmp");

    uchar* tmp;
    int i;

    image_dim_t image_dim = get_image_dim(input);
    image_t* output = make_image(image_dim);

    // Set up a 5x5 Mask
    image_dim_t mask_dim;
    mask_dim.height = 5;
    mask_dim.width = 5;
    mask_dim.channels = 1;
    image_t* mask = make_image(mask_dim);

    /* Init the 5x5 Mask image as a
    Laplacian, which looks like:
        -1 -1 -1 -1 -1
        -1 -1 -1 -1 -1
        -1 -1 24 -1 -1
        -1 -1 -1 -1 -1
        -1 -1 -1 -1 -1
    */
    /* set all mask values to -1 */
    tmp = mask->data;
    for (i = 0; i < 25; ++i) {
        *tmp = -1;
        ++tmp;
    }

    /* now fix the middle one */
    tmp = mask->data + 13;
    *tmp = 24;

    /* Now do the processing */
    convolve(input, mask, output);

    save_image(output, "lena-gray-laplacian-filter-result.bmp");

    // clean up
    free(input->data);
    free(mask->data);
    free(output->data);
    free(input);
    free(mask);
    free(output);

    return 0;
}