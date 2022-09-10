#pragma once

#include "px_image.h"
#include "px_cnn.h"

#ifdef __cplusplus
extern "C" {
#endif

bool px_image_almost_equal(px_image_t* expected, px_image_t* actual, int eps);

bool px_array_almost_equal(px_array_t* expected, px_array_t* actual, float eps);
bool px_matrix_almost_equal(px_matrix_t* expected, px_matrix_t* actual, float eps);

bool px_image_equal_in_shape(px_image_t* expected, px_image_t* actual, bool compare_stride);
bool px_image_equal_in_size(px_image_t* expected, px_image_t* actual);
bool px_size_equal(px_size_t expected, px_size_t actual);
px_size_t px_get_image_size(px_image_t* image);

bool px_image_size_equal(px_image_t* image, px_size_t expected_size);

void px_dump_matrix(const px_matrix_t* matrix);
void px_dump_cube(const px_cube_t* cube);

void px_dump_image_meta(const px_image_t* image, const char* msg);

#ifdef __cplusplus
}
#endif