#pragma once

#include <stdlib.h>
#include <string.h>

#include "px_image.h"

// draw text
// Now only support 0-9 a-z and space (' ')
void put_text(px_image_t* im, px_point_t org, px_color_t color, int font_size, const char* text);

void imshow(const char* winname, const px_image_t* im);
void waitkey(int milli_secs);