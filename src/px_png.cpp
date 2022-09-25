
#include "px_png.h"
#include "px_image.h"
#include "spng.h"
#include "px_log.h"
#include <vector>
#include <algorithm>
#include <stdlib.h>

// from miniz.h
enum
{
    Z_NO_COMPRESSION = 0,
    Z_BEST_SPEED = 1,
    Z_BEST_COMPRESSION = 9,
    Z_UBER_COMPRESSION = 10,
    Z_DEFAULT_LEVEL = 6,
    Z_DEFAULT_COMPRESSION = -1
};

// from miniz.h
enum
{
    Z_DEFAULT_STRATEGY = 0,
    Z_FILTERED = 1,
    Z_HUFFMAN_ONLY = 2,
    Z_RLE = 3,
    Z_FIXED = 4
};

// from opencv2/imgcodecs.hpp
enum ImwritePNGFlags {
       IMWRITE_PNG_STRATEGY_DEFAULT      = 0, //!< Use this value for normal data.
       IMWRITE_PNG_STRATEGY_FILTERED     = 1, //!< Use this value for data produced by a filter (or predictor).Filtered data consists mostly of small values with a somewhat random distribution. In this case, the compression algorithm is tuned to compress them better.
       IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY = 2, //!< Use this value to force Huffman encoding only (no string match).
       IMWRITE_PNG_STRATEGY_RLE          = 3, //!< Use this value to limit match distances to one (run-length encoding).
       IMWRITE_PNG_STRATEGY_FIXED        = 4  //!< Using this value prevents the use of dynamic Huffman codes, allowing for a simpler decoder for special applications.
     };

//! Imwrite flags
enum ImwriteFlags {
       IMWRITE_JPEG_QUALITY        = 1,  //!< For JPEG, it can be a quality from 0 to 100 (the higher is the better). Default value is 95.
       IMWRITE_JPEG_PROGRESSIVE    = 2,  //!< Enable JPEG features, 0 or 1, default is False.
       IMWRITE_JPEG_OPTIMIZE       = 3,  //!< Enable JPEG features, 0 or 1, default is False.
       IMWRITE_JPEG_RST_INTERVAL   = 4,  //!< JPEG restart interval, 0 - 65535, default is 0 - no restart.
       IMWRITE_JPEG_LUMA_QUALITY   = 5,  //!< Separate luma quality level, 0 - 100, default is 0 - don't use.
       IMWRITE_JPEG_CHROMA_QUALITY = 6,  //!< Separate chroma quality level, 0 - 100, default is 0 - don't use.
       IMWRITE_PNG_COMPRESSION     = 16, //!< For PNG, it can be the compression level from 0 to 9. A higher value means a smaller size and longer compression time. If specified, strategy is changed to IMWRITE_PNG_STRATEGY_DEFAULT (Z_DEFAULT_STRATEGY). Default value is 1 (best speed setting).
       IMWRITE_PNG_STRATEGY        = 17, //!< One of cv::ImwritePNGFlags, default is IMWRITE_PNG_STRATEGY_RLE.
       IMWRITE_PNG_BILEVEL         = 18, //!< Binary level PNG, 0 or 1, default is 0.
       IMWRITE_PXM_BINARY          = 32, //!< For PPM, PGM, or PBM, it can be a binary format flag, 0 or 1. Default value is 1.
       IMWRITE_EXR_TYPE            = (3 << 4) + 0, /* 48 */ //!< override EXR storage type (FLOAT (FP32) is default)
       IMWRITE_EXR_COMPRESSION     = (3 << 4) + 1, /* 49 */ //!< override EXR compression type (ZIP_COMPRESSION = 3 is default)
       IMWRITE_WEBP_QUALITY        = 64, //!< For WEBP, it can be a quality from 1 to 100 (the higher is the better). By default (without any parameter) and for quality above 100 the lossless compression is used.
       IMWRITE_PAM_TUPLETYPE       = 128,//!< For PAM, sets the TUPLETYPE field to the corresponding string value that is defined for the format
       IMWRITE_TIFF_RESUNIT = 256,//!< For TIFF, use to specify which DPI resolution unit to set; see libtiff documentation for valid values
       IMWRITE_TIFF_XDPI = 257,//!< For TIFF, use to specify the X direction DPI
       IMWRITE_TIFF_YDPI = 258, //!< For TIFF, use to specify the Y direction DPI
       IMWRITE_TIFF_COMPRESSION = 259, //!< For TIFF, use to specify the image compression scheme. See libtiff for integer constants corresponding to compression formats. Note, for images whose depth is CV_32F, only libtiff's SGILOG compression scheme is used. For other supported depths, the compression scheme can be specified by this flag; LZW compression is the default.
       IMWRITE_JPEG2000_COMPRESSION_X1000 = 272 //!< For JPEG2000, use to specify the target compression rate (multiplied by 1000). The value can be from 0 to 1000. Default is 1000.
     };


