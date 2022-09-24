#ifndef FIBERCV_IMDRAW_H
#define FIBERCV_IMDRAW_H

#include "image.h"


namespace fc {

// draw (solid) circle on image
void circle(Image* im, const Point* center, int radius, Color color);

// draw rectangle on image
void rectangle(Image* im, Rect r, Color color, int thickness=1);

// draw text
// Now only support 0-9 a-z and space (' ')
void put_text(fc::Image* im, fc::Point org, fc::Color color, int font_size, const char* text);

} // namespace fc

#endif