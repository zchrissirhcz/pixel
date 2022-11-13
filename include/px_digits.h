#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @param digits at least 8 elements
void px_decimal_to_binary_s8(int8_t x, int* digits);

int8_t px_binary_to_decimal_s8(int* bits);

void px_reverse_bits(int* digits, int len);

#ifdef __cplusplus
}
#endif