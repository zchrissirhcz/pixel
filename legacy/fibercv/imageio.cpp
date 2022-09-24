#include "imageio.h"

/*
.bmp: stb_image
.jpg: stb_image
.png: stb_image

.ppm: https://github.com/Tencent/ncnn/blob/master/src/opencv.cpp
.pgm: https://github.com/Tencent/ncnn/blob/master/src/opencv.cpp

*/


#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#if 0
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#endif


namespace fc {

    void imwrite_ppm(const char* filename, Image* im);
    void imwrite_pgm(const char* filename, Image* im);

    unsigned char* imread_ppm(const char* filename, int* width, int* height, int* channel);
    unsigned char* imread_pgm(const char* filename, int* width, int* height, int* channel);

    unsigned char* imread_bmp(const char* filename, int* _width, int* _height, int* _channel);
    void imwrite_bmp(const char* filename, int width, int height, int channel,
                     const unsigned char* data, const int src_align = 1);

    // load an image as BGR order
Image* imread(const char* filename) {
    if (strlen(filename) < 5) {
        fprintf(stderr, "filename too short\n");
        return NULL;
    }

    const char* ext = filename + strlen(filename) - 4;
    if(strcmp(ext, ".bmp")!=0 &&
            strcmp(ext, ".jpg")!=0 &&
            strcmp(ext, ".png")!=0 &&
            strcmp(ext, ".ppm")!=0 &&
            strcmp(ext, ".pgm")!=0
        ) {
        fprintf(stderr, "not supported extension %s\n", ext);
        return NULL;
    }

    int width, height, component;
    unsigned char* data = NULL;
    if(0 == strcmp(ext, ".bmp")) {
        data = imread_bmp(filename, &width, &height, &component);
    }
    else if (0 == strcmp(ext, ".png") || 0 == strcmp(ext, ".jpg")) {
        data = stbi_load(filename, &width, &height, &component, 0);
        if (component == 4) {
            component = 3;
            Image* image_tmp = make_image_ptr(width, height, component);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int src_idx = i * width * 4 + j * 4;
                    int dst_idx = i * width * 3 + j * 3;
                    image_tmp->data[dst_idx] = data[src_idx];
                    image_tmp->data[dst_idx+1] = data[src_idx+1];
                    image_tmp->data[dst_idx+2] = data[src_idx+2];
                }
            }
            unsigned char* shadow = data;
            free(shadow);
            data = image_tmp->data;
            free(image_tmp);
        }
    }
    else if (0 == strcmp(ext, ".ppm")) {
        data = imread_ppm(filename, &width, &height, &component);
    }
    else if (0 == strcmp(ext, ".pgm")) {
        data = imread_pgm(filename, &width, &height, &component);
    }
    if (data == NULL) {
        fprintf(stderr, "file %s not exist or failed to load\n", filename);
        return NULL;
    }
    Image* image = make_image_ptr(width, height, component, data);

    if(0 == strcmp(ext, ".png") || 0 == strcmp(ext, ".jpg") ||
        0 == strcmp(ext, ".ppm") || 0 == strcmp(ext, ".pgm"))
    {
        rgb_bgr_swap(image);
    }
    return image;
}

void imwrite(const char* filename, Image* im)
{
    if (strlen(filename) < 5) {
        fprintf(stderr, "filename too short\n");
        return;
    }
    const char* ext = filename + strlen(filename) - 4;
    if(strcmp(ext, ".bmp")!=0 &&
            strcmp(ext, ".jpg")!=0 &&
            strcmp(ext, ".png")!=0 &&
            strcmp(ext, ".ppm")!=0 &&
            strcmp(ext, ".pgm")!=0
        ) {
        fprintf(stderr, "not supported extension %s\n", ext);
        return;
    }

    if(0==strcmp(ext, ".bmp")) {
        // rgb_bgr_swap(im); // BRG->RGB
        // stbi_write_bmp(filename, im->width, im->height, im->channel, im->data);
        // rgb_bgr_swap(im);
        imwrite_bmp(filename, im->width, im->height, im->channel, im->data, im->align);
    } 
    else if(0==strcmp(ext, ".jpg")) {
        rgb_bgr_swap(im); // BRG->RGB
        int quality = 100;
        stbi_write_jpg(filename, im->width, im->height, im->channel, im->data, quality);
        rgb_bgr_swap(im);
    } 
    else if(0==strcmp(ext, ".png")) {
        rgb_bgr_swap(im); // BRG->RGB
        int stride_in_bytes = im->linebytes;
        stbi_write_png(filename, im->width, im->height, im->channel, im->data, stride_in_bytes);
        rgb_bgr_swap(im);
    }
    else if (0 == strcmp(ext, ".ppm")) {
        rgb_bgr_swap(im); // BRG->RGB
        imwrite_ppm(filename, im);
        rgb_bgr_swap(im);
    }
    else if (0 == strcmp(ext, ".pgm")) {
        rgb_bgr_swap(im); // BRG->RGB
        imwrite_pgm(filename, im);
        rgb_bgr_swap(im);
    }
}

void imwrite_ppm(const char* filename, Image* im)
{
    FILE* fp = fopen(filename, "wb");
    if (!fp)
        return;

    assert(im->channel == 3);
    fprintf(fp, "P6\n%d %d\n255\n", im->width, im->height);
    fwrite(im->data, 1, im->mass, fp);

    fclose(fp);
}

void imwrite_pgm(const char* filename, Image* im)
{
    FILE* fp = fopen(filename, "wb");
    if (!fp)
        return;

    assert(im->channel == 1);
    fprintf(fp, "P5\n%d %d\n255\n", im->width, im->height);
    fwrite(im->data, 1, im->mass, fp);

    fclose(fp);
}

unsigned char* imread_ppm(const char* filename, int* width, int* height, int* channel)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return NULL;

    char magic[3];
    int nscan = fscanf(fp, "%2s\n%d %d\n255\n", magic, width, height);
    if (nscan == 3 && magic[0] == 'P' && (magic[1] == '6')) {
        *channel = 3;
        Image image = make_image(*width, *height, *channel);
        fread(image.data, 1, image.mass, fp);
        return image.data;
    }
    return NULL;
}

unsigned char* imread_pgm(const char* filename, int* width, int* height, int* channel)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return NULL;

    char magic[3];
    int nscan = fscanf(fp, "%2s\n%d %d\n255\n", magic, width, height);
    if (nscan == 3 && magic[0] == 'P' && (magic[1] == '5')) {
        *channel = 1;
        Image image = make_image(*width, *height, *channel);
        fread(image.data, 1, image.mass, fp);
        return image.data;
    }
    return NULL;
}


}