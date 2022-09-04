#include "pixel_log.h"

#include "pixel_rng.h"
#include "pixel_benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <random>

// reference C++11 style for generating random float number
// time cost and data distribution is similar to prng.h's
int cpp11_method() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(0.f, RAND_MAX * 1.0f);

#if _MSC_VER
    const int loop_count = RAND_MAX * 100;
#else
    const int loop_count = 32767 * 100;
#endif
    FILE* fd = fopen("out2.txt", "w");
    double t_start = pixel_get_current_time();
    for (int i = 0; i < loop_count; i++) {
        float val = dist(rng);
        fprintf(fd, "%.6f\n", val);
    }
    double t_cost = pixel_get_current_time() - t_start;
    printf("main2 time cost %.6lf ms\n", t_cost);
    fclose(fd);

    return 0;
}

int main() {
    
    PIXEL_SRAND(7767517);
    float val = pixel_random_float(-1.2, 233);
    printf("%.3f\n", val);

    float val2 = pixel_random_float(-1.2, 233);
    printf("%.3f\n", val2);


    return 0;
}