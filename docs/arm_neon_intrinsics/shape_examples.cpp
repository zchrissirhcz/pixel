#include <stdio.h>
#include <arm_neon.h>

// https://developer.arm.com/documentation/den0018/a/NEON-Instruction-Set-Architecture/Instruction-syntax?lang=en
// <shape> is a modifier:
//  L(Long)
//  W(Wide)
//  N(Narrow)


// https://developer.arm.com/documentation/den0018/a/NEON-Instruction-Set-Architecture/Instruction-syntax/Instruction-shape?lang=en

// Shape modifier `None spcified`
// Both operands and rsults are the same width
void shape_None_example()
{
    uint8x8_t v1 = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8x8_t v2 = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8x8_t v3 = vadd_u8(v1, v2);
    // 0, 2, 4, 6, 8, 10, 12, 14, 

    uint8_t res[8];
    vst1_u8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%u, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// Shape modifier `Long-L`
// Operands are the same width. Number of bits in each result element is double the number of bits in each operand element.
void shape_L_example()
{
    int8x8_t v1 = {0, 1, 2, 3, 4, 5, 6, 7};
    int8x8_t v2 = {120, 121, 122, 123, 124, 125, 126, 127};
    int16x8_t v3 = vaddl_s8(v1, v2);
    // 120, 122, 124, 126, 128, 130, 132, 134, 

    int16_t res[8];
    vst1q_s16(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// Shape modifier `Narrow-N`
// Operands are the same width. Number of bits in each result element is half the number of bits in each operand element.
void shape_N_example()
{
    uint16x8_t v1 = {250, 251, 252, 253, 254, 255, 256, 257};
    uint16x8_t v2 = {1, 1, 1, 1, 1, 1, 1, 1};
    uint8x8_t v3 = vaddhn_u16(v1, v2);
    // 0, 0, 0, 0, 0, 1, 1, 1, 

    uint8_t res[8];
    vst1_u8(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%d, ", res[i]);
    }
    fprintf(stderr, "\n");
}

// Shape modifier `Wide-W`
// Result and operand are twice the width of the second operand.
void shape_W_example()
{
    // VADDW.I16 Q0, Q1, D4
    uint16x8_t v1 = {256, 257, 258, 259, 260, 261, 262, 263};
    uint8x8_t v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    uint16x8_t v3 = vaddw_u16(v1, v2);
    // 769, 257, 1285, 259, 1801, 261, 2317, 263,

    uint16_t res[8];
    vst1q_u16(res, v3);

    for (int i = 0; i < 8; i++)
    {
        fprintf(stderr, "%u, ", res[i]);
    }
    fprintf(stderr, "\n");
}

int main()
{
    //shape_None_example();
    //shape_L_example();
    //shape_N_example();
    shape_W_example();

    return 0;
}