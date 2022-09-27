#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

int px_write_png(const char* filepath, int height, int width, int channel, unsigned char* buffer);
void px_read_png(const char* filepath, int* height, int* width, int* channel, unsigned char** buffer);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <vector>

int px_write_png_custom(const char* filepath, int height, int width, int channels, unsigned char* buffer, const std::vector<int>& params={});

#endif // __cplusplus

