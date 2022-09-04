#include "px_image.h"
#include <stdlib.h>
#include <string.h>

px_image_t* px_create_image(int h, int w, int c)
{
    px_image_t* image = (px_image_t*) malloc(sizeof(px_image_t));
    memset(image, 0, sizeof(px_image_t));
    image->height = h;
    image->width = w;
    image->channel = c;
    image->stride = w * c;

    const int bufsize = sizeof(uint8_t) * image->stride;
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