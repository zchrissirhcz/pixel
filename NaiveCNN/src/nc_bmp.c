#include "nc_bmp.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "px_arithm.h"

#define NC_LOADBMP_FILE_NOT_FOUND         1
#define NC_LOADBMP_INVALID_FILE_FORMAT    2
#define NC_LOADBMP_INVALID_SIGNATURE      3
#define NC_LOADBMP_INVALID_BITS_PER_PIXEL 4
#define NC_LOADBMP_OUT_OF_MEMORY          5

#define NC_SAVEBMP_FILE_CANNOT_WRITE 6

#define NC_LOADBMP_RGBA 4

#define nc_bark_exit(ret_code)                                                                        \
    printf("=== NcError! Reason: %s. Position: line %d, file %s\n\n", #ret_code, __LINE__, __FILE__); \
    exit(ret_code)

void nc_load_bmp(const char* filename, uint8_t** _buf, int* _height, int* _width, int channel, bool line_align)
{
    FILE* fp = fopen(filename, "rb");

    if (!fp)
    {
        nc_bark_exit(NC_LOADBMP_FILE_NOT_FOUND);
    }

    //bmp header: 54 bytes
    //  file_header: 14 bytes
    //  info_header: 40 bytes
    unsigned char hd[54]; //14+40
    memset(hd, 0, 54);

    if (0 == fread(hd, 54, 1, fp))
    {
        fclose(fp);
        nc_bark_exit(NC_LOADBMP_INVALID_FILE_FORMAT);
    }
    if (0 != strncmp((const char*)hd, "BM", 2))
    {
        fclose(fp);
        nc_bark_exit(NC_LOADBMP_INVALID_SIGNATURE);
    }
    if (hd[28] != 24 && hd[28] != 32)
    {
        fclose(fp);
        nc_bark_exit(NC_LOADBMP_INVALID_BITS_PER_PIXEL);
    }

    uint32_t width = (uint32_t)hd[18] << 0 | (uint32_t)hd[19] << 8 | (uint32_t)hd[20] << 16 | (uint32_t)hd[21] << 24;
    uint32_t height = (uint32_t)hd[22] << 0 | (uint32_t)hd[23] << 8 | (uint32_t)hd[24] << 16 | (uint32_t)hd[25] << 24;

    unsigned char* buf = NULL;
    if (width > 0 && height > 0)
    {
        // w*3: we only need to store BGR instead of BGRA image
        uint32_t line_bytes = line_align ? px_align_up(width * 3, 4) : width * 3;
        uint32_t line_pad = line_bytes - width * 3; // for store
        buf = (uint8_t*)malloc(line_bytes * channel);
        if (!buf)
        {
            fclose(fp);
            nc_bark_exit(NC_LOADBMP_OUT_OF_MEMORY);
        }

        uint32_t padding = px_align_up(width * 3, 4) - width * 3; // for read
        unsigned char bmp_pad[3];
        unsigned char* cur = buf;
        for (uint32_t h = height - 1; h > 0; h--)
        {
            for (uint32_t w = 0; w < width; w++)
            {
                // get pixel with BGR order
                if (0 == fread(cur, 3, 1, fp))
                {
                    free(buf);
                    fclose(fp);
                    nc_bark_exit(NC_LOADBMP_INVALID_FILE_FORMAT);
                }
                cur += 3;
                if (channel == NC_LOADBMP_RGBA)
                {
                    *cur++ = 255;
                }
            }
            cur += line_pad;
            if (padding != fread(bmp_pad, 1, padding, fp))
            {
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

void nc_save_bmp(const char* filename, const uint8_t* buf, int height, int width, int channel)
{
    FILE* fp = fopen(filename, "wb");
    if (!fp)
    {
        nc_bark_exit(NC_SAVEBMP_FILE_CANNOT_WRITE);
    }

    unsigned char hd[54] = {
        //bmp header
        'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,   //bmp file header, 14 bytes
        40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 //bmp info header, 40 bytes
    };
    const unsigned int size = 54 + width * height * 3; //3channel

    hd[2] = (uint8_t)(size);
    hd[3] = (uint8_t)(size >> 8);
    hd[4] = (uint8_t)(size >> 16);
    hd[5] = (uint8_t)(size >> 24);

    hd[18] = (uint8_t)(width);
    hd[19] = (uint8_t)(width >> 8);
    hd[20] = (uint8_t)(width >> 16);
    hd[21] = (uint8_t)(width >> 24);

    hd[22] = (uint8_t)(height);
    hd[23] = (uint8_t)(height >> 8);
    hd[24] = (uint8_t)(height >> 16);
    hd[25] = (uint8_t)(height >> 24);

    if (1 != fwrite(hd, 54, 1, fp))
    {
        fclose(fp);
        nc_bark_exit(NC_SAVEBMP_FILE_CANNOT_WRITE);
    }

    char bmp_pad[3] = {0, 0, 0};
    uint32_t pixel_bytes = width * 3;
    uint32_t line_bytes = px_align_up(width * 3, 4);
    uint32_t line_pad = line_bytes - pixel_bytes;
    buf += line_bytes * (height - 1);
    for (int h = height - 1; h > 0; h--)
    {
        if (fwrite(buf, pixel_bytes, 1, fp) == 1 && fwrite(bmp_pad, 1, line_pad, fp) == line_pad)
        {
            buf -= pixel_bytes;
        }
        else
        {
            nc_bark_exit(NC_SAVEBMP_FILE_CANNOT_WRITE);
        }
    }
    fclose(fp);
}
