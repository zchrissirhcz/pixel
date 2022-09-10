#include "nc_image.h"

#include <stdlib.h>
#include <assert.h>

NcImage* nc_make_empty_image() {
    NcImage* im = (NcImage*)malloc(sizeof(NcImage));
    im->h = 0; im->w = 0; im->c = 0;
    im->linebytes = 0;
    im->mass = 0;
    im->data = NULL;
    return im;
}

NcImage* nc_make_image_header(uint h, uint w, uint c) {
    NcImage* im = (NcImage*)malloc(sizeof(NcImage));
    im->h = h; im->w = w; im->c = c;
    im->linebytes = w * c; //TODO: align here
    im->mass = im->linebytes * im->h;
    im->data = NULL;
    return im;
}

NcImage* nc_make_image_zero(uint h, uint w, uint c) {
    NcImage* im = nc_make_image_header(h, w, c);
    im->data = (unsigned char*)calloc(im->mass, 1);
    return im;
}

NcImage* nc_make_image(uint h, uint w, uint c, uchar* data) {
    NcImage* im = nc_make_image_header(h, w, c);
    im->data = data;
    return im;
}

void nc_image_rgb_to_bgr_inplace(NcImage* im){
    assert(im->c==3);
    uchar* c = im->data, t;
    uint line_pad = im->linebytes - im->w*3;
    for(uint h=0; h<im->h; h++) {
        for(uint w=0; w<im->w; w++, c+=3) {
            t = c[0]; c[0] = c[2]; c[2]=t;
        }
        c+=line_pad;
    }
}

void nc_image_bgr_to_rgb_inplace(NcImage* im){
    nc_image_rgb_to_bgr_inplace(im);
}
