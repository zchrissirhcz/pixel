#ifndef PIXEL_MATCALC_ARRAY_FILL_H
#define PIXEL_MATCALC_ARRAY_FILL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void array_fill_f32_asimd(float* data, size_t len, float value);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_MATCALC_ARRAY_FILL_H