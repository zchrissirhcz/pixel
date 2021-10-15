// https://stackoverflow.com/questions/69556488/reference-implementation-of-vrecpeq-f32-intrinsic/69558565#69558565
// https://gcc.godbolt.org/z/j89cK9hnc

#include <iostream>
#include <cstring>
#include <iomanip>

// Convenience struct to deal with encoding and decoding ieee754 floats
struct float_parts {
    explicit float_parts(float v);
    explicit operator float() const;

    std::uint32_t sign;
    std::uint32_t fraction;
    std::uint32_t exp;
};

// Adapted from:
// https://developer.arm.com/documentation/ddi0596/2021-03/Shared-Pseudocode/Shared-Functions?lang=en#impl-shared.FPRecipEstimate.2

// RecipEstimate()
// ===============
// Compute estimate of reciprocal of 9-bit fixed-point number.
//
// a is in range 256 .. 511 representing a number in
// the range 0.5 <= x < 1.0.
// result is in the range 256 .. 511 representing a
// number in the range 1.0 to 511/256
std::uint32_t RecipEstimate(std::uint32_t a) {
    a = a*2+1;
    std::uint32_t b = (1 << 19) / a;
    return ( b + 1) / 2;
}

// FPRecipEstimate()
// =================
float FPRecipEstimate(float operand) {
    // ([...],sign,[...]) = FPUnpack(operand, [...], [...]);
    // fraction = operand<22:0> : Zeros(29);
    // exp = UInt(operand<30:23>);
    float_parts parts{operand};

    // scaled = UInt('1':fraction<51:44>);
    std::uint32_t scaled = 0x100 | ((parts.fraction >> 15) & 0xFF) ;

    // when 32 result_exp =  253 - exp; // In range 253-254 = -1 to 253+1 = 254
    parts.exp = 253 - parts.exp;

    // // Scaled is in range 256 .. 511 representing a
    // // fixed-point number in range [0.5 .. 1.0].
    // estimate = RecipEstimate(scaled, increasedprecision);
    std::uint32_t estimate = RecipEstimate(scaled);

    // fraction = estimate<11:0> : Zeros(40);
    parts.fraction = (estimate & 0xff ) << 15;

    return float(parts);
}

int main() {
    std::cout << std::setprecision(5)
              << FPRecipEstimate(1.0f) << "\n"
              << FPRecipEstimate(2.0f) << "\n"
              << FPRecipEstimate(3.0f) << "\n"
              << FPRecipEstimate(4.0f);
}

float_parts::float_parts(float v) {
    std::uint32_t v_bits;
    std::memcpy(&v_bits, &v, sizeof(float));

    sign = (v_bits >> 31) & 0x1;
    fraction = v_bits & ((1 << 23) - 1);
    exp = (v_bits >> 23) & 0xff;
}

float_parts::operator float() const {
    std::uint32_t v_bits =
        ((sign & 0x1) << 31) |
        (fraction & ((1 << 23) - 1)) |
        ((exp & 0xff) << 23);

    float result;
    std::memcpy(&result, &v_bits, sizeof(float));
    return result;
}
