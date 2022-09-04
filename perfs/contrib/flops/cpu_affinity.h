#pragma once

#include <stdint.h>
#include <stddef.h>

int set_sched_affinity(size_t thread_affinity_mask);
int get_cpucount();
int get_max_freq_khz(int cpuid);
