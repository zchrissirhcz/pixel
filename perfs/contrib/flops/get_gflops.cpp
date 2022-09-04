#include <time.h>
#include <stdio.h>
#include "cpu_affinity.h"
#include <vector>

#define LOOP (1e9)
//#define LOOP (0x40000000ll)
//#define OP_FLOATS (80) //一次fmla是并行4组mul+add，所以`fmla v0.4s, v0.4s, v0.4s`这条指令是8 ops。一共10条指令，因此是80 ops。

// float32x4_t vfmaq_f32 (float32x4_t __a, float32x4_t __b, float32x4_t __c);
// FMLA Vd.4S,Vn.4S,Vm.4S
static void perf_fmla_vec_vec(int loop_count, int* op_floats)
{
#if __aarch64__
    asm volatile(
        "0: \n"
        "fmla v0.4s, v0.4s, v0.4s \n"
        "fmla v1.4s, v1.4s, v1.4s \n"
        "fmla v2.4s, v2.4s, v2.4s \n"
        "fmla v3.4s, v3.4s, v3.4s \n"
        "fmla v4.4s, v4.4s, v4.4s \n"
        "fmla v5.4s, v5.4s, v5.4s \n"
        "fmla v6.4s, v6.4s, v6.4s \n"
        "fmla v7.4s, v7.4s, v7.4s \n"
        "fmla v8.4s, v8.4s, v8.4s \n"
        "fmla v9.4s, v9.4s, v9.4s \n"
        "fmla v10.4s, v10.4s, v10.4s \n"
        "fmla v11.4s, v11.4s, v11.4s \n"
        "fmla v12.4s, v12.4s, v12.4s \n"
        "fmla v13.4s, v13.4s, v13.4s \n"
        "fmla v14.4s, v14.4s, v14.4s \n"
        "fmla v15.4s, v15.4s, v15.4s \n"
        "fmla v16.4s, v16.4s, v16.4s \n"
        "fmla v17.4s, v17.4s, v17.4s \n"
        "fmla v18.4s, v18.4s, v18.4s \n"
        "fmla v19.4s, v19.4s, v19.4s \n"
        "fmla v20.4s, v20.4s, v20.4s \n"
        "fmla v21.4s, v21.4s, v21.4s \n"
        "fmla v22.4s, v22.4s, v22.4s \n"
        "fmla v23.4s, v23.4s, v23.4s \n"
        "fmla v24.4s, v24.4s, v24.4s \n"
        "fmla v25.4s, v25.4s, v25.4s \n"
        "fmla v26.4s, v26.4s, v26.4s \n"
        "fmla v27.4s, v27.4s, v27.4s \n"
        "fmla v28.4s, v28.4s, v28.4s \n"
        "fmla v29.4s, v29.4s, v29.4s \n"
        "fmla v30.4s, v30.4s, v30.4s \n"
        "fmla v31.4s, v31.4s, v31.4s \n"
        "subs %w0, %w0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
            , 
            "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );
    *op_floats = 8 * 32; // 8 ops * 32条指令
#else
    asm volatile(
        "0: \n"
        "vmla.f32 q0, q0, q0 \n"
        "vmla.f32 q1, q1, q1 \n"
        "vmla.f32 q2, q2, q2 \n"
        "vmla.f32 q3, q3, q3 \n"
        "vmla.f32 q4, q4, q4 \n"
        "vmla.f32 q5, q5, q5 \n"
        "vmla.f32 q6, q6, q6 \n"
        "vmla.f32 q7, q7, q7 \n"
        "vmla.f32 q8, q8, q8 \n"
        "vmla.f32 q9, q9, q9 \n"
        "vmla.f32 q10, q10, q10 \n"
        "vmla.f32 q11, q11, q11 \n"
        "vmla.f32 q12, q12, q12 \n"
        "vmla.f32 q13, q13, q13 \n"
        "vmla.f32 q14, q14, q14 \n"
        "vmla.f32 q15, q15, q15 \n"
        "subs %0, %0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
    );
    *op_floats = 8 * 16;
#endif
}

// float32x4_t vfmaq_n_f32 (float32x4_t a, float32x4_t b, float32_t n)
// FMLA Vd.4S,Vn.4S,Vm.S[0]
// 经尝试，armv7用11条指令得到的GFLOPS最大
static void perf_fmla_vec_scalar(int loop_count, int* op_floats)
{
#if __aarch64__
    asm volatile(
        "0: \n"
        "fmla v0.4s, v0.4s, v0.s[0] \n"
        "fmla v1.4s, v1.4s, v1.s[0] \n"
        "fmla v2.4s, v2.4s, v2.s[0] \n"
        "fmla v3.4s, v3.4s, v3.s[0] \n"
        "fmla v4.4s, v4.4s, v4.s[0] \n"
        "fmla v5.4s, v5.4s, v5.s[0] \n"
        "fmla v6.4s, v6.4s, v6.s[0] \n"
        "fmla v7.4s, v7.4s, v7.s[0] \n"
        "fmla v8.4s, v8.4s, v8.s[0] \n"
        "fmla v9.4s, v9.4s, v9.s[0] \n"
        "fmla v10.4s, v10.4s, v10.s[0] \n"
        "fmla v11.4s, v11.4s, v11.s[0] \n"
        "fmla v12.4s, v12.4s, v12.s[0] \n"
        "fmla v13.4s, v13.4s, v13.s[0] \n"
        "fmla v14.4s, v14.4s, v14.s[0] \n"
        "fmla v15.4s, v15.4s, v15.s[0] \n"
        "fmla v16.4s, v16.4s, v16.s[0] \n"
        "fmla v17.4s, v17.4s, v17.s[0] \n"
        "fmla v18.4s, v18.4s, v18.s[0] \n"
        "fmla v19.4s, v19.4s, v19.s[0] \n"
        "fmla v20.4s, v20.4s, v20.s[0] \n"
        "fmla v21.4s, v21.4s, v21.s[0] \n"
        "fmla v22.4s, v22.4s, v22.s[0] \n"
        "fmla v23.4s, v23.4s, v23.s[0] \n"
        "fmla v24.4s, v24.4s, v24.s[0] \n"
        "fmla v25.4s, v25.4s, v25.s[0] \n"
        "fmla v26.4s, v26.4s, v26.s[0] \n"
        "fmla v27.4s, v27.4s, v27.s[0] \n"
        "fmla v28.4s, v28.4s, v28.s[0] \n"
        "fmla v29.4s, v29.4s, v29.s[0] \n"
        "fmla v30.4s, v30.4s, v30.s[0] \n"
        "fmla v31.4s, v31.4s, v31.s[0] \n"
        "subs %w0, %w0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", 
          "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
          ,
          "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31"
    );
    *op_floats = 8*32;
#else
    //For vector-scalar multiplications, the 32bit scalar container must be d0 - d15 by definition.
    //因此只能用d0-d15
    asm volatile(
        "0: \n"
        "vmla.f32 q0, q0, d0[0] \n"
        "vmla.f32 q1, q1, d2[0] \n"
        "vmla.f32 q2, q2, d4[0] \n"
        "vmla.f32 q3, q3, d6[0] \n"
        "vmla.f32 q4, q4, d8[0] \n"
        "vmla.f32 q5, q5, d10[0] \n"
        "vmla.f32 q6, q6, d12[0] \n"
        "vmla.f32 q7, q7, d14[0] \n"
        "vmla.f32 q8, q8, d1[0] \n"
        "vmla.f32 q9, q9, d3[0] \n"
        "vmla.f32 q10, q10, d5[0] \n"
        "vmla.f32 q11, q11, d7[0] \n"
        "vmla.f32 q12, q12, d9[0] \n"
        "vmla.f32 q13, q13, d11[0] \n"
        "vmla.f32 q14, q14, d13[0] \n"
        "vmla.f32 q15, q15, d15[0] \n"
        "subs %0, %0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
    );
    *op_floats = 8*16;
#endif
}

static double get_time(struct timespec* start, struct timespec* end)
{
    return end->tv_sec - start->tv_sec + (end->tv_nsec - start->tv_nsec) * 1e-9;
}

static int measure_gflops() {
    struct timespec start, end;
    double time_used = 0.0;
    int op_floats = 0;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    perf_fmla_vec_vec(LOOP, &op_floats);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_used = get_time(&start, &end);
    printf("perf_fmla_vec_vec: %.2lf \n", LOOP*op_floats*1.0 * 1e-9 / time_used);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    perf_fmla_vec_scalar(LOOP, &op_floats);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_used = get_time(&start, &end);
    printf("perf_fmla_vec_scalar: %.2lf GFLOPS\n", LOOP*op_floats*1.0 * 1e-9 / time_used);

    return 0;
}




static void swapSort(std::vector<int> &arr, std::vector<int> &idx, bool reverse = true) {
  if (reverse) {
    for (int i = 0; i < arr.size() - 1; ++i) {
      int maxVal = arr[i];
      int maxIdx = i;
      for (int j = i + 1; j < arr.size(); ++j) {
        if (arr[j] > maxVal) {
          maxVal = arr[j];
          maxIdx = j;
        }
      }
      // swap val
      int tmp = arr[maxIdx];
      arr[maxIdx] = arr[i];
      arr[i] = tmp;
      // swap idx
      tmp = idx[maxIdx];
      idx[maxIdx] = idx[i];
      idx[i] = tmp;
    }
  } else {
    for (int i = 0; i < arr.size() - 1; ++i) {
      int minVal = arr[i];
      int minIdx = i;
      for (int j = i + 1; j < arr.size(); ++j) {
        if (arr[j] < minVal) {
          minVal = arr[j];
          minIdx = j;
        }
      }
      // swap val
      int tmp = arr[minIdx];
      arr[minIdx] = arr[i];
      arr[i] = tmp;
      // swap idx
      tmp = idx[minIdx];
      idx[minIdx] = idx[i];
      idx[i] = tmp;
    }
  }
}


// naive boxfilter implementation
static void Boxfilter(int radius, int height, int width) {
  float *input = new float[height * width];
  float *output = new float[height * width];
  for (int h = 0; h < height; ++h) {
    int height_sift = h * width;
    for (int w = 0; w < width; ++w) {
      int start_h = std::max(0, h - radius);
      int end_h = std::min(height - 1, h + radius);
      int start_w = std::max(0, w - radius);
      int end_w = std::min(width - 1, w + radius);
      float tmp = 0;
      for (int sh = start_h; sh <= end_h; ++sh) {
        for (int sw = start_w; sw <= end_w; ++ sw) {
          tmp += input[sh * width + sw];
        }
      }
      output[height_sift + w] = tmp;
    }
  }
  delete[] input;
  delete[] output;
}



int main() {
    int cpu_count = get_cpucount();
    fprintf(stderr, "cpu numbers %d\n", cpu_count);

    std::vector<int> cpu_max_freq_khz(cpu_count);
    std::vector<int> cpu_idx(cpu_count);
    for (int i=0; i<cpu_count; i++)
    {
        cpu_max_freq_khz[i] = get_max_freq_khz(i);
        cpu_idx[i] = i;
    }

    int the_cpu_id = 7;
    // bind big cores
    fprintf(stderr, "bind big core %d, whose max freq khz is %d:\n", the_cpu_id, cpu_max_freq_khz[the_cpu_id]);
    size_t mask = 0;
    mask |= (1<<the_cpu_id);
    int ret = set_sched_affinity(mask);

    // measure GFLOPS
    measure_gflops();

    return 0;
}