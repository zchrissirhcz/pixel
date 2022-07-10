#include <stdio.h>
#include <arm_neon.h>

int main()
{
    int8x16_t v1 = {-1, 1, -2, 2, -4, 4, -8, 8, -16, 16, -32, 32, -64, 64, -128, 127};
    int8x16_t v2 = vshrq_n_s8(v1, 3);
    // -1, 0, -1, 0, -1, 0, -1, 1, -2, 2, -4, 4, -8, 8, -16, 15, 

    // -128: 10000000
    //    11110000
    // -16:  11110000

    int8_t res[16];
    vst1q_s8(res, v2);

    for (int i = 0; i < 16; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");

    return 0;
}