int px_write_png_custom(const char* filepath, int height, int width, int channels, unsigned char* buffer, const std::vector<int>& params)
{
    bool m_buf = false;

    const int elemsize1 = 1; // for gray/rgb images, elemsize1=1.  for png16 images, elemsize1=2
    const size_t image_size = height * width * (elemsize1 * channels);
    void* image = buffer;
    size_t png_size;
    int ret = 0;
    FILE* volatile f = nullptr;
    

    /* Creating an encoder context requires a flag */
    spng_ctx *ctx = spng_ctx_new(SPNG_CTX_ENCODER);


    if (m_buf) 
    {
        // png_set_write_fn(png_ptr, this,
        //     (png_rw_ptr)writeDataToBuf, (png_flush_ptr)flushBuf);
        
        //spng_set_png_stream(ctx, writeDataToBuf, flushBuf);
    }
    else
    {
        f = fopen(filepath, "wb");
        if (f)
            spng_set_png_file(ctx, f);
    }

    /* Encode to internal buffer managed by the library */
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    /* Specify image dimensions, PNG format */
    struct spng_ihdr ihdr;
    ihdr.height = height;
    ihdr.width = width;
    ihdr.bit_depth = elemsize1 * 8;
    ihdr.color_type = channels == 1 ? SPNG_COLOR_TYPE_GRAYSCALE : channels == 3 ? SPNG_COLOR_TYPE_TRUECOLOR : SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    ihdr.compression_method = 0;
    ihdr.filter_method = 0;
    ihdr.interlace_method = 0;

    //-------------------------
    int compression_level = -1;
    int compression_strategy = IMWRITE_PNG_STRATEGY_RLE; // Default strategy
    bool isBilevel = false; // biBilevel is for png_set_packing(), which not supported by libspng now(2022.3.27)

    for( size_t i = 0; i < params.size(); i += 2 )
    {
        if( params[i] == IMWRITE_PNG_COMPRESSION )
        {
            compression_strategy = IMWRITE_PNG_STRATEGY_DEFAULT; // Default strategy
            compression_level = params[i+1];
            compression_level = std::min(std::max(compression_level, 0), (int)Z_BEST_COMPRESSION);
        }
        if( params[i] == IMWRITE_PNG_STRATEGY )
        {
            compression_strategy = params[i+1];
            compression_strategy = std::min(std::max(compression_strategy, 0), (int)Z_FIXED);
        }
        if( params[i] == IMWRITE_PNG_BILEVEL )
        {
            isBilevel = params[i+1] != 0;
        }
    }

    if (m_buf || f)
    {
        if( compression_level >= 0 )
        {
            //png_set_compression_level( png_ptr, compression_level );
            spng_set_option(ctx, SPNG_IMG_COMPRESSION_LEVEL, compression_level);
        }
        else
        {
            // tune parameters for speed
            // (see http://wiki.linuxquestions.org/wiki/Libpng)
            // png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
            spng_set_option(ctx, SPNG_FILTER_CHOICE, SPNG_FILTER_CHOICE_SUB);
            //png_set_compression_level(png_ptr, Z_BEST_SPEED);
            spng_set_option(ctx, SPNG_IMG_COMPRESSION_LEVEL, Z_BEST_SPEED);
        }
        //png_set_compression_strategy(png_ptr, compression_strategy);
        spng_set_option(ctx, SPNG_IMG_COMPRESSION_STRATEGY, compression_strategy);
        //-------------------------

        /* Image will be encoded according to ihdr.color_type, .bit_depth */
        ret = spng_set_ihdr(ctx, &ihdr);
        if (ret != 0)
        {
            PX_LOGE("spng_set_ihdr() error: %s\n", spng_strerror(ret));
        }

        // png_set_bgr(); // There is no replacement of `png_set_bgr()` in libspng. we have to manually implement it

        /* SPNG_FMT_PNG is a special value that matches the format in ihdr,
        SPNG_ENCODE_FINALIZE will finalize the PNG with the end-of-file marker */
        spng_format fmt = SPNG_FMT_PNG;
        ret = spng_encode_image(ctx, image, image_size, fmt, SPNG_ENCODE_FINALIZE);
        if (ret != 0) {
            PX_LOGE("spng_encode_image() error: %s\n", spng_strerror(ret));
        }
    }

    /* Free context memory */
    spng_ctx_free(ctx);

    fflush(f);
    if(f) fclose(f);

    return 0;
}

int px_write_png(const char* filepath, int height, int width, int channel, unsigned char* buffer)
{
    return px_write_png_custom(filepath, height, width, channel, buffer);
}



