//#define _NJ_INCLUDE_HEADER_ONLY
#include "nanojpeg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

int main() {
    //const char* fn = "girl.jpg";
    const char* fn = "mingren.jpg";
    FILE* f = fopen(fn, "rb");
    fseek(f, 0, SEEK_END);
    int size = (int) ftell(f);
    char* buf = (char*) malloc(size);
    fseek(f, 0, SEEK_SET);
    size = (int) fread(buf, 1, size, f);
    fclose(f);

    njInit();
    if (njDecode(buf, size)) {
        free((void*)buf);
        printf("Error decoding the input file.\n");
        return 1;
    }
    free((void*)buf);

    cv::Size image_size;
    image_size.height = njGetHeight();
    image_size.width  = njGetWidth();
    cv::Mat image(image_size, CV_8UC3, njGetImage());
    cv::imshow("image", image);
    cv::waitKey(0);

    return 0;
}
