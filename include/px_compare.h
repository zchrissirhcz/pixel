#pragma once

#include "px_image.h"
#include "px_cnn.h"

#ifdef __cplusplus
extern "C" {
#endif

bool px_image_almost_equal(px_image_t* expected, px_image_t* actual, int eps);

bool px_array_almost_equal(px_array_t* expected, px_array_t* actual, float eps);

#ifdef __cplusplus
}
#endif