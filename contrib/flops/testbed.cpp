#include <time.h>
#include <stdio.h>

#define LOOP (1e9)
#define OP_FLOATS (80)

static void TEST(int loop_count)
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


static double get_time(struct timespec* start, struct timespec* end)
{
    return end->tv_sec - start->tv_sec + (end->tv_nsec - start->tv_nsec) * 1e-9;
}

int main() {
    struct timespec start, end;
    double time_used = 0.0;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    TEST(LOOP);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_used = get_time(&start, &end);
    printf("perf: %.6lf \n", LOOP*OP_FLOATS*1.0 * 1e-9 / time_used);

    return 0;
}