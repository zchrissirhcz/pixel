#include "gtest/gtest.h"

#include "pixel_neon.hpp"
#include <arm_neon.h>

TEST(round, vrndn)
{
    {
        float32x2_t v1 = {0.50f, 1.6};
        float32x2_t v_out = vrndn_f32(v1);
        float expected_out[2] = {0, 2};

        pxl::float32x2_t pv1 = {0.50f, 1.6};
        pxl::float32x2_t pv_out = pxl::vrndn_f32(pv1);

        float out[2];
        vst1_f32(out, v_out);
        for (int i=0; i<2; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }
    
    {
        float32x2_t v1 = {-1.50, -1.6};
        float32x2_t v_out = vrndn_f32(v1);
        float expected_out[2] = {-2, -2};

        pxl::float32x2_t pv1 = {-1.50, -1.6};
        pxl::float32x2_t pv_out = pxl::vrndn_f32(pv1);

        float out[2];
        vst1_f32(out, v_out);
        for (int i=0; i<2; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x2_t v1 = {0.5, -0.1};
        float32x2_t v_out = vrndn_f32(v1);
        float expected_out[2] = {0, 0};

        pxl::float32x2_t pv1 = {0.5, -0.1};
        pxl::float32x2_t pv_out = pxl::vrndn_f32(pv1);

        float out[2];
        vst1_f32(out, v_out);
        for (int i=0; i<2; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            //ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x2_t v1 = {0, -0.5};
        float32x2_t v_out = vrndn_f32(v1);
        float expected_out[2] = {2, 2};

        pxl::float32x2_t pv1 = {0, -0.5};
        pxl::float32x2_t pv_out = pxl::vrndn_f32(pv1);

        float out[2];
        vst1_f32(out, v_out);
        for (int i=0; i<2; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            //ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }
}

TEST(round, vrndq)
{
    {
        float32x4_t v1 = {1.50, 1.6, 2.5, 3.6};
        float32x4_t v_out = vrndq_f32(v1);
        float expected_out[4] = {1, 1, 2, 3};

        pxl::float32x4_t pv1 = {1.50, 1.6, 2.5, 3.6};
        pxl::float32x4_t pv_out = pxl::vrndq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {-1.50, -1.6, -2.5, -3.6};
        float32x4_t v_out = vrndq_f32(v1);
        float expected_out[4] = {-1, -1, -2, -3};

        pxl::float32x4_t pv1 = {-1.50, -1.6, -2.5, -3.6};
        pxl::float32x4_t pv_out = pxl::vrndq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }
    
}


TEST(round, vrnda)
{
    {
        float32x4_t v1 = {1.0, 1.6, 2.5, 3.6};
        float32x4_t v_out = vrndaq_f32(v1);
        float expected_out[4] = {1, 2, 3, 4};

        pxl::float32x4_t pv1 = {1.0, 1.6, 2.5, 3.6};
        pxl::float32x4_t pv_out = pxl::vrndaq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {-1.0, -1.6, -2.5, -3.6};
        float32x4_t v_out = vrndaq_f32(v1);
        float expected_out[4] = {-1, -2, -3, -4};

        pxl::float32x4_t pv1 = {-1.0, -1.6, -2.5, -3.6};
        pxl::float32x4_t pv_out = pxl::vrndaq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {-1.0, -1.6, -2.5, -3.6};
        float32x4_t v_out = vrndaq_f32(v1);
        float expected_out[4] = {-1, -2, -3, -4};

        pxl::float32x4_t pv1 = {-1.0, -1.6, -2.5, -3.6};
        pxl::float32x4_t pv_out = pxl::vrndaq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {1.5, -1.5, 3147483649.0, -2147483649.0};
        float32x4_t v_out = vrndaq_f32(v1);
        float expected_out[4] = {2, -2, 3147483648, -2147483648};

        pxl::float32x4_t pv1 = {1.5, -1.5, 3147483649.0, -2147483649.0};
        pxl::float32x4_t pv_out = pxl::vrndaq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }
}

TEST(round, vrndp)
{
    {
        float32x4_t v1 = {1.0, 1.6, 2.5, 3.6};
        float32x4_t v_out = vrndpq_f32(v1);
        float expected_out[4] = {1, 2, 3, 4};

        pxl::float32x4_t pv1 = {1.0, 1.6, 2.5, 3.6};
        pxl::float32x4_t pv_out = pxl::vrndpq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "%f, ", out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {-1.0, -1.6, -2.5, -3.6};
        float32x4_t v_out = vrndpq_f32(v1);
        float expected_out[4] = {-1, -1, -2, -3};

        pxl::float32x4_t pv1 = {-1.0, -1.6, -2.5, -3.6};
        pxl::float32x4_t pv_out = pxl::vrndpq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {1.5, -1.5, 3147483649.0, -2147483649.0};
        float32x4_t v_out = vrndpq_f32(v1);
        float expected_out[4] = {2, -1, 3147483648, -2147483648};

        pxl::float32x4_t pv1 = {1.5, -1.5, 3147483649.0, -2147483649.0};
        pxl::float32x4_t pv_out = pxl::vrndpq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }
}

TEST(round, vrndm)
{
    {
        float32x4_t v1 = {1.0, 1.6, 2.5, 3.6};
        float32x4_t v_out = vrndmq_f32(v1);
        float expected_out[4] = {1, 1, 2, 3};

        pxl::float32x4_t pv1 = {1.0, 1.6, 2.5, 3.6};
        pxl::float32x4_t pv_out = pxl::vrndmq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {-1.0, -1.6, -2.5, -3.6};
        float32x4_t v_out = vrndmq_f32(v1);
        float expected_out[4] = {-1, -2, -3, -4};

        pxl::float32x4_t pv1 = {-1.0, -1.6, -2.5, -3.6};
        pxl::float32x4_t pv_out = pxl::vrndmq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
    }

    {
        float32x4_t v1 = {1.5, -1.5, 3147483649.0, -2147483649.0};
        float32x4_t v_out = vrndmq_f32(v1);
        float expected_out[4] = {1, -2, 3147483648, -2147483648};

        pxl::float32x4_t pv1 = {1.5, -1.5, 3147483649.0, -2147483649.0};
        pxl::float32x4_t pv_out = pxl::vrndmq_f32(pv1);

        float out[4];
        vst1q_f32(out, v_out);
        for (int i=0; i<4; i++)
        {
            ASSERT_EQ(pv_out[i], out[i]);
            ASSERT_EQ(expected_out[i], out[i]);
            //fprintf(stderr, "[%f, %f], ", out[i], pv_out[i]);
        }
        //fprintf(stderr, "\n");
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
