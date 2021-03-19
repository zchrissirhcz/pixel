#ifndef PIXEL_MATCALC_ARRAY_MEAN_H
#define PIXEL_MATCALC_ARRAY_MEAN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

float array_mean_naive(unsigned char* data, size_t len);

float array_mean_asimd(unsigned char* data, size_t len);

float array_mean_asimd2(unsigned char* data, size_t len);

float array_mean_asimd3(unsigned char* data, size_t len);

float array_mean_asimd4(unsigned char* data, size_t len);

float array_mean_asimd5(unsigned char* data, size_t len);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_ARRAY_MEAN_H