#include "px_digits.h"
#include <stdbool.h>

void px_reverse_bits(int* digits, int len)
{
    const int half_len = len >> 1;
    for (int i = 0; i < half_len; i++)
    {
        int t = digits[i];
        digits[i] = digits[len-1-i];
        digits[len-1-i] = t;
    }
}

void px_decimal_to_binary_s8(int8_t x, int* digits)
{
    const int len = 8;
    for (int i = 0; i < len; i++)
    {
        digits[len - 1 - i] = x & 1;
        x = x >> 1;
    }
}

int8_t px_binary_to_decimal_s8(int* bits)
{
    int* b = bits;

    const int len = 8;
    px_reverse_bits(bits, len);

    int8_t res = 0;

    bool negative_sign = (b[len-1] == 1);

    int p = 1;
    int carry = 1;
    int digit;
    for (int i = 0; i < len-1; i++)
    {
        if (negative_sign)
        {
            digit = 1 - b[i] + carry;
            if (digit == 2)
            {
                carry = 1;
                digit = 0;
            }
            else
            {
                carry = 0;
            }
        }
        else
        {
            digit = b[i];
        }
        res += digit * p;
        p *= 2;
        //printf("%d * pow(2, %d)\n", digit, i);
    }

    if (negative_sign)
    {
        res *= -1;
    }
    
    return res;
}
