#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// decode .jpg image file
void pxl_decode_jpg(const char* fn, int line_align, int* _h, int* _w, int* _c, unsigned char** _buffer, bool swap_bgr);

// encode .jpg image file
void pxl_encode_jpg(const char* fn, int h, int w, int c, const unsigned char* buf, int read_linebytes, bool swap_bgr);


#ifdef __cplusplus
}
#endif
