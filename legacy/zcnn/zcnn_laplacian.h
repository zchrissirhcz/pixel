#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C"{
#endif

int test_laplacian_filter();

void convolve(px_image_t* In, px_image_t* Mask, px_image_t* Out);

#ifdef __cplusplus
}
#endif
