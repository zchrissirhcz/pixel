#pragma once

#include <stdio.h>

#if __linux__ || __ANDROID__
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#endif

static int set_sched_affinity(size_t thread_affinity_mask)
{
#ifdef __GLIBC__
    pid_t pid = syscall(SYS_gettid);
#else
#ifdef PI3
    pid_t pid = getpid();
#else
    pid_t pid = gettid();
#endif
#endif
    int syscallret = syscall(__NR_sched_setaffinity, pid, sizeof(thread_affinity_mask), &thread_affinity_mask);
    if (syscallret)
    {
        fprintf(stderr, "syscall error %d\n", syscallret);
        return -1;
    }
    return 0;
}

