#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// load image in BGR order if channel == 3
void px_read_bmp(const char* filepath, int* _height, int* _width, int* _channel, unsigned char** _buffer);

// save image in BGR order if channel == 3
void px_write_bmp(const char* filepath, int height, int width, int channel, const unsigned char* buffer);

void px_read_bmp_custom(const char* filepath, int* _height, int* _width, int* _channel, unsigned char** _buffer, int line_align, bool swap_bgr);
void px_write_bmp_custom(const char* filepath, int height, int width, int channel, const unsigned char* buf, int read_linebytes, bool swap_bgr);


#ifdef __cplusplus
}
#endif
