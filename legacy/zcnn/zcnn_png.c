/*
@file: nc_png.c
@date: 2020-03-22 19:14:23
@author: Zhuo Zhang(imzhuo@foxmail.com)

@description: load and save png file image using libpng, and provided as compatible with OpenCV
This implementation is:
- Compatible with OpenCV, i.e. BGR format
- Can handle both 4-byte-aligned or not aligned case, for loaded result / buffer to be saved to file

Note: This file does not define image struct/class. Feel free to define it yourself,
or just use 3rdparty provided, such as OpenCV's Mat/IplImage.

@references
- src/pixl/io_libpng.cc (https://github.com/mbrlabs/pixl/blob/master/src/pixl/io_libpng.cc)
- OpenCV 3.4.9, modules/imgcodecs/src/grfmt_png.cpp
- libpng's libpng-manual.txt (libpng-1.6/libpng-manual.txt)
*/


#include "zcnn_png.h"
#include <stdlib.h>
#include <string.h>

#include <png.h>

// define log macro
#define NC_PNG_THROW(msg) \
    fprintf(stderr, "=== NcError! Reason: %s. Position: line %d, file %s\n\n", #msg, __LINE__, __FILE__); \
    exit(1)


static uint nc_align_up(uint x, uint n) {
    return ((x + n - 1) / n)*n;
}

static bool is_big_endian()
{
    return (((const int*)"\0\x1\x2\x3\x4\x5\x6\x7")[0] & 255) != 0;
}


void nc_image_load_png(const char* filename, uchar** _buf, int* _height, int* _width, int* _channels, bool line_align)
{
    // open file
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        NC_PNG_THROW("Failed to read file");
    }

    // verify header(signature)
    uchar header[8];
    fread(header, 1, 8, fp);

    bool is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(fp);
        NC_PNG_THROW("Not png signature");
    }

    png_struct* png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        NC_PNG_THROW("Failed to get png struct");
    }

    png_info* info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        NC_PNG_THROW("Failed to get png info");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        NC_PNG_THROW("Failed during setjmp/longjmp");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);
    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (bit_depth != 8) {
        NC_PNG_THROW("Not supporting !=8 bit depth yet");
    }
    int channels = png_get_channels(png_ptr, info_ptr);
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    png_byte** row_pointers = (png_byte**)malloc(height * sizeof(png_byte*));

    int write_linebytes = rowbytes;
    if (line_align) {
        write_linebytes = nc_align_up(rowbytes, 4);
    }
    png_byte* buffer = (png_byte*)malloc(height*write_linebytes);
    for (int i = 0; i < height; i++) {
        row_pointers[i] = buffer + i * write_linebytes;
    }

    png_set_bgr(png_ptr);

    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, NULL);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(row_pointers);
    fclose(fp);

    //write back
    *_buf = buffer;
    *_height = height;
    *_width = width;
    *_channels = channels;
}


void nc_image_save_png(const char* filename, const uchar* buf, uint height, uint width, uint channels, uint read_linebytes)
{
    png_struct* png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        NC_PNG_THROW("failed to create write struct");
    }

    png_info* info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        NC_PNG_THROW("error occurs during setjmp/longjmp");
    }

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        NC_PNG_THROW("failed to open/create result file");
    }
    png_init_io(png_ptr, fp);
    
    // tune parameters for speed
    // (see http://wiki.linuxquestions.org/wiki/Libpng)
    int compression_strategy = 3; //IMWRITE_PNG_STRATEGY_RLE in OpenCV
    const int z_best_speed = 1;
    png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
    png_set_compression_level(png_ptr, z_best_speed);
    png_set_compression_strategy(png_ptr, compression_strategy);

    int color_type;
    if (channels == 1) {
        color_type = PNG_COLOR_TYPE_GRAY;
    }
    else if (channels == 3) {
        color_type = PNG_COLOR_TYPE_RGB;
    }
    else {
        color_type = PNG_COLOR_TYPE_RGBA;
    }
    
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        8,
        color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png_ptr, info_ptr);

    png_set_bgr(png_ptr); //save as bgr colors

    if (!is_big_endian()) {
        png_set_swap(png_ptr);
    }

    png_byte** row_pointers = (png_byte**)malloc(height * sizeof(png_byte*));
    for (uint i = 0; i < height; i++) {
        row_pointers[i] = (png_byte*)buf + i * read_linebytes;
    }
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);

    fclose(fp);
    free(row_pointers);
}