#pragma once

#include "px_image.h"
#include "px_cnn.h"

#ifdef __cplusplus
extern "C" {
#endif

bool px_image_almost_equal(px_image_t* expected, px_image_t* actual, int eps);

bool px_array_almost_equal(px_array_t* expected, px_array_t* actual, float eps);
bool px_matrix_almost_equal(px_matrix_t* expected, px_matrix_t* actual, float eps);

void px_dump_matrix(const px_matrix_t* matrix);
void px_dump_cube(const px_cube_t* cube);

#ifdef __cplusplus
}
#endif