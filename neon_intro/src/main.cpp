#include <arm_neon.h>

#include <iostream>

using namespace std;

static void print_uint8x8(uint8x8_t data) {
    static uint8_t p[8];

    vst1_u8(p, data);
    for (int i=0; i<8; i++) {
        printf("%2d ", p[i]);
    }
    printf("\n");
}

static void print_uint8x16(uint8x16_t data) {
    static uint8_t p[16];

    vst1q_u8(p, data);
    for (int i=0; i<16; i++) {
        printf("%02d ", p[i]);
    }
    printf("\n");
}

int main() {

    uint8x8_t data;
    data = vdup_n_u8(255);
    print_uint8x8(data);

    const uint8_t value[] = {
        1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16
    };
    uint8x16_t data2 = vld1q_u8(value);
    print_uint8x16(data2);

    return 0;
}
