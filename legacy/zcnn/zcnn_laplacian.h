#pragma once

#include "zcnn_image.h"

#ifdef __cplusplus
extern "C"{
#endif

int test_laplacian_filter();

void convolve(image_t* In, image_t* Mask, image_t* Out);

#ifdef __cplusplus
}
#endif
