#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int px_clamp(int x, int minval, int maxval);

int px_align_up(int x, int power_of_2);

float px_fast_sqrt(float num);

#ifdef __cplusplus
}
#endif