#pragma once

#include <stdbool.h>
#include "nc_types.h"

typedef struct NcImage {
    uint h, w, c;
    uint linebytes; // w*c, or alignUp(w*c, 4)
    uint mass; //linebytes * h
    uchar* data;
} NcImage;

#ifdef __cplusplus
extern "C" {
#endif

// make an empty image, i.e. height/width/channel/data==0
NcImage* nc_make_empty_image();

// make an image header, i.e. data will be NULL
NcImage* nc_make_image_header(uint h, uint w, uint c);

// make an image, whose data will be filled with 0s
NcImage* nc_make_image_zero(uint h, uint w, uint c);

// make an image, whose data will point to specified data
NcImage* nc_make_image(uint h, uint w, uint c, unsigned char* data);

// load an bmp image (BGR order)
void nc_image_load_bmp(const char* filename, uchar** _buf, uint* _h, uint* _w, uint c, bool line_align);

// save an bmp image
void nc_image_save_bmp(const char* filename, const uchar* buf, uint h, uint w, uint c);

// RGB to BGR
void nc_image_rgb_to_bgr_inplace(NcImage* im);

// BGR to RGB
void nc_image_bgr_to_rgb_inplace(NcImage* im);

#ifdef __cplusplus
}
#endif
