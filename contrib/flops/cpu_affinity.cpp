#include <limits.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>

#ifdef __ANDROID__
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#endif

int set_sched_affinity(size_t thread_affinity_mask)
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
// refer to: https://github.com/Tencent/ncnn/blob/ee41ef4a378ef662d24f137d97f7f6a57a5b0eba/src/cpu.cpp
int get_cpucount()
{
    int count = 0;
#ifdef __ANDROID__
    // get cpu count from /proc/cpuinfo
    FILE* fp = fopen("/proc/cpuinfo", "rb");
    if (!fp)
        return 1;

    char line[1024];
    while (!feof(fp))
    {
        char* s = fgets(line, 1024, fp);
        if (!s)
            break;

        if (memcmp(line, "processor", 9) == 0)
        {
            count++;
        }
    }

    fclose(fp);
#endif

    if (count < 1)
        count = 1;

    if (count > (int)sizeof(size_t) * 8)
    {
        fprintf(stderr, "more than %d cpu detected, thread affinity may not work properly :(\n", (int)sizeof(size_t) * 8);
    }

    return count;
}
// refer to: https://github.com/Tencent/ncnn/blob/ee41ef4a378ef662d24f137d97f7f6a57a5b0eba/src/cpu.cpp
#ifdef __ANDROID__
int get_max_freq_khz(int cpuid)
{
    // first try, for all possible cpu
    char path[256];
    sprintf(path, "/sys/devices/system/cpu/cpufreq/stats/cpu%d/time_in_state", cpuid);
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        // second try, for online cpu
        sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/stats/time_in_state", cpuid);
        fp = fopen(path, "rb");
        if (fp)
        {
            int max_freq_khz = 0;
            while (!feof(fp))
            {
                int freq_khz = 0;
                int nscan = fscanf(fp, "%d %*d", &freq_khz);
                if (nscan != 1)
                    break;

                if (freq_khz > max_freq_khz)
                    max_freq_khz = freq_khz;
            }
            fclose(fp);
            if (max_freq_khz != 0)
                return max_freq_khz;
            fp = NULL;
        }

        if (!fp)
        {
            // third try, for online cpu
            sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", cpuid);
            fp = fopen(path, "rb");

            if (!fp)
                return -1;

            int max_freq_khz = -1;
            fscanf(fp, "%d", &max_freq_khz);

            fclose(fp);

            return max_freq_khz;
        }
    }

    int max_freq_khz = 0;
    while (!feof(fp))
    {
        int freq_khz = 0;
        int nscan = fscanf(fp, "%d %*d", &freq_khz);
        if (nscan != 1)
            break;

        if (freq_khz > max_freq_khz)
            max_freq_khz = freq_khz;
    }
    fclose(fp);
    return max_freq_khz;
}
#endif // __ANDROID__

/*
std::mutex g_display_mutex;
void threadFun() {
    size_t cpu;
    auto syscallret = syscall(__NR_getcpu, &cpu, NULL, NULL);
    std::stringstream ss;
    ss << std::this_thread::get_id();

    g_display_mutex.lock();
    printf("thread %s, running on cpu: %d\n", ss.str().c_str(), cpu);
    g_display_mutex.unlock();

    // std::this_thread::sleep_for (std::chrono::microseconds(100));

    Boxfilter(7, 500, 500);

    // syscallret = syscall(__NR_getcpu, &cpu, NULL, NULL);
    // g_display_mutex.lock();
    // printf("thread %s, running on cpu: %d\n", ss.str().c_str(), cpu);
    // g_display_mutex.unlock();
}

int main(int argc, char *argv[]) {

    int cpu_count = get_cpucount();
    printf("cpu numbers %d\n", cpu_count);

    std::vector<int> cpu_max_freq_khz(cpu_count);
    std::vector<int> cpu_idx(cpu_count);
    for (int i=0; i<cpu_count; i++)
    {
        cpu_max_freq_khz[i] = get_max_freq_khz(i);
        cpu_idx[i] = i;
    }
    
    printf("before sort\n");
    for (int i = 0; i < cpu_count; ++i) {
        printf("cpu_%d:%d, ", cpu_idx[i], cpu_max_freq_khz[i]);
    }
    printf("\n");

    swapSort(cpu_max_freq_khz, cpu_idx);

    printf("after sort\n");
    for (int i = 0; i < cpu_count; ++i) {
      printf("cpu_%d:%d, ", cpu_idx[i], cpu_max_freq_khz[i]);
    }
    printf("\n\n");

    // distinguish big & little cores with ncnn strategy 
    int max_freq_khz_max = 0;
    int max_freq_cpu_idx = 0;
    for (int i = 0; i < cpu_count; i++) {
        if (cpu_max_freq_khz[i] > max_freq_khz_max) {
            max_freq_khz_max = cpu_max_freq_khz[i];
            max_freq_cpu_idx = i;
        }
    }
    
    // warm up
    for (int i = 0; i < 5; ++i) {
        Boxfilter(7, 500, 500);
    }

    // bind big cores
    printf("bind big cores ex:\n");
    size_t mask = 0;
    for (int i = 0; i < cpu_count; ++i) {
      if (cpu_max_freq_khz[i] == max_freq_khz_max) {
        mask |= (1 << cpu_idx[i]);
        printf("bind cpu: %d, ", cpu_idx[i]);
      }
    }
    printf("\n");
    int ret = set_sched_affinity(mask);

    // measure GFLOPS



    return 0;
}
*/