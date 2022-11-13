#pragma once

#include <stdint.h>
#include <type_traits>
#include "px_log.h"

#ifdef __cplusplus
extern "C" {
#endif

void px_get_2s_complement(int* bits, int len, int* res);

// for signed
int px_binary_to_decimal_signed(int* bits, int len);
void px_decimal_to_binary_signed(int n, int* bits, int len);
int px_binary_to_decimal_signed_simulate(int* bits, int len);

// for unsigned
unsigned px_binary_to_decimal_unsigned(int* bits, int len);
void px_decimal_to_binary_unsigned(unsigned n, int* bits, int len);

void px_binary_add(int* bits1, int* bits2, int* bits_result, int len);

/// @param bits >=32 element array
void px_decimal_to_binary_f32(float N, int* bits);
/// @param bits >=32 element array
float px_binary_to_decimal_f32(int* bits);

#ifdef __cplusplus
}
#endif

// ex 2.37 of "Computer System Introduction" (2nd edition)
// 对于 signed 类型， 根据符号位判断， 只有如下三种情况对应到溢出
// m n s
// 0 0 1
// 1 1 0
//
// ex 2.38 of "Computer System Introduction" (2nd edition)
// 对于 unsigned 类型， 根据符号位判断，只有如下几种情况对应到溢出
// m n s
// 0 1 0
// 1 0 0
// 1 1 0
// 1 1 1
template<typename T>
inline bool px_determine_add_overflow(T m, T n)
{
    if (std::is_integral<T>())
    {
        T s = m + n;
        const int mask = (1 << (sizeof(T) * 8 - 1));
        if (std::is_signed<T>())
        {
            return (((n & m & ~s) | (~n & ~m & s)) & mask);
        }
        else
        {
            return (((n & m) | ((n | m) & ~s)) & mask);
        }
    }
    else
    {
        PX_LOGE("not supported yet\n");
        return false;
    }
}
