#pragma once

#include <stdlib.h>
#include <string.h>

#include "px_image.h"

typedef enum ResizeMethod
{
    kNEAREST=0,
    kBILINEAR=1
} ResizeMethod;


// create histogram
px_image_t* histogram(px_image_t* im);

// draw text
// Now only support 0-9 a-z and space (' ')
void put_text(px_image_t* im, px_point_t org, px_color_t color, int font_size, const char* text);


#ifdef FIBERCV_IMSHOW
#include "imshow.h"
#endif
