/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helloneon-intrinsics.h"

#include "fastMalloc.h"

// return current time in milliseconds
static double now_ms(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

// this is a FIR filter implemented in C
static void fir_filter_c(short* output, const short* input, const short* kernel, int width, int kernelSize)
{
    int  offset = -kernelSize/2;
    for (int i=0; i<width; i++) {
        int sum = 0;
        for (int j=0; j<kernelSize; j++) {
            sum += kernel[j]*input[i+offset+j];
        }
        output[i] = (short)((sum + 0x8000) >> 16);
    }
}

static void fir_filter_c_fast(short* output, const short* input, const short* kernel, int width, int kernelSize)
{
    int offset = -kernelSize/2;
    const short* buf = input + offset;
    for (int i=0; i<width; i++) {
        int sum = 0;
        for (int j=0; j<kernelSize; j++) {
            sum += kernel[j]*buf[j];
        }
        output[i] = (short)((sum + 0x8000) >> 16);
        buf++;
    }
}

static void perf_fir_filter(const int fir_iterations)
{
    printf("-----------------------------\n");
    printf("perf with fir_iterations=%d\n", fir_iterations);
    printf("-----------------------------\n");

    char* str;
    char buffer[512];

    const int fir_kernel_size = 32;
    const int fir_output_size = 2560;
    const int fir_input_size = (fir_output_size + fir_kernel_size);

    short fir_input_0[fir_input_size];
    // fir_input_0相当于是fir_input做padding
    const short* fir_input = fir_input_0 + (fir_kernel_size/2);
    short fir_output[fir_output_size];
    short fir_output_expected[fir_output_size];

    static const short  fir_kernel[fir_kernel_size] = {
        0x10, 0x20, 0x40, 0x70, 0x8c, 0xa2, 0xce, 0xf0, 0xe9, 0xce, 0xa2, 0x8c, 0x70, 0x40, 0x20, 0x10,
        0x10, 0x20, 0x40, 0x70, 0x8c, 0xa2, 0xce, 0xf0, 0xe9, 0xce, 0xa2, 0x8c, 0x70, 0x40, 0x20, 0x10
    };

    // setup FIR input - whatever
    {
        for (int i=0; i<fir_input_size; i++) {
            fir_input_0[i] = (5*i) & 255;
        }
        fir_filter_c(fir_output_expected, fir_input, fir_kernel, fir_output_size, fir_kernel_size);
    }

    //---------------------------------------------
    // Benchmark small FIR filter loop - C version
    //--------------------------------------------
    double t_start, t_cost_c, t_cost_c_fast, t_cost_neon, t_cost_neon_fast;
    t_start = now_ms();
    {
        for (int i=0; i<fir_iterations; i++) {
            fir_filter_c(fir_output, fir_input, fir_kernel, fir_output_size, fir_kernel_size);
        }
    }
    t_cost_c = now_ms() - t_start;

    asprintf(&str, "FIR Filter benchmark:\nC version: %g ms\n", t_cost_c);
    strlcpy(buffer, str, sizeof(buffer));
    free(str);


    //------------------------------------------------
    // Benchmark small FIR filter loop - C fast
    //------------------------------------------------
    strlcat(buffer, "C fast version: ", sizeof(buffer));
    t_start = now_ms();
    {
        for (int i=0; i<fir_iterations; i++) {
            fir_filter_c_fast(fir_output, fir_input, fir_kernel, fir_output_size, fir_kernel_size);
        }
    }
    t_cost_c_fast = now_ms() - t_start;
    asprintf(&str, "%g ms (x%g faster)\n", t_cost_c_fast, t_cost_c / (t_cost_c_fast < 1e-6 ? 1. : t_cost_c_fast));
    strlcat(buffer, str, sizeof(buffer));
    free(str);


    //------------------------------------------------
    // Benchmark small FIR filter loop - Neon version
    //------------------------------------------------
    strlcat(buffer, "Neon version: ", sizeof(buffer));
    t_start = now_ms();
    {
        for (int i=0; i<fir_iterations; i++) {
            fir_filter_neon_intrinsics(fir_output, fir_input, fir_kernel, fir_output_size, fir_kernel_size);
        }
    }
    t_cost_neon = now_ms() - t_start;
    asprintf(&str, "%g ms (x%g faster)\n", t_cost_neon, t_cost_c / (t_cost_neon < 1e-6 ? 1. : t_cost_neon));
    strlcat(buffer, str, sizeof(buffer));
    free(str);


    //------------------------------------------------
    // Benchmark small FIR filter loop - Neon fast
    //------------------------------------------------
    strlcat(buffer, "Neon fast version: ", sizeof(buffer));
    t_start = now_ms();
    {
        for (int i=0; i<fir_iterations; i++) {
            fir_filter_neon_fast(fir_output, fir_input, fir_kernel, fir_output_size, fir_kernel_size);
        }
    }
    t_cost_neon_fast = now_ms() - t_start;
    asprintf(&str, "%g ms (x%g faster)\n", t_cost_neon_fast, t_cost_c / (t_cost_neon < 1e-6 ? 1. : t_cost_neon_fast));
    strlcat(buffer, str, sizeof(buffer));
    free(str);


    // check the result, just in case
    {
        int fails = 0;
        for (int i=0; i<fir_output_size; i++) {
            if (fir_output[i] != fir_output_expected[i]) {
                if (++fails < 16) {
                    printf("neon[%d] = %d expected %d", i, fir_output[i], fir_output_expected[i]);
                }
            }
        }
        printf("%d fails\n", fails);
    }

    printf("%s\n", buffer);
}

int main() {
    perf_fir_filter(10);
    perf_fir_filter(100);
    perf_fir_filter(1000);
    //perf_fir_filter(10000);

    return 0;
}
