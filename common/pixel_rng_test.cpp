#include "pixel_log.h"

#include "pixel_rng.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    
    PIXEL_SRAND(7767517);
    float val = pixel_random_float(-1.2, 233);
    printf("%.3f\n", val);

    float val2 = pixel_random_float(-1.2, 233);
    printf("%.3f\n", val2);


    return 0;
}