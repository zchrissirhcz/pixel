#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "zcnn_image.h"

typedef struct FcBGR {
    unsigned char b, g, r;
} FcBGR;

#ifdef __cplusplus
extern "C" {
#endif

    // interpolation with nearest neighbor method
    void bgr_resize_nearest(const image_t* src, image_t* dst);

    // interpolation with bi-linear method
    void bgr_resize_bilinear(const image_t* src, image_t* dst);

    // interpolation with bi-cubic method
    void bgr_resize_bicubic(const image_t* src, image_t* dst);

    // interpolation with area method
    void bgr_resize_area(const image_t* src, image_t* dst);

    // get bgr pixel
    inline FcBGR* bgr_pixels(const image_t* im, const int x, const int y);

#ifdef __cplusplus
}
#endif