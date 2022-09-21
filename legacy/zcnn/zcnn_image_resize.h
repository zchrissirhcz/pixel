#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

// interpolation with nearest neighbor method
void bgr_resize_nearest(const px_image_t* src, px_image_t* dst);

#ifdef __cplusplus
}
#endif