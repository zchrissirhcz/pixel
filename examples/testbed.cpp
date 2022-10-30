#include <stdio.h>
#include <string>
#include "px_image_io.h"

int main2()
{
    std::string image_path = "/Users/zz/Downloads/OpenCL异构并行计算随书源码/util/example/lena.ppm";
    px_image_t* image = px_read_image(image_path.c_str());
    px_image_t* image2 = px_create_image(image->height, image->width, image->channel);
    px_rgb2bgr(image, image2);
    px_write_image(image2, "lena.bmp");

    return 0;
}

int main()
{
    px_image_t* image = px_read_image("lena.bmp");
    px_write_image(image, "lena.ppm");

    px_image_t* image2 = px_read_image("lena.ppm");
    px_write_image(image2, "lena2.bmp");

    return 0;
}