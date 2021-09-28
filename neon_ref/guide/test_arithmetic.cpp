#include "gtest/gtest.h"

#include <arm_neon.h>

// add: vaddq_f32 or vaddq_f64
TEST(neon, add)
{
    float32x4_t v1 = { 1.0, 2.0, 3.0, 4.0 };
    float32x4_t v2 = { 1.0, 1.0, 1.0, 1.0 };
    float32x4_t v_sum = vaddq_f32(v1, v2);
    float expected_sum[4] = { 2.0, 3.0, 4.0, 5.0 };

    float sum[4];
    vst1q_f32(sum, v_sum);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_sum[i] == sum[i]);
    }
}

// multiply: vmulq_f32 or vmulq_f64
TEST(neon, multiply)
{
    float32x4_t v1 = { 1.0, 2.0, 3.0, 4.0 };
    float32x4_t v2 = { 1.0, 1.0, 1.0, 1.0 };
    float32x4_t v_prod = vmulq_f32(v1, v2);
    float expected_prod[4] = { 1.0, 2.0, 3.0, 4.0 };

    float prod[4];
    vst1q_f32(prod, v_prod);
    for(int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_prod[i] == prod[i]);
    }
}

// multiply and accumulate: vmlaq_f32
TEST(neon, multiply_and_accumulate)
{
    float32x4_t v1 = { 1.0, 2.0, 3.0, 4.0 };
    float32x4_t v2 = { 2.0, 2.0, 2.0, 2.0 };
    float32x4_t v3 = { 3.0, 3.0, 3.0, 3.0 };
    float32x4_t v_acc = vmlaq_f32(v3, v1, v2);  // acc = v3 + v1 * v2
    float expected_acc[4] = { 5.0, 7.0, 9.0, 11.0 };

    float acc[4];
    vst1q_f32(acc, v_acc);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_acc[i] == acc[i]);
    }
}

// multiply by a scalar: vmulq_n_f32 or vmulq_n_f64
TEST(neon, multipy_by_scalar)
{
    float32x4_t v = { 1.0, 2.0, 3.0, 4.0 };
    float32_t s = 3.0;
    float32x4_t v_prod = vmulq_n_f32(v, s);
    float expected_prod[4] = { 3.0, 6.0, 9.0, 12.0 };

    float prod[4];
    vst1q_f32(prod, v_prod);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_prod[i] == prod[i]);
    }
}

// multiply by a scalar and accumulate: vmlaq_n_f32 or vmlaq_n_f64
TEST(neon, multiply_by_scalar_and_accumulate)
{
    float32x4_t v1 = { 1.0, 2.0, 3.0, 4.0 };
    float32x4_t v2 = { 1.0, 1.0, 1.0, 1.0 };
    float32_t s = 3.0;
    float32x4_t v_acc = vmlaq_n_f32(v1, v2, s);
    float expected_acc[4] = { 4.0, 5.0, 6.0, 7.0 };

    float acc[4];
    vst1q_f32(acc, v_acc);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_acc[i] == acc[i]);
    }
}

// invert (needed for division): vrecpeq_f32 or vrecpeq_f64
TEST(neon, invert)
{
    float32x4_t v = { 1.0, 2.0, 3.0, 4.0 };
    float32x4_t v_reciprocal = vrecpeq_f32(v);
    float expected_reciprocal[4] = { 0.998046875, 0.499023438, 0.333007813, 0.249511719 };

    float reciprocal[4];
    vst1q_f32(reciprocal, v_reciprocal);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_reciprocal[i] == reciprocal[i]);
    }
}

// invert (more accurately): use a Newton-Raphson iteration to refine the estimate
// http://en.wikipedia.org/wiki/Division_algorithm#Newton.E2.80.93Raphson_division
TEST(neon, invert_accurate)
{
    float32x4_t v = { 1.0, 2.0, 3.0, 4.0 };
    float32x4_t v_reciprocal = vrecpeq_f32(v);
    float32x4_t v_inverse = vmulq_f32(vrecpsq_f32(v, v_reciprocal), v_reciprocal);
    float expected_inverse[4] = { 0.999996185, 0.499998093, 0.333333015, 0.249999046 };

    float inverse[4];
    vst1q_f32(inverse, v_inverse);
    for (int i=0; i<4; i++)
    {
        EXPECT_TRUE(expected_inverse[i] == inverse[i]);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    // Run a specific test only
    //testing::GTEST_FLAG(filter) = "MyLibrary.TestReading"; // I'm testing a new feature, run something quickly
    //testing::GTEST_FLAG(filter) = "pyrDown.PyrDownVec_16u8u";

    // Exclude a specific test
    //testing::GTEST_FLAG(filter) = "-cvtColorTwoPlane.yuv420sp_to_rgb:-cvtColorTwoPlane.rgb_to_yuv420sp"; // The writing test is broken, so skip it

    return RUN_ALL_TESTS();
}
