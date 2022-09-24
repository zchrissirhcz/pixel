#include "px_image_io.h"
#include "px_bmp.h"
#include "px_image.h"
#include "px_log.h"
#include "px_filesystem.h"

// #include "zcnn_convert_color.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "px_ppm_pgm.h"

px_image_t* px_read_image(const char* filename)
{
    char* ext = strstr(filename, ".");
    if (strcmp(ext, ".bmp")==0)
    {
        int height;
        int width;
        int channel;
        uint8_t* buffer;
        px_read_bmp(filename, &height, &width, &channel, &buffer);
        px_image_t* image = px_create_image_header(height, width, channel);
        image->data = buffer;
        return image;
    }

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data)
    {
        PX_LOGE("Cannot load image \"%s\"\nSTB Reason: %s\n",
            filename, stbi_failure_reason());
    }
    
    px_image_t* image = px_create_image_header(height, width, channels);
    image->height = height;
    image->width = width;
    image->channel = channels;
    image->data = data;

    printf("!! channel = %d\n", channels);

    if (strcmp(ext, ".ppm") == 0)
    {
        return image;
    }

    if (image->channel == 3)
    {
        px_rgb2bgr_inplace(image);
    }
    else if (image->channel == 4)
    {
        px_rgba2bgra_inplace(image);
    }

    return image;
}

static bool is_valid_image_extension(const char* ext)
{
    if(strcmp(ext, ".bmp") == 0 || strcmp(ext, ".jpg") == 0 || strcmp(ext, ".png") == 0 || strcmp(ext, ".ppm") == 0 || strcmp(ext, ".pgm") == 0)
    {
        return true;
    }
    return false;
}

static px_image_t* transform_for_stb_order(px_image_t* im0)
{
    px_image_t* im = im0;
    if (im0->channel == 3)
    {
        im = px_create_image(im0->height, im0->width, im0->channel);
        px_rgb2bgr(im0, im);
    }
    else if (im0->channel == 4)
    {
        im = px_create_image(im0->height, im0->width, im0->channel);
        px_rgba2bgra(im0, im);
    }
    return im;
}

bool px_write_image(px_image_t* im0, const char* filename)
{
    if (strlen(filename) < 5)
    {
        PX_LOGE("failed to save image: filename too short to be valid\n");
        return false;
    }

    if (px_get_file_or_directory_state(filename) != PX_FILE_NOT_EXIST)
    {
        //PX_LOGE("failed to save image: filename already exist\n");
        //return false;
        PX_LOGE("warning: overwriting existing file");
    }

    const char* ext = filename + strlen(filename) - 4;

    bool valid_ext = is_valid_image_extension(ext);
    if (!valid_ext)
    {
        PX_LOGE("failed to save image: not supported extension %s\n", ext);
        return false;
    }

    px_image_t* im = im0;
    if (strcmp(ext, ".bmp") == 0)
    {
        //stbi_write_bmp(filename, im->width, im->height, im->channel, im->data);
        px_write_bmp(filename, im0->height, im0->width, im0->channel, im0->data);
    }
    else if(strcmp(ext, ".jpg") == 0)
    {
        const int quality = 95; // or 100
        im = transform_for_stb_order(im0);
        stbi_write_jpg(filename, im->width, im->height, im->channel, im->data, quality);
    }
    else if(strcmp(ext, ".png") == 0)
    {
        int stride_in_bytes = im->width * im->channel;
        im = transform_for_stb_order(im0);
        stbi_write_png(filename, im->width, im->height, im->channel, im->data, stride_in_bytes);
    }
    else if (strcmp(ext, ".ppm") == 0)
    {
        px_write_ppm(filename, im->data, im->height, im->width);
    }
    else if (strcmp(ext, ".pgm") == 0)
    {
        px_write_pgm(filename, im->data, im->height, im->width);
    }

    if (im != im0)
    {
        px_destroy_image(im);
    }

    return true;
}

