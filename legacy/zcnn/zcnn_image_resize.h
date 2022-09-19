#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "px_image.h"

typedef struct FcBGR {
    unsigned char b, g, r;
} FcBGR;

#ifdef __cplusplus
extern "C" {
#endif

// interpolation with nearest neighbor method
void bgr_resize_nearest(const px_image_t* src, px_image_t* dst);

// get bgr pixel
inline FcBGR* bgr_pixels(const px_image_t* im, const int x, const int y);

#ifdef __cplusplus
}
#endif