#include <stdio.h>
#include <string>
#include "px_image_io.h"
#include "px_image.h"

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

int main()
{
    imageio_example();
    histogram_example();
    rotate270_example();
}