#include <iostream>

#include <arm_neon.h>

using namespace std;

static void print_uint8x8(uint8x8_t data) {
    static uint8_t p[8];

    // void vst1_u8 (uint8_t * ptr, uint8x8_t val)
    vst1_u8(p, data);
    for (int i=0; i<8; i++) {
        printf("%2d ", p[i]);
    }
    printf("\n");
}

static void print_uint8x16(uint8x16_t data) {
    static uint8_t p[16];

    // void vst1q_u8 (uint8_t * ptr, uint8x16_t val)
    vst1q_u8(p, data);
    for (int i=0; i<16; i++) {
        printf("%02d ", p[i]);
    }
    printf("\n");
}

static void print_uint8x8x2(uint8x8x2_t data) {
    static uint8_t p[16];

    // void vst1_u8_x2 (uint8_t * ptr, uint8x8x2_t val)
    vst1_u8_x2(p, data);
    for (int i=0; i<16; i++) {
        printf("%02d ", p[i]);
    }
    printf("\n");
}

void test_vdup() {
    printf("----------------------------------------\n");
    printf("show usage of vdup related intrinsics\n");
    printf("----------------------------------------\n");

    // vdup_n_u8: 用类型为u8的值，初始化一个长度为8、元素类型为u8的向量
    // uint8x8_t vdup_n_u8 (uint8_t value)
    uint8x8_t data1 = vdup_n_u8(255);
    print_uint8x8(data1);

    // vmov_n_u8: 用类型为u8的值，初始化一个长度为8、元素类型为u8的向量 （等效于vdup_n_u8）
    // uint8x8_t vmov_n_u8 (uint8_t value)
    uint8x8_t data2 = vmov_n_u8(255);
    print_uint8x8(data2);

    // vdupq_n_u8: 用类型为u8的值，初始化一个长度为16、元素类型为u8的向量
    // uint8x16_t vdupq_n_u8 (uint8_t value)
    uint8x16_t data3 = vdupq_n_u8(255);
    print_uint8x16(data3);

    printf("\n");
}

void test_vld() {
    printf("----------------------------------------\n");
    printf("show usage of vld related intrinsics\n");
    printf("----------------------------------------\n");

    const uint8_t value[] = {
        1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        9, 10, 11, 12, 13, 14, 15, 16
    };

    // vld1_u8: 将u8类型的数组value，拷贝前8个元素到 长度为8、元素类型为u8的向量
    // uint8x8_t vld1_u8 (uint8_t const * ptr)
    uint8x8_t data1 = vld1_u8(value);
    print_uint8x8(data1);

    // vld1q_u8: 将u8类型的数组value，拷贝到一个长度为16、元素类型为u8的向量
    // uint8x16_t vld1q_u8 (uint8_t const * ptr)
    // 说明：如果value长度不足16，则只拷贝有效部分，其他置0；如果value超过16元素，则只拷贝前16元素
    uint8x16_t data2 = vld1q_u8(value);
    print_uint8x16(data2);

    // vld1_u8_x2:
    // uint8x8x2_t vld1_u8_x2 (uint8_t const * ptr)
    // 位置:arm64_neon.h 说明它是arm64才有的指令
    uint8x8x2_t data3 = vld1_u8_x2(value);
    print_uint8x8x2(data3);

    printf("\n");
}

static void test_add() {
    printf("----------------------------------------\n");
    printf("show usage of vadd related intrinsics\n");
    printf("----------------------------------------\n");

    uint8x16_t data = vdupq_n_u8(230);
    uint8x16_t offset = vmovq_n_u8(3);
    data = vaddq_u8(data, offset);
    print_uint8x16(data);
}

int main() {
    test_vdup();

    test_vld();

    test_add();

    return 0;
}
