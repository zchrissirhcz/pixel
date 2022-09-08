#include "px_image.h"
#include <stdlib.h>
#include <string.h>

px_image_t* px_create_image_header(int height, int width, int channel)
{
    px_image_t* image = (px_image_t*) malloc(sizeof(px_image_t));
    memset(image, 0, sizeof(px_image_t));
    image->height = height;
    image->width = width;
    image->channel = channel;
    image->stride = width * channel;
    return image;
}

px_image_t* px_create_image(int height, int width, int channel)
{
    px_image_t* image = image = px_create_image_header(height, width, channel);
    const int bufsize = sizeof(uint8_t) * image->stride * image->height;
    image->data = (uint8_t*) malloc(bufsize);

    return image;
}

void px_destroy_image(px_image_t* image)
{
    if (image)
    {
        if (image->data)
        {
            free(image->data);
            image->data = NULL;
        }
        free(image);
    }
}

void px_destroy_image_header(px_image_t* image)
{
    if (image)
    {
        free(image);
    }
}

uint8_t px_get_pixel(px_image_t* image, int i, int j, int k)
{
    const int idx = i * image->stride + j * image->channel + k;
    return image->data[idx];
}

void px_set_pixel(px_image_t* image, int i, int j, int k, uint8_t value)
{
    const int idx = i * image->stride + j * image->channel + k;
    image->data[idx] = value;
}