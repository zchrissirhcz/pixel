#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

/// load RGB order image.
px_image_t* px_read_image(const char* filename);

/// only support .bmp, .jpg, .png
bool px_write_image(px_image_t* im, const char* filename);

#ifdef __cplusplus
}
#endif
