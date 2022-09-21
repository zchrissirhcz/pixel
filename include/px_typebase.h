#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct px_size_t
{
    int width;
    int height;
} px_size_t;

typedef struct px_pad_t
{
    int top;
    int bottom;
    int left;
    int right;
} px_pad_t;

#ifdef __cplusplus
extern "C" {
#endif

px_size_t px_create_size(int height, int width);

#ifdef __cplusplus
}
#endif