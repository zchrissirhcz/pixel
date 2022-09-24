#ifndef FIBERCV_IMAGEIO_H
#define FIBERCV_IMAGEIO_H

#include "image.h"

namespace fc {

// load an image as BGR order, like opencv
// supported format: .bmp, .jpg, .png, .ppm, .pgm
Image* imread(const char* filename);

// save an BGR ordered image, like opencv
// supported format: .bmp, .jpg, .png, .ppm, .pgm
void imwrite(const char* save_name, Image* im);

} // namespace fc

#endif