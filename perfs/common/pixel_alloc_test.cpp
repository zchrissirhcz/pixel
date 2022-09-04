#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pixel_alloc.h"

int main() {

    float* data = (float*)pixel_fast_malloc(10*sizeof(float));
    pixel_fast_free(data);

    return 0;
}