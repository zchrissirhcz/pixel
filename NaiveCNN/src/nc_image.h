#pragma once

#include <stdbool.h>

#include "naive_cnn.h"

#ifdef __cplusplus
extern "C" {
#endif

// load an bmp image (BGR order)
void nc_load_bmp(const char* filename, uint8_t** _buf, uint32_t* _height, uint32_t* _width, uint32_t channel, bool line_align);

// save an bmp image
void nc_save_bmp(const char* filename, const uint8_t* buf, uint32_t height, uint32_t width, uint32_t channel);

void nc_swap_rgb_and_bgr_inplace(NcImage* im);

#ifdef __cplusplus
}
#endif
