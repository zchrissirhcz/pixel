#include "px_digits.h"
#include "px_log.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void px_decimal_to_binary_unsigned(unsigned x, int* bits, int len)
{
    for (int i = 0; i < len; i++)
    {
        bits[len - 1 - i] = x & 1;
        x = x >> 1;
    }
}

unsigned px_binary_to_decimal_unsigned(int* bits, int len)
{
    unsigned Q = 0;
    for (int i = 0; i < len; i++)
    {
        int flag = (bits[len - 1 - i] << i);
        Q = Q | flag;
    }
    return Q;
}

/// @param bits 原码(true code)
/// @param res 补码(2's complement)
void px_get_2s_complement(int* bits, int len, int* res)
{
    int carry = 1;
#if BITS_DEBUG
    printf("! getting 2's complement:\n");
#endif
    for (int i = 0; i < len; i++)
    {
        int idx = len - 1 - i;
        int t = (1 - bits[idx] + carry);
        res[idx] = t % 2;
        carry = t / 2;
#if BITS_DEBUG
        printf("    i=%d, idx=%d, res[%d]=(1-%d+%d)=%d\n", 
            i, idx, idx, bits[idx], carry, res[idx]
        );
#endif
    }
}

void px_decimal_to_binary_signed(int n, int* bits, int len)
{
    for (int i = 0; i < len; i++)
    {
        bits[i] = (n >> (len - 1 - i)) & 1;
    }
}

void px_sign_extension(int* Q, int sign, int len, int total_len)
{
    if (sign == 1)
    {
        for (int i = len; i < total_len; i++)
        {
            *Q = *Q | (1 << i);
        }
    }
    else if (sign == 0)
    {
        for (int i = len; i < total_len; i++)
        {
            *Q = *Q & (~(1 << i));
        }
    }
}

int px_binary_to_decimal_signed(int* bits, int len)
{
    int Q = 0;
    for (int i = 0; i < len; i++)
    {
        int flag = (bits[len - 1 - i] << i);
        Q = Q | flag;
    }
    int sign = bits[0];
    px_sign_extension(&Q, sign, len, 32);
    return Q;
}

int px_binary_to_decimal_signed_simulate(int* bits, int len)
{
    int p = 1;
    int res = 0;

    bool negative = false;
    int* b = bits;
    if (bits[0] == 1)
    {
        negative = true;
        int* complements = (int*)malloc(sizeof(int) * len);
        px_get_2s_complement(bits, len, complements);
        b = complements;
    }
    for (int i = 0; i < len; i++)
    {
        int idx = len - 1 - i;
        res += b[idx] * p;
        p *= 2;
    }
    if (negative)
    {
        res *= -1;
        free(b);
    }

    return res;
}

void px_binary_add(int* bits1, int* bits2, int* bits_result, int len)
{
    int* b1 = bits1;
    bool negative1 = false;
    if (bits1[0] < 0)
    {
        negative1 = true;
        b1 = (int*)malloc(sizeof(int) * len);
        px_get_2s_complement(bits1, len, b1);
    }

    int* b2 = bits2;
    bool negative2 = false;
    if (bits2[0] < 0)
    {
        negative2 = true;
        b2 = (int*)malloc(sizeof(int) * len);
        px_get_2s_complement(bits2, len, b2);
    }

    int carry = 0;
    for (int i = 0; i < len; i++)
    {
        int idx = len - 1 - i;
        int t = bits1[idx] + bits2[idx] + carry;
        bits_result[idx] = (t & 1);
        carry = (t >> 1);
    }

    if (negative1)
    {
        free(b1);
    }
    if (negative2)
    {
        free(b2);
    }
}


void px_decimal_to_binary_f32(float N, int* bits)
{
    int Q = *(int*)(&N);
    for (int i = 0; i < 32; i++)
    {
        bits[32 - 1 - i] = Q & 1;
        Q = Q >> 1;
    }
}

float px_binary_to_decimal_f32(int* bits)
{
    int Q;
    memset(&Q, 0, sizeof(int));
    for (int i = 0; i < 32; i++)
    {
        Q = Q | (bits[i] << (32 - 1 - i));
    }
    float res = *(float*)&Q;
    return res;
}