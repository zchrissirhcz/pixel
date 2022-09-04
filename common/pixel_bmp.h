#pragma once

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
void pxl_decode_bmp(const char* fn, int line_align, int* _h, int* _w, int* _c, unsigned char** _buffer, bool swap_bgr);

// encode .bmp image file
void pxl_encode_bmp(const char* fn, int h, int w, int c, const unsigned char* buf, int read_linebytes, bool swap_bgr);

#ifdef __cplusplus
}
#endif
