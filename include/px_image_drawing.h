#pragma once

#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

void px_draw_circle(px_image_t* im, const px_point_t* center, int radius, px_color_t color);
void px_draw_rectangle(px_image_t* im, px_rect_t r, px_color_t color, int thickness);

#ifdef __cplusplus
}
#endif