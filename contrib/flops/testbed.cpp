#include <time.h>
#include <stdio.h>

#define LOOP (1e9)
#define OP_FLOATS (80)

// float32x4_t vfmaq_f32 (float32x4_t __a, float32x4_t __b, float32x4_t __c);
// FMLA Vd.4S,Vn.4S,Vm.4S
static void TEST1(int loop_count)
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
        "subs %w0, %w0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9"
    );
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
        "subs %0, %0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9"
    );
#endif
}

// float32x4_t vfmaq_n_f32 (float32x4_t a, float32x4_t b, float32_t n)
// FMLA Vd.4S,Vn.4S,Vm.S[0]
// 经尝试，armv7用11条指令得到的GFLOPS最大
static void TEST2(int loop_count)
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
        "subs %w0, %w0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9"
    );
#else
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
        "vmla.f32 q8, q8, d0[0] \n"
        "vmla.f32 q9, q9, d2[0] \n"
        "vmla.f32 q10, q10, d4[0] \n"
        "subs %0, %0,    #1 \n"
        "bne 0b \n"
        : "=r"(loop_count) //%0
        : "0"(loop_count)
        : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10"
    );
#endif
}

static double get_time(struct timespec* start, struct timespec* end)
{
    return end->tv_sec - start->tv_sec + (end->tv_nsec - start->tv_nsec) * 1e-9;
}

int main() {
    struct timespec start, end;
    double time_used = 0.0;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    TEST2(LOOP);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_used = get_time(&start, &end);
    printf("perf: %.6lf \n", LOOP*OP_FLOATS*1.0 * 1e-9 / time_used);

    return 0;
}