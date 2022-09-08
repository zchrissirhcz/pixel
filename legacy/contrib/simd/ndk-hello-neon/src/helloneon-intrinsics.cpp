/*
 * Copyright (C) 2016 The Android Open Source Project
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
#include "helloneon-intrinsics.h"
#include <arm_neon.h>
#include <stdio.h>

void fir_filter_neon_fast(short* output, const short* input, const short* kernel,
        int width, int kernelSize)
{
    int offset = -kernelSize/2;
    int segments = kernelSize/4;
    const short* buf = input + offset;
    for(int t=0; t<width; t++) {
        int32x4_t sum1 = vdupq_n_s32(0);
        int32x4_t sum2 = vdupq_n_s32(0);
        int32x4_t sum3 = vdupq_n_s32(0);
        int32x4_t sum4 = vdupq_n_s32(0);

        int32x4_t partialSumsNeon = vdupq_n_s32(0);

        const short* vector1 = input + offset + t;
        const short* vector2 = kernel;
        for(int i=0; i+3<segments; i+=4) {
            int16x8_t v11 = vld1q_s16(vector1);
            int16x4_t v11_low = vget_low_s16(v11);
            int16x4_t v11_high = vget_high_s16(v11);

            int16x8_t v12 = vld1q_s16(vector2);
            int16x4_t v12_low = vget_low_s16(v12);
            int16x4_t v12_high = vget_high_s16(v12);

            int16x8_t v21 = vld1q_s16(vector1+8);
            int16x4_t v21_low = vget_low_s16(v21);
            int16x4_t v21_high = vget_high_s16(v21);

            int16x8_t v22 = vld1q_s16(vector2+8);
            int16x4_t v22_low = vget_low_s16(v22);
            int16x4_t v22_high = vget_high_s16(v22);

            sum1 = vmlal_s16(sum1, v11_low, v12_low);
            sum2 = vmlal_s16(sum2, v11_high, v12_high);
            sum3 = vmlal_s16(sum3, v21_low, v22_low);
            sum4 = vmlal_s16(sum4, v21_high, v22_high);

            vector1 += 16;
            vector2 += 16;
        }

        partialSumsNeon = sum1 + sum2 + sum3 + sum4;
        int sum = 0;

        // sum up remain parts
        int remain = kernelSize % 4;
        for(int i=0; i<remain; i++) {
            int16x4_t v11 = vld1_s16(vector1);
            int16x4_t v12 = vld1_s16(vector2);
            partialSumsNeon = vmlal_s16(partialSumsNeon, v11, v12);
            vector1 += 4;
            vector2 += 4;
        }

        // store partial sums
        int partialSums[4];
        vst1q_s32(partialSums, partialSumsNeon);

        // sum up partial sums
        for(int i=0; i<4; i++) {
            sum += partialSums[i];
        }

        output[t] = (short)((sum + 0x8000) >> 16);
    }
}

void fir_filter_neon_intrinsics(short* output, const short* input,
        const short* kernel, int width, int kernelSize)
{
#if 1
   int offset = -kernelSize/2;

   for (int i=0; i<width; i++)
   {
        int sum = 0;
        int32x4_t sum_vec = vdupq_n_s32(0);
        for(int j=0; j<kernelSize/4; j++)
        {
            int16x4_t kernel_vec = vld1_s16(kernel + j*4);
            int16x4_t input_vec = vld1_s16(input + (i+offset+j*4));
            sum_vec = vmlal_s16(sum_vec, kernel_vec, input_vec);
        }

        sum += vgetq_lane_s32(sum_vec, 0);
        sum += vgetq_lane_s32(sum_vec, 1);
        sum += vgetq_lane_s32(sum_vec, 2);
        sum += vgetq_lane_s32(sum_vec, 3);

        // remain parts
        if(kernelSize & 3)
        {
            for(int j=kernelSize-(kernelSize&3); j<kernelSize; j++)
                sum += kernel[j] * input[i+offset+j];
        }

        output[i] = (short)((sum + 0x8000) >> 16);
    }
#else /* for comparison purposes only */
    int offset = -kernelSize/2;
    short* buf = input + offset;
    for (int i=0; i<width; i++) {
        int sum = 0;
        // dotProduct of kernel and buf
        for (int j=0; j<kernelSize; j++) {
            //sum += kernel[j]*input[i+offset+j];
            sum += kernel[j]*buf[j];
        }
        output[i] = (short)((sum + 0x8000) >> 16);
    }
#endif
}
