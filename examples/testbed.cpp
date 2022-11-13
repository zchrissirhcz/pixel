#include <stdio.h>
#include <string>
#include "px_image_io.h"
#include "px_image.h"
#include "px_digits.h"

int imageio_example()
{
    std::string image_path = "/Users/zz/Downloads/OpenCL异构并行计算随书源码/util/example/lena.ppm";
    px_image_t* image = px_read_image(image_path.c_str());
    px_image_t* image2 = px_create_image(image->height, image->width, image->channel);
    px_rgb_to_bgr(image, image2);
    px_write_image(image2, "lena.bmp");

    return 0;
}

int imageio_example2()
{
    px_image_t* image = px_read_image("lena.bmp");
    px_write_image(image, "lena.ppm");

    px_image_t* image2 = px_read_image("lena.ppm");
    px_write_image(image2, "lena2.bmp");

    return 0;
}

void histogram_example()
{
    std::string image_path = "lena.bmp";
    px_image_t* src = px_read_image(image_path.c_str());
    //px_image_t* hist = px_histogram(src);
    px_image_t* hist = px_histogram_rgb(src, PX_HISTOGRAM_GRAY);
    px_write_image(hist, "hist.png");

    px_destroy_image(src);
    px_destroy_image(hist);
}

void nv21_to_rgb_example()
{
    std::string image_path = "/Users/zz/data/1920x1080.NV21";
    FILE* fin = fopen(image_path.c_str(), "rb");
    const int width = 1920;
    const int height = 1080;
    const int bufsize = width * height * 3 / 2;
    uint8_t* buf = (uint8_t*)malloc(bufsize);
    fread(buf, bufsize, 1, fin);
    fclose(fin);

    px_image_t* y_plane = px_create_image_header(height, width, 1);
    y_plane->data = buf;
    px_image_t* uv_plane = px_create_image_header(height/2, width/2, 2);
    uv_plane->data = buf + height * width;

    px_image_t* rgb = px_create_image(height, width, 3);

    px_nv21_to_rgb(y_plane, uv_plane, rgb);

    px_rgb_to_nv21(rgb, y_plane, uv_plane);
    FILE* fout = fopen("1920x1080.NV21", "wb");
    fwrite(buf, bufsize, 1, fout);
    fclose(fout);

    px_image_t* bgr = px_create_image(height, width, 3);
    px_rgb_to_bgr(rgb, bgr);
    px_write_image(bgr, "1920x1080_v3.png");

    px_destroy_image_header(y_plane);
    px_destroy_image_header(uv_plane);
    px_destroy_image(bgr);
    px_destroy_image(rgb);
    free(buf);
}

void rotate270_example()
{
    std::string image_path = "lena.bmp";
    px_image_t* src = px_read_image(image_path.c_str());
    const int dst_height = src->width;
    const int dst_width = src->height;
    px_image_t* dst = px_create_image(dst_height, dst_width, src->channel);

    px_image_rotate270(src, dst);
    px_write_image(dst, "lena_rotate270.bmp");

    px_destroy_image(src);
    px_destroy_image(dst);
}

void sobel3x3_example()
{
    std::string image_path = "lena.bmp";
    px_image_t* src = px_read_image(image_path.c_str());

    px_image_t* gray = px_create_image(src->height, src->width, 1);
    px_rgb_to_gray(src, gray);

    const int dst_height = src->height;
    const int dst_width = src->width;
    px_image_t* dst = px_create_image(dst_height, dst_width, 1);

    px_sobel3x3(gray, dst);
    px_write_image(dst, "lena_sobel3x3.jpg");

    px_destroy_image(src);
    px_destroy_image(dst);
}

void digit_test()
{
    int digits[8];
    int x = -128;
    px_decimal_to_binary_signed(x, digits, 8);
    for (int i = 0; i < 8; i++)
    {
        printf("%d, ", digits[i]);
    }
    printf("\n");
}

int main()
{
    //imageio_example();
    //histogram_example();
    //rotate270_example();
    //nv21_to_rgb_example();
    //sobel3x3_example();
    digit_test();

    return 0;
}
