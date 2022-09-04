#pragma once

#include "px_typebase.h"

typedef struct px_image_t
{
    uint8_t* data;
    int width;
    int height;
    int channel;
    int stride;
} px_image_t;

typedef struct px_size_t
{
    int width;
    int height;
} px_size_t;

#ifdef __cplusplus
extern "C" {
#endif

px_image_t* px_create_image(int h, int w, int c);
void px_destroy_image(px_image_t* image);

void px_rgb2bgr(px_image_t* src, px_image_t* dst);
void px_rgb2gray(px_image_t* src, px_image_t* dst);

// flipmode
// input     vertical    horizontal    both
// A  B      C  D        B  A          D  C
// C  D      A  B        D  C          B  A
enum
{
    PX_FLIP_VERTICAL = 0,
    PX_FLIP_HORIZONTAL = 1,
    PX_FLIP_BOTH = 2
};
void px_flip(px_image_t* src, px_image_t* dst, int flipmode);

void px_resize_nearest(px_image_t* src, px_image_t* dst, px_size_t dsize);
void px_resize_linear(px_image_t* src, px_image_t* dst, px_size_t dsize);
void px_resize_cubic(px_image_t* src, px_image_t* dst, px_size_t dsize);

#ifdef __cplusplus
}
#endif