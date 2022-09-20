#include "zcnn_laplacian.h"
#include <stdlib.h>
#include "px_image_io.h"

/* 2-D Discreate Convolution */

void convolve(px_image_t* input, px_image_t* mask, px_image_t* output)
{
    int i, j, m, n, idx, jdx;
    int ms, im, val;
    unsigned char* tmp;

    /* the outer summation loop */
    for (i = 0; i < input->height; i++)
    {
        for (j = 0; j < input->width; j++)
        {
            val = 0;
            for (m = 0; m < mask->height; m++)
            {
                for (n = 0; n < mask->width; n++)
                {
                    ms = (char)*(mask->data + m * mask->width + n);
                    idx = i - m;
                    jdx = j - n;
                    if (idx >= 0 && jdx >= 0)
                    {
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
    px_image_t* input = px_read_image("lena-gray-400.bmp");

    uint8_t* tmp;
    px_image_t* output = px_create_image(input->height, input->width, input->channel);

    // Set up a 5x5 Mask
    const int mask_height = 5;
    const int mask_width = 5;
    const int mask_channels = 1;
    px_image_t* mask = px_create_image(mask_height, mask_width, mask_channels);

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
    for (int i = 0; i < 25; ++i)
    {
        *tmp = -1;
        ++tmp;
    }

    /* now fix the middle one */
    tmp = mask->data + 13;
    *tmp = 24;

    /* Now do the processing */
    convolve(input, mask, output);

    px_write_image(output, "lena-gray-laplacian-filter-result.bmp");

    // clean up
    px_destroy_image(input);
    px_destroy_image(mask);
    px_destroy_image(output);

    return 0;
}