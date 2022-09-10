#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "nc_image.h"
#include "nc_memory.h"

#define NC_LOADBMP_FILE_NOT_FOUND 1
#define NC_LOADBMP_INVALID_FILE_FORMAT 2
#define NC_LOADBMP_INVALID_SIGNATURE 3
#define NC_LOADBMP_INVALID_BITS_PER_PIXEL 4
#define NC_LOADBMP_OUT_OF_MEMORY 5

#define NC_SAVEBMP_FILE_CANNOT_WRITE 6

#define NC_LOADBMP_RGBA 4

#define nc_bark_exit(ret_code) \
    printf("=== NcError! Reason: %s. Position: line %d, file %s\n\n", #ret_code, __LINE__, __FILE__); \
    exit(ret_code)

void nc_image_load_bmp(const char* filename, uchar** _buf, uint* _height, uint* _width, uint channel, bool line_align) {
    FILE* fp = fopen(filename, "rb");

    if (!fp) {
        nc_bark_exit(NC_LOADBMP_FILE_NOT_FOUND);
    }

    //bmp header: 54 bytes
    //  file_header: 14 bytes
    //  info_header: 40 bytes
    unsigned char hd[54]; //14+40
    memset(hd, 0, 54);

    if (0==fread(hd, 54, 1, fp)) {
        fclose(fp);
        nc_bark_exit(NC_LOADBMP_INVALID_FILE_FORMAT);
    }
    if (0 != strncmp((const char*)hd, "BM", 2)) {
        fclose(fp);
        nc_bark_exit(NC_LOADBMP_INVALID_SIGNATURE);
    }
    if (hd[28]!=24 && hd[28]!=32) {
        fclose(fp);
        nc_bark_exit(NC_LOADBMP_INVALID_BITS_PER_PIXEL);
    }

    uint width = (uint)hd[18]<<0 | (uint)hd[19]<<8 | (uint)hd[20]<<16 | (uint)hd[21]<<24;
    uint height = (uint)hd[22]<<0 | (uint)hd[23]<<8 | (uint)hd[24]<<16 | (uint)hd[25]<<24;

    unsigned char* buf = NULL;
    if (width>0 && height>0) {
        // w*3: we only need to store BGR instead of BGRA image
        uint line_bytes = line_align ? nc_align_up(width*3, 4) : width*3;
        uint line_pad = line_bytes - width*3; // for store
        buf = (uchar*)malloc(line_bytes*channel);
        if (!buf) {
            fclose(fp);
            nc_bark_exit(NC_LOADBMP_OUT_OF_MEMORY);
        }

        uint padding = nc_align_up(width*3,4) - width*3; // for read
        unsigned char bmp_pad[3];
        unsigned char* cur = buf;
        for(uint h=height-1; h>0; h--) {
            for(uint w=0; w<width; w++) {
                // get pixel with BGR order
                if (0==fread(cur, 3, 1, fp)) {
                    free(buf);
                    fclose(fp);
                    nc_bark_exit(NC_LOADBMP_INVALID_FILE_FORMAT);
                }
                cur+=3;
                if (channel==NC_LOADBMP_RGBA) {
                    *cur++=255;
                }
            }
            cur+=line_pad;
            if (padding!=fread(bmp_pad, 1, padding, fp)) {
                free(buf);
                fclose(fp);
                nc_bark_exit(NC_LOADBMP_INVALID_FILE_FORMAT);
            }
        }
    }

    //write back
    *_buf = buf;
    *_width = width;
    *_height = height;

    fclose(fp);
}

void nc_image_save_bmp(const char* filename, const uchar* buf, uint height, uint width, uint channel) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        nc_bark_exit(NC_SAVEBMP_FILE_CANNOT_WRITE);
    }

    unsigned char hd[54] = { //bmp header
        'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, //bmp file header, 14 bytes
        40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 //bmp info header, 40 bytes 
    };
    const unsigned int size = 54 + width*height*3;  //3channel
    
    hd[2] = (uchar)(size);
    hd[3] = (uchar)(size>>8);
    hd[4] = (uchar)(size>>16);
    hd[5] = (uchar)(size>>24);

    hd[18] = (uchar)(width);
    hd[19] = (uchar)(width>>8);
    hd[20] = (uchar)(width>>16);
    hd[21] = (uchar)(width>>24);

    hd[22] = (uchar)(height);
    hd[23] = (uchar)(height>>8);
    hd[24] = (uchar)(height>>16);
    hd[25] = (uchar)(height>>24);

    if (1!=fwrite(hd, 54, 1, fp)) {
        fclose(fp);
        nc_bark_exit(NC_SAVEBMP_FILE_CANNOT_WRITE);
    }
    
    char bmp_pad[3] = {0, 0, 0};
    uint pixel_bytes = width * 3;
    uint line_bytes = nc_align_up(width*3, 4);
    uint line_pad = line_bytes - pixel_bytes;
    buf += line_bytes * (height-1);
    for(uint h=height-1; h!=-1; h--) {
        if (fwrite(buf, pixel_bytes, 1, fp)==1 && fwrite(bmp_pad, 1, line_pad, fp)==line_pad) {
            buf -= pixel_bytes;
        } else {
            nc_bark_exit(NC_SAVEBMP_FILE_CANNOT_WRITE);
        }
    }
    fclose(fp);
}
