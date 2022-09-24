#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 3 channel image
uint8_t* px_read_ppm(const char* filename, int* height, int* width);
void px_write_ppm(const char* filename, uint8_t* data, int height, int width);

// 1 channel image
uint8_t* px_read_pgm(const char* filename, int* height, int* width);
void px_write_pgm(const char* filename, uint8_t* data, int height, int width);

#ifdef __cplusplus
}
#endif