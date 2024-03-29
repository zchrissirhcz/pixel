#include "px_log.h"
#include "px_bmp.h"
#include "px_assert.h"
#include "px_arithm.h"
#include "px_endian.h"

#include <stdlib.h>
#include <string.h>

// References
// 1. https://wiki.multimedia.cx/index.php/BMP
// 2. Chapter 23 in "Intermediate C Programming", by YungSiang-Lu, Chinese tranlation edition
// 3. https://www.cnblogs.com/wainiwann/p/7086844.html
// 4. https://github.com/vallentin/LoadBMP
// 5. opencv/modules/imgcodecs/src/grfmt_bmp.hpp

typedef struct BMP_file_header
{
    char magic[2]; // must be 'B' 'M'
    int32_t file_size;
    int16_t reserved[2];
    int32_t offset; // offset to bitmap data, relative to start of file
} BMP_file_header;

// BMP_info_header: the most widely used DIB header definition
typedef struct BMP_info_header
{
    uint32_t header_size;
    int32_t width;
    int32_t height;

    int16_t num_planes;        // must be 1
    uint16_t bits_per_pixel;   //BPP. Typical values are 1, 4, 8, 16, 24 and 32
    uint32_t compression_type; // in most common cases, it is 0 (BI_RGB)
    uint32_t image_size;
    int32_t x_resolution;
    int32_t y_resolution;
    uint32_t num_colors;           // number of colors in palette. if 0, use 2^(BPP)
    uint32_t num_important_colors; // the number of important colors used, or 0 when every color is important; generally ignored
} BMP_info_header;

typedef enum BMP_compression
{
    BMP_RGB = 0,
    BMP_RLE8 = 1,
    BMP_RLE4 = 2,
    BMP_BITFIELDS = 3
} BMP_compression;

typedef struct BMP_palette_entry
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved; // or alpha
} BMP_palette_entry;

typedef struct BMP_image
{
    BMP_file_header file_header;
    BMP_info_header info_header;
    BMP_palette_entry* palette;
    uint8_t* data;
} BMP_image;

static bool is_valid_bmp_filename(const char* filepath)
{
    // filename checking, may be ignored
    if (strlen(filepath) < 5)
    {
        PX_LOGE("filename too short");
        return false;
    }

    // file extension checking, may be ignored
    const char* ext = filepath + strlen(filepath) - 4;
    if (strcmp(ext, ".bmp") != 0)
    {
        PX_LOGE("filename not ends with .bmp");
        return false;
    }

    return true;
}

void px_read_bmp(const char* filepath, int* _height, int* _width, int* _channel, unsigned char** _buffer)
{
    px_read_bmp_custom(filepath, _height, _width, _channel, _buffer, 1, false);
}

static int32_t read_int32_from_binary(unsigned char* buf)
{
    int value = (uint32_t)buf[0] << 0 | (uint32_t)buf[1] << 8 | (uint32_t)buf[2] << 16 | (uint32_t)buf[3] << 24;

    int big_endian = px_is_big_endian();
    if (big_endian)
    {
        PX_SWAP_BYTES(value);
    }
    return value;
}

static uint32_t read_uint32_from_binary(unsigned char* buf)
{
    uint32_t value = (uint32_t)buf[0] << 0 | (uint32_t)buf[1] << 8 | (uint32_t)buf[2] << 16 | (uint32_t)buf[3] << 24;

    int big_endian = px_is_big_endian();
    if (big_endian)
    {
        PX_SWAP_BYTES(value);
    }
    return value;
}

static int16_t read_int16_from_binary(unsigned char* buf)
{
    int16_t value = (uint16_t)buf[0] << 0 | (uint16_t)buf[1] << 8;

    int big_endian = px_is_big_endian();
    if (big_endian)
    {
        PX_SWAP_BYTES(value);
    }
    return value;
}

