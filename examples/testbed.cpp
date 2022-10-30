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

int main()
{
    histogram_example();
}