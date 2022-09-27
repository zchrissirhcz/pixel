#pragma once

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void px_read_png(const char* filename, unsigned char** data, size_t* w, size_t* h);
void px_write_png(const char* filename, unsigned char* data, size_t w, size_t h ,size_t bitdepth);

#ifdef __cplusplus
}
#endif