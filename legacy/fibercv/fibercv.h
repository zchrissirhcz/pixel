#pragma once

#include <stdlib.h>
#include <string.h>

#include "px_image.h"

typedef enum ResizeMethod
{
    kNEAREST=0,
    kBILINEAR=1
} ResizeMethod;

typedef struct Point
{
    int x;
    int y;
} Point;

typedef struct Color
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Color;

typedef struct Rect
{
    int x1;
    int y1;
    int x2;
    int y2;
} Rect;

// create histogram
px_image_t* histogram(px_image_t* im);

// drawing
// draw (solid) circle on image
void circle(px_image_t* im, const Point* center, int radius, Color color);

// draw rectangle on image
void rectangle(px_image_t* im, Rect r, Color color, int thickness=1);

// draw text
// Now only support 0-9 a-z and space (' ')
void put_text(px_image_t* im, Point org, Color color, int font_size, const char* text);


#ifdef FIBERCV_IMSHOW
#include "imshow.h"
#endif
