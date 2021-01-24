#ifndef PIXEL_BMP_H
#define PIXEL_BMP_H

#include <stdint.h>
#include <stdbool.h>
#include "pixel_platform.h"

typedef enum _PxlImageFormat {
    _PxlImage_BGR,
    _PxlImage_RGB,
    _PxlImage_GRAY,
} _PxlImageFormat;

#ifdef __cplusplus
extern "C" {
#endif

// decode .bmp image file
void _pxl_decode_bmp(const char* fn, int line_align, int* _h, int* _w, int* _c, unsigned char** _buffer, bool swap_bgr);

#ifdef __cplusplus
}
#endif

#endif