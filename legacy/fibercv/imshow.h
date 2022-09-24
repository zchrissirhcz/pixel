#ifndef FIBERCV_IMSHOW_H
#define FIBERCV_IMSHOW_H

#include "image.h"

namespace fc {

void imshow(const char* winname, const Image* im);

void waitkey(int milli_secs);

} // namespace fc

#endif
