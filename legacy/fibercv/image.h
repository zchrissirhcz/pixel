#ifndef FIBERCV_IMAGE_H
#define FIBERCV_IMAGE_H

#define FIBERCV_VERSION_MAJOR 0
#define FIBERCV_VERSION_MINOR 0
#define FIBERCV_VERSION_PATCH 1

#include <stdlib.h>
#include <string.h>

namespace fc {

typedef enum MemType {
    kSTACK=0,
    kHEAP=1
} MemType;

typedef struct Image {
    // necessary data members
    int height;
    int width;
    int channel;
    unsigned char* data;

    // auxilary data members
    int align; // line align, 1 or 4
    int linebytes; // align_up(width*channel, align)
    int mass; // linebytes*height
    MemType mem_type; // the image itself, stack or heap?
    MemType data_mem_type; // the image data buffer, stack or heap?
} Image;

typedef enum ResizeMethod {
    kNEAREST=0,
    kBILINEAR=1
} ResizeMethod;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Color {
    unsigned char b;
    unsigned char g;
    unsigned char r;
} Color;

typedef struct Rect {
    int x1;
    int y1;
    int x2;
    int y2;
} Rect;

// create image
Image make_image(int w, int h, int c, int align=1);

// create image
Image* make_image_ptr(int w, int h, int c, int align=1);

// create image, with specified data buffer
Image* make_image_ptr(int w, int h, int c, unsigned char* data, int align=1);

// release image's heap memory
void free_image(Image* im);

// bgr <=> rgb
void rgb_bgr_swap(Image* im);

// convert bgr/rgb image to gray image
void bgr_to_gray(Image* im, Image* gray);

// gray image to black/white image (binarize)
void gray_to_bw(Image* gray, Image* bw, unsigned char thresh);

// image resize, support nearest/bilinear
void resize(Image* im_src, Image* im_dst, ResizeMethod method=kNEAREST);

// create histgram
Image* histgram(Image* im);

} // namespace fc

#endif // COINCV_IMAGE_H
