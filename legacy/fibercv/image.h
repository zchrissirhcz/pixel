#pragma once

#include <stdlib.h>
#include <string.h>

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

// image resize, support nearest/bilinear
void resize(Image* im_src, Image* im_dst, ResizeMethod method=kNEAREST);

// create histgram
Image* histgram(Image* im);

