#include "zcnn_image.h"
#include <stdlib.h>

image_t* make_image(image_dim_t dim)
{
    image_t* image = (image_t*)malloc(sizeof(image_t));
    image->height = dim.height;
    image->width = dim.width;
    image->channels = dim.channels;
    size_t buf_sz = dim.height * dim.width * dim.channels;
    image->data = (uchar*)malloc(buf_sz);
    return image;
}

image_dim_t get_image_dim(image_t* im) {
    image_dim_t dim;
    dim.width = im->width;
    dim.height = im->height;
    dim.channels = im->channels;
    return dim;
}