static uint16_t read_uint16_from_binary(unsigned char* buf)
{
    uint16_t value = (uint16_t)buf[0] << 0 | (uint16_t)buf[1] << 8;

    int big_endian = px_is_big_endian();
    if (big_endian)
    {
        PX_SWAP_BYTES(value);
    }
    return value;
}

/// decode bmp image
/// when bmp is BGR(bpp=24), decoded as BGR image  (3 channel)
/// when bmp is GRAY(bpp=8), decoded as GRAY image (1 channel)
/// other bpp (e.g. 16, 32) not supported yet
/// @param line_align: when storing result buffer, line align. (only 1 and 4 are valid)
/// @param _buffer: result buffer, should be NULL when passing in, and should be free by user
/// @param swap_bgr: when the bmp is BGR (bpp=24) and swap_bgr is 1, then swap b and r, gain RGB buffer
void px_read_bmp_custom(const char* filepath, int* _height, int* _width, int* _channel, unsigned char** _buffer, int line_align, bool swap_bgr)
{
    FILE* fin = NULL;
    int height = 0;
    int width = 0;
    int src_channel = 0;
    int dst_channel = 0;
    BMP_image bmp_image;
    memset(&bmp_image, 0, sizeof(BMP_image));
    unsigned char* buffer = NULL;
    do {
        if (!is_valid_bmp_filename(filepath))
        {
            break;
        }

        if (line_align != 1 && line_align != 4)
        {
            PX_LOGE("line_align invalid, only 1 or 4 supported");
            break;
        }

        // ----------------------------------------------------------------------
        // => read bmp file header, bmp info header's raw buffer
        // ----------------------------------------------------------------------
        unsigned char bmp_file_header_buf[14] = {0};
        unsigned char bmp_info_header_buf[40] = {0};

        fin = fopen(filepath, "rb");
        if (!fin)
        {
            PX_LOGE("failed to open file %s", filepath);
            break;
        }
        if (fread(bmp_file_header_buf, sizeof(bmp_file_header_buf), 1, fin) != 1)
        {
            PX_LOGE("fread error");
            break;
        }
        if (fread(bmp_info_header_buf, sizeof(bmp_info_header_buf), 1, fin) != 1)
        {
            PX_LOGE("fread error");
            break;
        }

        // ----------------------------------------------------------------------
        // => decode and validate bmp_file_header
        // ----------------------------------------------------------------------
        BMP_file_header* bmp_file_header = &bmp_image.file_header;
        BMP_info_header* bmp_info_header = &bmp_image.info_header;

        unsigned char* hd = bmp_file_header_buf;
        bmp_file_header->magic[0] = hd[0];
        bmp_file_header->magic[1] = hd[1];

        // parse and check magic (signature)
        const char* bmp_signature = "BM";
        if (memcmp(bmp_file_header, bmp_signature, strlen(bmp_signature)) != 0)
        {
            PX_LOGE("invalid bmp signagure");
            break;
        }

        // parse file_size
        bmp_file_header->file_size = read_int32_from_binary(hd + 2);

        // parse reserved
        bmp_file_header->reserved[0] = read_int16_from_binary(hd + 6);
        bmp_file_header->reserved[1] = read_int16_from_binary(hd + 8);

        // parse offset
        bmp_file_header->offset = read_int32_from_binary(hd + 10);

        // ----------------------------------------------------------------------
        // => decode and validate bmp_info_header
        // ----------------------------------------------------------------------
        // BMP_info_header: the most widely used DIB header definition

        // typedef struct BMP_info_header
        // {
        //     uint32_t header_size;
        //     int32_t  width;
        //     int32_t  height;
        //
        //     int16_t  num_planes; // must be 1
        //     uint16_t bits_per_pixel; //BPP. Typical values are 1, 4, 8, 16, 24 and 32
        //     uint32_t compression_type; // in most common cases, it is 0 (BI_RGB)
        //     uint32_t image_size;
        //     int32_t  x_resolution;
        //     int32_t  y_resolution;
        //     uint32_t num_colors; // number of colors in palette. if 0, use 2^(BPP)
        //     uint32_t num_important_colors; // the number of important colors used, or 0 when every color is important; generally ignored
        // } BMP_info_header;

        // parse header_size and validate
        hd = bmp_info_header_buf;
        bmp_info_header->header_size = read_uint32_from_binary(hd);
        if (bmp_info_header->header_size != 40)
        {
            PX_LOGE("invalid header size. only 40 is valid");
            break;
        }

        // parse width and validate
        bmp_info_header->width = read_int32_from_binary(hd + 4);
        if (bmp_info_header->width <= 0)
        {
            PX_LOGE("invalid width, >0 required, but got %d", bmp_info_header->width);
            break;
        }

        // parse height and validate
        bmp_info_header->height = read_int32_from_binary(hd + 8);
        if (bmp_info_header->height <= 0)
        {
            PX_LOGE("invalid height, >0 required, but got %d", bmp_info_header->height);
            break;
        }

        // parse num_planes and validate
        bmp_info_header->num_planes = read_int16_from_binary(hd + 12);
        if (bmp_info_header->num_planes != 1)
        {
            PX_LOGE("invalid num_planes, must be 1");
            break;
        }

        // parse bits_per_pixel and validate
        bmp_info_header->bits_per_pixel = read_uint16_from_binary(hd + 14);
        int bpp = bmp_info_header->bits_per_pixel;
        if (bpp != 8 && bpp != 24 && bpp != 32)
        {
            PX_LOGE("not supported bpp(%d), only 8, 24, 32 supported now", bpp);
            break;
        }

        // parse compression_type and validate
        bmp_info_header->compression_type = read_uint32_from_binary(hd + 16);
        if (bmp_info_header->compression_type != BMP_RGB)
        {
            PX_LOGE("not supported compression_type(%d), only BMP_RGB supported now", bmp_info_header->compression_type);
            break;
        }

        // parse image_size
        bmp_info_header->image_size = read_uint32_from_binary(hd + 20);

        // parse x_resolution
        bmp_info_header->x_resolution = read_int32_from_binary(hd + 24);

        // parse y_resolution
        bmp_info_header->y_resolution = read_int32_from_binary(hd + 28);

        // parse num_colors
        bmp_info_header->num_colors = read_uint32_from_binary(hd + 32);

        // parse num_important_colors
        bmp_info_header->num_important_colors = read_uint32_from_binary(hd + 36);

        // ----------------------------------------------------------------------
        // => read palette if necessary
        // ----------------------------------------------------------------------
        // only when compression type is BMP_RGB(0) and BPP<16 (1,2,4,8), palette is required
        // for simplicity, we only consider BPP=8 palette, i.e. gray image
        if (bmp_info_header->compression_type == BMP_RGB && bpp == 8)
        {
            int num_of_palette;
            if (bmp_info_header->num_colors != 0 && bmp_info_header->num_colors < (1U << bpp))
            {
                num_of_palette = bmp_info_header->num_colors;
            }
            else
            {
                num_of_palette = (1 << bpp);
            }
            // allocate and read-in palette's data
            bmp_image.palette = (BMP_palette_entry*)malloc(num_of_palette * sizeof(BMP_palette_entry));
            if (bmp_image.palette == NULL)
            {
                PX_LOGE("failed to allocate memory for palette");
                break;
            }
            if (fread(bmp_image.palette, num_of_palette * sizeof(BMP_palette_entry), 1, fin) != 1)
            {
                PX_LOGE("fread failed for reading palette");
                break;
            }
        }

        // ----------------------------------------------------------------------
        // => read pixels
        // ----------------------------------------------------------------------
        // palette is not required, for BGR images
        height = bmp_info_header->height;
        width = bmp_info_header->width;
        src_channel = bpp / 8;
        int src_linebytes = px_align_up(width * src_channel, 4);
        if (bmp_image.palette == NULL)
        {
            dst_channel = 3;
            int dst_linebytes = px_align_up(width * dst_channel, line_align);
            unsigned char bmp_gap[3] = {0};
            int src_gap = src_linebytes - width * src_channel;
            int buf_size = dst_linebytes * height;
            bmp_image.data = (unsigned char*)malloc(buf_size);
            buffer = bmp_image.data;
            unsigned char* dst_line = bmp_image.data + (height - 1) * dst_linebytes;
            bool fread_valid = true;
            int line_limit = dst_channel * width;
            for (int y = height - 1; y != -1 && fread_valid; y--)
            {
                for (int x = 0; x < line_limit && fread_valid; x += dst_channel)
                {
                    if (fread(dst_line + x, dst_channel, 1, fin) != 1)
                    {
                        PX_LOGE("fread failed in %s:%d , y=%d(%d), x=%d(%d) (filepath=%s, dst_channel=%d)\n", 
                            __FILE__, __LINE__,
                            y, height,
                            x, line_limit,
                            filepath, dst_channel
                        );
                        exit(1);
                        fread_valid = false;
                    }
                    if (swap_bgr)
                    {
                        dst_line[x] ^= dst_line[x + 2] ^= dst_line[x] ^= dst_line[x + 2]; // BGR -> RGB
                    }
                }
                dst_line -= dst_linebytes;
                if ((src_gap !=0) && (fread(bmp_gap, src_gap, 1, fin) != 1))
                {
                        PX_LOGE("fread failed in %s:%d , y=%d(%d) (filepath=%s, dst_channel=%d)\n", 
                            __FILE__, __LINE__,
                            y, height,
                            filepath, dst_channel
                        );
                    exit(1);
                    fread_valid = false;
                }
            }
            if (!fread_valid)
            {
                PX_LOGE("fread failed when read pixels");
                break;
            }
        }
        else
        {
            // reading pixel color indices into bmp_image.data
            // then get real color from palette
            dst_channel = src_channel; // 1
            int buf_size = src_linebytes * height;
            int dst_linebytes = px_align_up(width * dst_channel, line_align);
            bmp_image.data = (unsigned char*)malloc(buf_size);
            if (fread(bmp_image.data, buf_size, 1, fin) != 1)
            {
                PX_LOGE("fread failed when read pixel color indices");
                break;
            }

            buffer = (unsigned char*)malloc(dst_linebytes * height);
            if (buffer == NULL)
            {
                PX_LOGE("malloc failed");
                break;
            }
            int line_limit = dst_channel * width;
            unsigned char* dst_line = buffer + (height - 1) * dst_linebytes;
            unsigned char* src_line = bmp_image.data;
            for (int y = height - 1; y != -1; y--)
            {
                for (int x = 0; x < line_limit; x += dst_channel)
                {
                    if (dst_channel == 3)
                    {
                        dst_line[x] = bmp_image.palette[src_line[x]].red;
                        dst_line[x + 1] = bmp_image.palette[src_line[x]].green;
                        dst_line[x + 2] = bmp_image.palette[src_line[x]].blue;
                    }
                    else if (dst_channel == 1)
                    {
                        dst_line[x] = bmp_image.palette[src_line[x]].red;
                    }
                }
                src_line += src_linebytes;
                dst_line -= dst_linebytes;
            }
        }

    } while (0);

    if (fin != NULL)
    {
        fclose(fin);
    }
    if (buffer != bmp_image.data)
    {
        free(bmp_image.data);
    }
    if (bmp_image.palette != NULL)
    {
        free(bmp_image.palette);
    }

    *_height = height;
    *_width = width;
    *_channel = dst_channel;
    *_buffer = buffer;
}

