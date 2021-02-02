#include <stdlib.h>

#if __ARM_NEON
#include <arm_neon.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/pixel_log.h"
#include "common/pixel_benchmark.h"

typedef struct RGBImage {
    int h, w;
    unsigned char* data;
} RGBImage;

typedef struct GrayImage {
    int h, w;
    unsigned char* data;
} GrayImage;

void rgb_to_gray(RGBImage* rgb, GrayImage* gray) {
    int n = rgb->h * rgb->w;
    unsigned char* src = rgb->data;
    unsigned char* dst = gray->data;
    for (int i=0; i<n; i++) {
        int r = *src++;
        int g = *src++;
        int b = *src++;

        int y = (r*77) + (g*151) + (b*28);
        *dst++ = (y>>8);
    }
}

#if __ARM_NEON
void rgb_to_gray_neon(RGBImage* rgb, GrayImage* gray) {
    const int CHANNELS = 3;
    int height = rgb->h;
    int width = rgb->w;
    const int total_bytes = height * width * CHANNELS;
    const int stride_bytes = 48;
    const int left_bytes = total_bytes % stride_bytes;
    const int multiply_bytes = total_bytes - left_bytes;

    for (int i = 0; i < multiply_bytes; i += stride_bytes)
    {
        uint8_t *target = img + i;

        // swap R and B channel with NEON
        uint8x16x3_t a = vld3q_u8(target);
        uint8x16x3_t b;
        b.val[0] = a.val[2];
        b.val[1] = a.val[1];
        b.val[2] = a.val[0];
        vst3q_u8(target, b);
    }

    // handling non-multiply array lengths
    for (int i = multiply_bytes; i < total_bytes; i += CHANNELS)
    {
        const auto r_channel = *(img + i);
        *(img + i) = *(img + i + 2);
        *(img + i + 2) = r_channel;
    }
}
#endif // __ARM_NEON

RGBImage load_image(const char* filename)
{
    RGBImage image;
    int channels;
    image.data = stbi_load(filename, &image.w, &image.h, &channels, 0);

    PIXEL_LOGD("-- loaded image %s, height=%d, width=%d",
        filename, image.h, image.w);
    return image;
}

RGBImage copy_image(RGBImage* src_image)
{
    RGBImage copy;
    copy.h = src_image->h;
    copy.w = src_image->w;
    size_t buf_size = copy.h * copy.w * 3;
    copy.data = (unsigned char*)malloc(buf_size);
    memcpy(copy.data, src_image->data, buf_size);
    return copy;
}

void save_image(RGBImage* image, const char* filename)
{
    if (strlen(filename) < 5) {
        fprintf(stderr, "filename too short\n");
        return;
    }
    const char* ext = filename + strlen(filename) - 4;
    if(0==strcmp(ext, ".jpg")) {
        int quality = 100;
        stbi_write_jpg(filename, image->w, image->h, 3, image->data, quality);
    }
    else if(0==strcmp(ext, ".png")) {
        int stride_in_bytes = image->w * 3;
        stbi_write_png(filename, image->w, image->h, 3, image->data, stride_in_bytes);
    }
}

void perf_test()
{
    // assign trials with an odd number, thus validating if neon and none-neon impl matches
    // you may use winmerge / beyond compare to compare tow images
    const int trials = 1001;

    RGBImage image = load_image("000001.jpg");
    RGBImage gray1 = make_image(&image);
    RGBImage gray2 = make_image(&image);

    // Without NEON intrinsics
    // Invoke dotProduct and measure performance
    // int lastResult = 0;

    double t_start = pixel_get_current_time();
    for (int i = 0; i < trials; i++) {
        rgb_to_gray(&image, &gray1);
    }
    double elapsedTime = pixel_get_current_time() - t_start;
    save_image(&image_copy1, "none_neon.jpg");

#if __ARM_NEON
    // With NEON intrinsics
    // Invoke dotProductNeon and measure performance
    // int lastResultNeon = 0;
    start = pixel_get_current_time();
    for (int i = 0; i < trials; i++) {
        rgb_to_gray_neon(&image_copy3);
    }
    double elapsedTimeNeon = pixel_get_current_time() - t_start;
    save_image(&image_copy3, "neon.jpg");
#endif // __ARM_NEON

    PIXEL_LOGD("no neon impl time cost %lf ms", elapsedTime)

#if __ARM_NEON
    PIXEL_LOGD("neon impl time cost %lf ms", elapsedTimeNeon);
#endif

    free(image.data);
    free(image_copy1.data);
    free(image_copy2.data);

#if __ARM_NEON
    free(image_copy3.data);
#endif
}

int main(int, char*[])
{
    perf_test();

    return 0;
}