#include <time.h>
#include <stdio.h>

#include <iostream>

static double get_time(struct timespec* start, struct timespec* end)
{
    return end->tv_sec - start->tv_sec + (end->tv_nsec - start->tv_nsec) * 1e-9;
}

static void benchmark_throughtput(double freq)
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

#if __aarch64__
    asm volatile(
        "mov x0, #0x40000000 \n"
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
        "subs x0, x0, #1 \n"
        "bne 0b \n"
        :
        :
        : "x0", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15"
    );
#endif // __aarch64__

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_used, perf, gops;
    time_used = get_time(&start, &end);
    perf = 0x40000000ll * 16ll / time_used * 1e-6 / freq;
    gops = perf * freq * 8 / 1000;
#if __aarch64__
    std::cout << "FP32 fmla: " << perf << " IPC, " << gops << " GOPS" << std::endl;
#else
    std::cout << "16 vmla: " << perf << " GOPS" << std::endl;
#endif // __aarch64__

}

int main() {

    double x1_freq = 2.84;
    benchmark_throughtput(x1_freq);

    return 0;
}