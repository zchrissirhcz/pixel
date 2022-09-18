#pragma once

#include <stdbool.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

#ifdef __cplusplus
extern "C"{
#endif

    void nc_image_load_png(const char* filename, uchar** _buf, int* _height, int* _width, int* _channel, bool line_align);
    void nc_image_save_png(const char* filename, const uchar* buf, uint height, uint width, uint channels, uint read_linebytes);

#ifdef __cplusplus
}
#endif

