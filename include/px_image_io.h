#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

/// supported formats: .bmp, .jpg, .png, .ppm, .pgm
px_image_t* px_read_image(const char* filename);

/// supported formats: .bmp, .jpg, .png, .ppm, .pgm
bool px_write_image(px_image_t* im, const char* filename);

#ifdef __cplusplus
}
#endif
