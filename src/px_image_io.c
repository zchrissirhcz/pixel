#include "px_image_io.h"
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

bool px_write_image(px_image_t* im, const char* filename)
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

    uint8_t* save_data = im->data;

    if (strcmp(ext, ".bmp") == 0)
    {
        stbi_write_bmp(filename, im->width, im->height, im->channel, save_data);
    }
    else if(strcmp(ext, ".jpg") == 0)
    {
        const int quality = 95; // or 100
        stbi_write_jpg(filename, im->width, im->height, im->channel, save_data, quality);
    }
    else if(strcmp(ext, ".png") == 0)
    {
        int stride_in_bytes = im->width * im->channel;
        stbi_write_png(filename, im->width, im->height, im->channel, save_data, stride_in_bytes);
    }
    else if (strcmp(ext, ".ppm") == 0)
    {
        px_write_ppm(filename, im->data, im->height, im->width);
    }
    else if (strcmp(ext, ".pgm") == 0)
    {
        px_write_pgm(filename, im->data, im->height, im->width);
    }

    return true;
}

