#include "zcnn_image_io.h"
#include "zcnn_file.h"
#include "zcnn_log.h"
#include "zcnn_file.h"
#include "zcnn_convert_color.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

image_t* load_image(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        ZCNN_LOGE("Cannot load image \"%s\"\nSTB Reason: %s\n",
            filename, stbi_failure_reason());
    }
    
    image_t* image = (image_t*)malloc(sizeof(image_t));
    image->height = height;
    image->width = width;
    image->channels = channels;
    image->data = data;
    return image;
}

bool save_image(image_t* im, const char* filename)
{
    if (strlen(filename) < 5)
    {
        ZCNN_LOGE("failed to save image: filename too short to be valid\n");
        return false;
    }

    if (get_file_or_directory_state(filename) != FILE_NOT_EXIST)
    {
        ZCNN_LOGE("failed to save image: filename already exist\n");
        return false;
    }

    const char* ext = filename + strlen(filename) - 4;

    if(strcmp(ext, ".bmp")!=0 && strcmp(ext, ".jpg")!=0 && strcmp(ext, ".png")!=0) {
        ZCNN_LOGE("failed to save image: not supported extension %s\n", ext);
        return false;
    }

    uchar* save_data = im->data;
    if (im->channels==3)
    {
        // bgr => rgb
        // image_t* bgr = im;
        // image_t rgb;
        // rgb.height = im->height;
        // rgb.width = im->width;
        // rgb.channels = im->channels;
        // size_t buf_size = rgb.height * rgb.width * rgb.channels;
        // rgb.data = (uchar*)malloc(buf_size);
        // convert_bgr_to_rgb(bgr, &rgb);
        // save_data = rgb.data;
    }

    if (strcmp(ext, ".bmp") == 0)
    {
        stbi_write_bmp(filename, im->width, im->height, im->channels, save_data);
    }
    else if(strcmp(ext, ".jpg") == 0)
    {
        const int quality = 95; // or 100
        stbi_write_jpg(filename, im->width, im->height, im->channels, save_data, quality);
    }
    else if(strcmp(ext, ".png") == 0) {
        int stride_in_bytes = im->width * im->channels;
        stbi_write_png(filename, im->width, im->height, im->channels, save_data, stride_in_bytes);
    }

    if (im->channels == 3)
    {
        //free(save_data);
    }

    return true;
}

