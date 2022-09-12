#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void px_read_bmp(const char* filepath, int line_align, int* _h, int* _w, int* _c, unsigned char** _buffer, bool swap_bgr);
void px_write_bmp(const char* filepath, int h, int w, int c, const unsigned char* buf, int read_linebytes, bool swap_bgr);


#ifdef __cplusplus
}
#endif
