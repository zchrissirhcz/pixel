#pragma once

#include "zcnn_base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct image_t
{
    int height;
    int width;
    int channels;
    uchar* data;
} image_t;

typedef struct image_dim_t
{
    int height;
    int width;
    int channels;
} image_dim_t;

image_t* make_image(image_dim_t dim);

image_dim_t get_image_dim(image_t* im);

#ifdef __cplusplus
}
#endif