void px_write_bmp(const char* filepath, int height, int width, int channel, const unsigned char* buffer)
{
    px_write_bmp_custom(filepath, height, width, channel, buffer, width * channel, false);
}

void px_write_bmp_custom(const char* filepath, int height, int width, int channels, const unsigned char* buf, int read_linebytes, bool swap_bgr)
{
    FILE* fout = NULL;

    do {
        fout = fopen(filepath, "wb");
        if (fout == NULL)
        {
            PX_LOGE("fopen failed");
            break;
        }

        if (read_linebytes < width * channels)
        {
            PX_LOGE("the given read_linebytes is not valid");
            break;
        }

        unsigned char hd[54] = {
            //bmp header
            'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // bmp file header, 14 bytes
            40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 // bmp info header, 40 bytes
        };
        unsigned int size = 54 + height * width * channels;
        if (channels == 1)
        {
            size += 1024; // 256 colors * 4 bytes, 256 palette is 1<<8, 8 is bpp
        }

        hd[2] = (uint8_t)(size);
        hd[3] = (uint8_t)(size >> 8);
        hd[4] = (uint8_t)(size >> 16);
        hd[5] = (uint8_t)(size >> 24);

        if (channels == 3)
        {
            hd[10] = 54;
        }
        else if (channels == 1)
        {
            hd[10] = (uint8_t)(1078);
            hd[11] = (uint8_t)(1078 >> 8);
        }

        hd[18] = (uint8_t)(width);
        hd[19] = (uint8_t)(width >> 8);
        hd[20] = (uint8_t)(width >> 16);
        hd[21] = (uint8_t)(width >> 24);

        hd[22] = (uint8_t)(height);
        hd[23] = (uint8_t)(height >> 8);
        hd[24] = (uint8_t)(height >> 16);
        hd[25] = (uint8_t)(height >> 24);

        hd[26] = 1;
        hd[28] = (uint8_t)channels * 8;

        if (1 != fwrite(hd, 54, 1, fout))
        {
            PX_LOGE("fwrite failed");
            break;
        }

        if (channels == 1)
        {
            // colorize with palette, required only when channels==1
            uint8_t palette[1024];
            for (int i = 0; i < 256; i++)
            {
                palette[i * 4 + 0] = i;
                palette[i * 4 + 1] = i;
                palette[i * 4 + 2] = i;
                palette[i * 4 + 3] = 0;
            }
            if (fwrite(palette, 1024, 1, fout) != 1)
            {
                PX_LOGE("fwrite failed");
                break;
            }
        }

        char bmp_pad[3] = {0, 0, 0};
        uint32_t write_linebytes = px_align_up(width * channels, 4);
        int line_limit = width * channels;
        uint32_t line_pad = write_linebytes - line_limit;

        bool fwrite_valid = true;
        const uint8_t* src_line = buf + (height - 1) * read_linebytes;

        if (channels == 3 && swap_bgr)
        {
            for (int y = height - 1; y != -1 && fwrite_valid; y--)
            {
                for (int x = 0; x < line_limit && fwrite_valid; x += channels)
                {
                    // BGR -> RGB
                    if (fwrite(src_line + x + 2, 1, 1, fout) != 1)
                    {
                        fwrite_valid = false;
                        PX_LOGE("fwrite failed");
                    }
                    if (fwrite(src_line + x + 1, 1, 1, fout) != 1)
                    {
                        fwrite_valid = false;
                        PX_LOGE("fwrite failed");
                    }
                    if (fwrite(src_line + x, 1, 1, fout) != 1)
                    {
                        fwrite_valid = false;
                        PX_LOGE("fwrite failed");
                    }
                }
                if (fwrite(bmp_pad, 1, line_pad, fout) != line_pad)
                {
                    fwrite_valid = false;
                    PX_LOGE("fwrite failed");
                }
                src_line -= read_linebytes;
            }
        }
        else
        {
            for (int y = height - 1; y != -1 && fwrite_valid; y--)
            {
                if (fwrite(src_line, line_limit, 1, fout) != 1)
                {
                    fwrite_valid = false;
                    PX_LOGE("fwrite failed");
                }
                if (fwrite(bmp_pad, 1, line_pad, fout) != line_pad)
                {
                    fwrite_valid = false;
                    PX_LOGE("fwrite failed");
                }
                src_line -= read_linebytes;
            }
            if (!fwrite_valid)
            {
                break;
            }
        }
    } while (0);

    if (fout != NULL)
    {
        fclose(fout);
    }
}