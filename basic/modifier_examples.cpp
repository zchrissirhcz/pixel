#include <stdio.h>
#include <arm_neon.h>

// `None` modifier means only do basic operation
void modifier_None_example()
{
    int8x8_t v1 = {0, 1, 2, 3, 4, 5, 6, 7};
    int8x8_t v2 = {120, 121, 122, 123, 124, 125, 126, 127};
    int8x8_t v3 = vadd_s8(v1, v2);
    // 120, 122, 124, 126, -128, -126, -124, -122,

    int8_t res[8];
    vst1_s8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// https://developer.arm.com/documentation/den0018/a/NEON-Instruction-Set-Architecture/Instruction-syntax/Instruction-modifiers?lang=en
// Q mean Saturation
// set break point before and after `vqadd_s8`, dump `FPSR` register's QC bit
void modifier_Q_example()
{
    int8x8_t v1 = {0, 1, 2, 3, 4, 5, 6, 7};
    int8x8_t v2 = {120, 121, 122, 123, 124, 125, 126, 127};
    int8x8_t v3 = vqadd_s8(v1, v2);
    // 120, 122, 124, 126, 127, 127, 127, 127, 

    int8_t res[8];
    vst1_s8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// `H` modifier means Halved
// Each element shifted right by one place (effectively a divide by two with truncation). VHADD can be used to calculate the mean of two inputs.
void modifier_H_example()
{
    int8x8_t v1 = {0, 1, 2, 3, 4, 5, 6, 7};
    int8x8_t v2 = {120, 121, 122, 123, 124, 125, 126, 127};
    int8x8_t v3 = vhadd_s8(v1, v2);
    // 60, 61, 62, 63, 64, 65, 66, 67, 

    int8_t res[8];
    vst1_s8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// `D` modifier for "double"
// `QD` means "dobule before saturation"
// vqdmull: the last `l` means each lane in result will be 2x wider than the input
void modifier_D_example()
{
    int16x4_t v1 = {-128, 60, 120, 127};
    int16x4_t v2 = {-128, 60, 120, 127};
    int32x4_t v3 = vqdmull_s16(v1, v2);
    // 32768, 7200, 28800, 32258, 

    int32_t res[4];
    vst1q_s32(res, v3);

    for (int i = 0; i < 4; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// modifier `R` for rounding
// i.e. the non-rounding one: (a + b) >> 8
//      the rounding one:     (a + b + (1<<7)) >> 8
void modifier_R_example()
{
    uint16x8_t v1 = {300, 300, 300, 300, 300, 300, 300, 300};
    uint16x8_t v2 = {10, 100, 1000, 10000, 10, 100, 1000, 10000};
    // uint8x8_t v3 = vsubhn_u16(v1, v2);
    // 1, 0, 253, 218, 1, 0, 253, 218, 

    uint8x8_t v3 = vrsubhn_u16(v1, v2);
    // 1, 1, 253, 218, 1, 1, 253, 218, 

    uint8_t res[8];
    vst1_u8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%u, ", res[i]);
    }
    fprintf(stderr, "\n");

    uint16_t v1_data[8];
    vst1q_u16(v1_data, v1);

    uint16_t v2_data[8];
    vst1q_u16(v2_data, v2);

    uint8_t v3_data[8];
    const uint16_t shift = 8;
    const uint16_t delta = (1 << (shift -1));
    fprintf(stderr, "v3_data:\n");
    for (int i = 0; i < 8; i++)
    {
        v3_data[i] = (v1_data[i] - v2_data[i] + delta) >> shift;
        fprintf(stderr, "%u, ", v3_data[i]);
    }
    fprintf(stderr, "\n");
}

int main()
{
    modifier_None_example();
    modifier_Q_example();
    modifier_H_example();
    modifier_D_example();
    modifier_R_example();

    return 0;
}