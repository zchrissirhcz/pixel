#ifndef PIXEL_MATCALC_ARRAY_MEAN_H
#define PIXEL_MATCALC_ARRAY_MEAN_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

float array_mean_u8_naive(unsigned char* data, size_t len);
float array_mean_u8_asimd1(unsigned char* data, size_t len);
float array_mean_u8_asimd2(unsigned char* data, size_t len);
float array_mean_u8_asimd3(unsigned char* data, size_t len);
float array_mean_u8_asimd4(unsigned char* data, size_t len);
float array_mean_u8_asimd5(unsigned char* data, size_t len);
float array_mean_u8_asimd6(unsigned char* data, size_t len);

float array_mean_s8_naive(int8_t* data, size_t len);
float array_mean_s8_asimd(int8_t* data, size_t len);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_ARRAY_MEAN_H