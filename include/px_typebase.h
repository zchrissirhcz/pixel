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

typedef struct px_point_t
{
    int x;
    int y;
} px_point_t;

typedef struct px_pointf_t
{
    float x;
    float y;
} px_pointf_t;

typedef struct px_rect_t
{
    int x1;
    int y1;
    int x2;
    int y2;
} px_rect_t;

typedef struct px_color_t
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
} px_color_t;

#ifdef __cplusplus
extern "C" {
#endif

px_size_t px_create_size(int height, int width);

#ifdef __cplusplus
}
#endif