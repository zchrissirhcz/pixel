#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <common/pixel_benchmark.h>
#include <algorithm> // for std::fill_n

#include "array_fill.h"

int main() {

    //size_t len = 1000000;
    size_t len = 224*224*3;
    float* data = (float*)malloc(sizeof(float) * len);
    double t_start, t_cost;

    t_start = pixel_get_current_time();
    std::fill_n(data, len, 3.14);
    t_cost = pixel_get_current_time() - t_start;
    printf("assign %zu float elements, std::fill_n,     cost %.2lf ms\n", len, t_cost);

    t_start = pixel_get_current_time();
    array_fill_f32_naive(data, len, 6.28);
    t_cost = pixel_get_current_time() - t_start;
    printf("assign %zu float elements, naive assign,    cost %.2lf ms\n", len, t_cost);

    t_start = pixel_get_current_time();
    array_fill_f32_asimd(data, len, 9.42);
    t_cost = pixel_get_current_time() - t_start;
    printf("assign %zu float elements, asimd,           cost %.2lf ms\n", len, t_cost);

    return 0;
}