static const char *color_type_str(enum spng_color_type color_type)
{
    switch(color_type)
    {
        case SPNG_COLOR_TYPE_GRAYSCALE: return "grayscale";
        case SPNG_COLOR_TYPE_TRUECOLOR: return "truecolor";
        case SPNG_COLOR_TYPE_INDEXED: return "indexed color";
        case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA: return "grayscale with alpha";
        case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA: return "truecolor with alpha";
        default: return "(invalid)";
    }
}

void px_read_png(const char* filepath, int* height, int* width, int* channel, unsigned char** buffer)
{
    FILE* png;
    int ret = 0;
    spng_ctx* ctx = NULL;
    unsigned char *image = NULL;

    struct spng_ihdr ihdr;
    bool error = false;

    png = fopen(filepath, "rb");

    if(png == NULL)
    {
        PX_LOGE("error opening input file %s\n", filepath);
        return;
    }

    do {
        ctx = spng_ctx_new(0);

        if(ctx == NULL)
        {
            PX_LOGE("spng_ctx_new() failed\n");
            error = true;
            break;
        }

        /* Ignore and don't calculate chunk CRC's */
        spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

        /* Set memory usage limits for storing standard and unknown chunks,
        this is important when reading untrusted files! */
        size_t limit = 1024 * 1024 * 64;
        spng_set_chunk_limits(ctx, limit, limit);

        /* Set source PNG */
        spng_set_png_file(ctx, png); /* or _buffer(), _stream() */

        ret = spng_get_ihdr(ctx, &ihdr);
        
        if(ret)
        {
            PX_LOGE("spng_get_ihdr() error: %s\n", spng_strerror(ret));
            error = true;
            break;
        }

        const char *color_name = color_type_str((spng_color_type)ihdr.color_type);

        PX_LOGE("width: %u\n"
            "height: %u\n"
            "bit depth: %u\n"
            "color type: %u - %s\n",
            ihdr.width, ihdr.height, ihdr.bit_depth, ihdr.color_type, color_name);

        PX_LOGE("compression method: %u\n"
            "filter method: %u\n"
            "interlace method: %u\n",
            ihdr.compression_method, ihdr.filter_method, ihdr.interlace_method);

        *height = ihdr.height;
        *width = ihdr.width;

        if (ihdr.color_type == SPNG_COLOR_TYPE_GRAYSCALE)
        {
            *channel = 1;
        }
        else if (ihdr.color_type == SPNG_COLOR_TYPE_TRUECOLOR)
        {
            *channel = 3;
        }
        else if (ihdr.color_type == SPNG_COLOR_TYPE_TRUECOLOR_ALPHA)
        {
            *channel = 4;
        }
        else
        {
            PX_LOGE("not handled ihdr.color_type for png image");
            *channel = 0;
        }

        struct spng_plte plte = {0};
        ret = spng_get_plte(ctx, &plte);

        if(ret && ret != SPNG_ECHUNKAVAIL)
        {
            PX_LOGE("spng_get_plte() error: %s\n", spng_strerror(ret));
            error = true;
            break;
        }

        if(!ret) PX_LOGE("palette entries: %u\n", plte.n_entries);


        size_t image_size, image_width;

        /* Output format, does not depend on source PNG format except for
        SPNG_FMT_PNG, which is the PNG's format in host-endian or
        big-endian for SPNG_FMT_RAW.
        Note that for these two formats <8-bit images are left byte-packed */
        int fmt = SPNG_FMT_PNG;

        /* With SPNG_FMT_PNG indexed color images are output as palette indices,
        pick another format to expand them. */
        if(ihdr.color_type == SPNG_COLOR_TYPE_INDEXED) fmt = SPNG_FMT_RGB8;

        ret = spng_decoded_image_size(ctx, fmt, &image_size);

        if(ret)
        {
            error = true;
            break;
        }

        image = (unsigned char*)malloc(image_size);
        *buffer = image;

        if(image == NULL)
        {
            error = true;
            break;
        }

        /* Decode the image in one go */
        if (*channel == 4)
        {
            ret = spng_decode_image(ctx, image, image_size, SPNG_FMT_RGBA8, 0);
        }
        else if (*channel == 1)
        {
            ret = spng_decode_image(ctx, image, image_size, SPNG_FMT_G8, 0);
        }
        else if (*channel == 3)
        {
            ret = spng_decode_image(ctx, image, image_size, SPNG_FMT_RGB8, 0);
        }

        if(ret)
        {
            PX_LOGE("spng_decode_image() error: %s\n", spng_strerror(ret));
            error = true;
            break;
        }
    } while(0);

    spng_ctx_free(ctx);
}