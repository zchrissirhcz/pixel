#include <stdio.h>
#include <arm_neon.h>

int main()
{
    int8x8_t v1 = {0, 1, 2, 3, 4, 5, 6, 7};
    int8x8_t v2 = {120, 121, 122, 123, 124, 125, 126, 127};
    int8x8_t v3 = vqadd_s8(v1, v2);

    int8_t res[8];
    vst1_s8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");

    return 0;
}
