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
    BMP_image bmp_image = {0};
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
        if (fread(bmp_file_header_buf, sizeof(bmp_file_header_buf), 1, fin) == 0)
        {
            PX_LOGE("fread error");
            break;
        }
        if (fread(bmp_info_header_buf, sizeof(bmp_info_header_buf), 1, fin) == 0)
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
        bmp_file_header->file_size = (uint32_t)hd[2] << 0 | (uint32_t)hd[3] << 8 | (uint32_t)hd[4] << 16 | (uint32_t)hd[5] << 24;

        int big_endian = px_is_big_endian();
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_file_header->file_size);
        }

        // parse reserved
        bmp_file_header->reserved[0] = (uint16_t)hd[6] << 0 | (uint16_t)hd[7] << 8;
        bmp_file_header->reserved[1] = (uint16_t)hd[8] << 0 | (uint16_t)hd[9] << 8;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_file_header->reserved[0]);
            PX_SWAP_BYTES(bmp_file_header->reserved[1]);
        }

        // parse offset
        bmp_file_header->offset = (uint32_t)hd[10] << 0 | (uint32_t)hd[11] << 8 | (uint32_t)hd[12] << 16 | (uint32_t)hd[13] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_file_header->offset);
        }

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
        bmp_info_header->header_size = (uint32_t)hd[0] << 0 | (uint32_t)hd[1] << 8 | (uint32_t)hd[2] << 16 | (uint32_t)hd[3] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->header_size);
        }
        if (bmp_info_header->header_size != 40)
        {
            PX_LOGE("invalid header size. only 40 is valid");
            break;
        }

        // parse width and validate
        bmp_info_header->width = (uint32_t)hd[4] << 0 | (uint32_t)hd[5] << 8 | (uint32_t)hd[6] << 16 | (uint32_t)hd[7] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->width);
        }
        if (bmp_info_header->width <= 0)
        {
            PX_LOGE("invalid width, >0 required, but got %d", bmp_info_header->width);
            break;
        }

        // parse height and validate
        bmp_info_header->height = (uint32_t)hd[8] << 0 | (uint32_t)hd[9] << 8 | (uint32_t)hd[10] << 16 | (uint32_t)hd[11] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->height);
        }
        if (bmp_info_header->height <= 0)
        {
            PX_LOGE("invalid height, >0 required, but got %d", bmp_info_header->height);
            break;
        }

        // parse num_planes and validate
        bmp_info_header->num_planes = (uint16_t)hd[12] << 0 | (uint16_t)hd[13] << 8;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->num_planes);
        }
        if (bmp_info_header->num_planes != 1)
        {
            PX_LOGE("invalid num_planes, must be 1");
            break;
        }

        // parse bits_per_pixel and validate
        bmp_info_header->bits_per_pixel = (uint16_t)hd[14] << 0 | (uint16_t)hd[15] << 8;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->bits_per_pixel);
        }
        int bpp = bmp_info_header->bits_per_pixel;
        if (bpp != 8 && bpp != 24 && bpp != 32)
        {
            PX_LOGE("not supported bpp, only 8, 24, 32 supported now");
            break;
        }

        // parse compression_type and validate
        bmp_info_header->compression_type = (uint32_t)hd[16] << 0 | (uint32_t)hd[17] << 8 | (uint32_t)hd[18] << 16 | (uint32_t)hd[19] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->compression_type);
        }
        if (bmp_info_header->compression_type != BMP_RGB)
        {
            PX_LOGE("not supported compression_type, only BMP_RGB supported now");
            break;
        }

        // parse image_size
        bmp_info_header->image_size = (uint32_t)hd[20] << 0 | (uint32_t)hd[21] << 8 | (uint32_t)hd[22] << 16 | (uint32_t)hd[23] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->image_size);
        }

        // parse x_resolution
        bmp_info_header->x_resolution = (uint32_t)hd[24] << 0 | (uint32_t)hd[25] << 8 | (uint32_t)hd[26] << 16 | (uint32_t)hd[27] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->x_resolution);
        }

        // parse y_resolution
        bmp_info_header->y_resolution = (uint32_t)hd[28] << 0 | (uint32_t)hd[29] << 8 | (uint32_t)hd[30] << 16 | (uint32_t)hd[31] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->y_resolution);
        }

        // parse num_colors
        bmp_info_header->num_colors = (uint32_t)hd[32] << 0 | (uint32_t)hd[33] << 8 | (uint32_t)hd[34] << 16 | (uint32_t)hd[35] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->num_colors);
        }

        // parse num_important_colors
        bmp_info_header->num_important_colors = (uint32_t)hd[36] << 0 | (uint32_t)hd[37] << 8 | (uint32_t)hd[38] << 16 | (uint32_t)hd[39] << 24;
        if (big_endian)
        {
            PX_SWAP_BYTES(bmp_info_header->num_important_colors);
        }

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
            if (0 == fread(bmp_image.palette, num_of_palette * sizeof(BMP_palette_entry), 1, fin))
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
                    if (0 == fread(dst_line + x, dst_channel, 1, fin))
                    {
                        fread_valid = false;
                    }
                    if (swap_bgr)
                    {
                        dst_line[x] ^= dst_line[x + 2] ^= dst_line[x] ^= dst_line[x + 2]; // BGR -> RGB
                    }
                }
                dst_line -= dst_linebytes;
                if (0 == fread(bmp_gap, 1, src_gap, fin))
                {
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
            if (0 == fread(bmp_image.data, buf_size, 1, fin))
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
            int src_lineskip = src_linebytes - src_channel * width;
            int dst_lineskip = dst_linebytes - line_limit;
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
            'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //bmp file header, 14 bytes
            40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 //bmp info header, 40 bytes
        };
        unsigned int size = 54 + height * width * channels;
        if (channels == 1)
        {
            size += 1024; //256 colors * 4 bytes, 256 palette is 1<<8, 8 is bpp
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
        { //colorize with palette, required only when channels==1
            uint8_t color_cnt = 0;
            uint8_t palette[1024];
            for (int i = 0; i < 256; i++)
            {
                palette[i * 4] = i;
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