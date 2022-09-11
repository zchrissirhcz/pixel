#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// load an bmp image (BGR order)
void nc_load_bmp(const char* filename, uint8_t** _buf, int* _height, int* _width, int channel, bool line_align);

// save an bmp image
void nc_save_bmp(const char* filename, const uint8_t* buf, int height, int width, int channel);

#ifdef __cplusplus
}
#endif
