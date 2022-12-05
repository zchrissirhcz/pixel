/*
 * Header-only CPU helper library
 * Created by Zhuo Zhang (imzhuo@foxmail.com)
 * Last update: 2022-12-04 20:26:00
 *
 * Usage:
 *
 * #include "cpu_helper.hpp"
 *
 * ...
 *
 * {
 *     plain::set_cpu_powersave(1);
 *     plain::print_cpu_info()
 * }
 */
#pragma once

//--------------------------------------------------------------------------------
// APIs
//--------------------------------------------------------------------------------

#if defined __ANDROID__ || defined __linux__
#include <sched.h> // cpu_set_t
#endif

//#define CPU_HELPER_USE_CPUINFO 1   // requires cpuinfo library

namespace plain {

static int get_cpu_count();

static int get_max_freq_khz(int cpuid);

// bind all threads on little clusters if powersave enabled
// affects HMP arch cpu like ARM big.LITTLE
// only implemented on android at the moment
// switching powersave is expensive and not thread-safe
// 0 = all cores enabled(default)
// 1 = only little clusters enabled
// 2 = only big clusters enabled
// return 0 if success for setter function
static int get_cpu_powersave();
static int set_cpu_powersave(int powersave);

class CpuSet
{
public:
    CpuSet();
    void enable(int cpuid);
    void disable(int cpuid);
    void disable_all();
    bool is_enabled(int cpu) const;
    int num_enabled() const;

public:
#if defined(__ANDROID__) || defined(__linux__)
    cpu_set_t cpu_set;
#endif
#if __APPLE__
    unsigned int policy;
#endif
};

// Usage:
// #define CPU_HELPER_USE_CPUINFO 1
// #include "cpuinfo.h"
// #include "plain/cpu_helper.hpp"
// void foo() {
// #if CPU_HELPER_USE_CPUINFO    
//    cpuinfo_initialize();
// #endif
//
//    plain::print_cpu_info(cpu_id);
//
// #if CPU_HELPER_USE_CPUINFO    
//     cpuinfo_deinitialize();
// #endif
// }
void print_cpu_info(int cpu_id);

} // namespace plain


//--------------------------------------------------------------------------------
// Implementations
//--------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

#include <limits.h>
#include <vector>

#if defined __ANDROID__ || defined __linux__
#if defined __ANDROID__
#if __ANDROID_API__ >= 18
#include <sys/auxv.h> // getauxval()
#endif
#include <dlfcn.h>
#endif
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

#if __APPLE__
#include <mach/mach.h>
#include <mach/machine.h>
#include <mach/thread_act.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define __IOS__ 1
#endif
// define missing cpu model for old sdk
#ifndef CPUFAMILY_ARM_HURRICANE
#define CPUFAMILY_ARM_HURRICANE 0x67ceee93
#endif
// A11
#ifndef CPUFAMILY_ARM_MONSOON_MISTRAL
#define CPUFAMILY_ARM_MONSOON_MISTRAL 0xe81e7ef6
#endif
// A12
#ifndef CPUFAMILY_ARM_VORTEX_TEMPEST
#define CPUFAMILY_ARM_VORTEX_TEMPEST 0x07d34b9f
#endif
// A13
#ifndef CPUFAMILY_ARM_LIGHTNING_THUNDER
#define CPUFAMILY_ARM_LIGHTNING_THUNDER 0x462504d2
#endif
// A14
#ifndef CPUFAMILY_ARM_FIRESTORM_ICESTORM
#define CPUFAMILY_ARM_FIRESTORM_ICESTORM 0x1b588bb3
#endif
// A15
#ifndef CPUFAMILY_ARM_AVALANCHE_BLIZZARD
#define CPUFAMILY_ARM_AVALANCHE_BLIZZARD 0xda33d83d
#endif
// M1
#ifndef CPUFAMILY_AARCH64_FIRESTORM_ICESTORM
#define CPUFAMILY_AARCH64_FIRESTORM_ICESTORM 0x1b588bb3
#endif
#endif // __APPLE__

#if _MSC_VER
// Faster Builds with Smaller Header Files
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa383745%28v=vs.85%29.aspx

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX

#include <windows.h>

#undef small
#undef min
#undef max
#undef abs
#undef NOATOM
#undef NOGDI
#undef NOGDICAPMASKS
#undef NOMETAFILE
#undef NOMSG
#undef NOOPENFILE
#undef NORASTEROPS
#undef NOSCROLL
#undef NOSOUND
#undef NOSYSMETRICS
#undef NOTEXTMETRIC
#undef NOWH
#undef NOCOMM
#undef NOKANJI
#undef NOCRYPT
#undef NOMCX
#endif // _MSC_VER

int plain::get_cpu_count()
{
    static int count = 0;
    if (count != 0)
    {
        return count;
    }

#if defined(__ANDROID__) || defined(__linux__)
    // http://man7.org/linux/man-pages/man3/sysconf.3.html
    // The number of processors currently online (available)
    count = (int) sysconf(_SC_NPROCESSORS_ONLN);
#elif __APPLE__
    size_t len = sizeof(count);
    sysctlbyname("hw.ncpu", &count, &len, NULL, 0);
#elif _OPENMP
    count = omp_get_max_threads();
#elif _MSC_VER
    // https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsysteminfo
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (int)sysinfo.dwNumberOfProcessors;
#else
    // get cpu count from /proc/cpuinfo
    FILE* fp = fopen("/proc/cpuinfo", "rb");
    if (fp)
    {
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
    }
    else
    {
        count = 1;
    }
#endif

    return count;
}

int plain::get_max_freq_khz(int cpuid)
{
    int max_freq_khz = -1;
#if defined(__ANDROID__) || defined(__linux__)
    // first try, for all possible cpu
    char path[256];
    sprintf(path, "/sys/devices/system/cpu/cpufreq/stats/cpu%d/time_in_state", cpuid);

    FILE* fp = fopen(path, "rb");
    if (fp)
    {
        while (!feof(fp))
        {
            int freq_khz = 0;
            int nscan = fscanf(fp, "%d %*d", &freq_khz);
            if (nscan != 1)
            {
                break;
            }

            if (freq_khz > max_freq_khz)
            {
                max_freq_khz = freq_khz;
            }
        }
        fclose(fp);
    }
    else
    {
        // second try, for online cpu
        sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/stats/time_in_state", cpuid);
        fp = fopen(path, "rb");

        if (fp)
        {
            while (!feof(fp))
            {
                int freq_khz = 0;
                int nscan = fscanf(fp, "%d %*d", & freq_khz);
                if (nscan != 1)
                {
                    break;
                }

                if (freq_khz > max_freq_khz)
                {
                    max_freq_khz = freq_khz;
                }
            }
            fclose(fp);

            if (max_freq_khz != 0)
            {
                return max_freq_khz;
            }

            fp = NULL;
        }
        
        if (!fp)
        {
            // third try, for online cpu
            sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_max_freq", cpuid);
            fp = fopen(path, "rb");

            if (!fp)
            {
                return -1;
            }
            int nscan = fscanf(fp, "%d", &max_freq_khz);
            if (nscan != 1)
            {
                fprintf(stderr, "fscanf cpuinfo_max_freq error %d", nscan);
            }
            fclose(fp);

            return max_freq_khz;
        }
    }
#endif
    return max_freq_khz;
}

namespace plain {

CpuSet::CpuSet()
{
#if defined(__ANDROID__) || defined(__linux__)
    disable_all();
#elif __APPLE__
    disable_all();
#endif
}

void CpuSet::enable(int cpuid)
{
#if defined(__ANDROID__) || defined(__linux__)
    CPU_SET(cpuid, &cpu_set);
#elif __APPLE__
    policy |= (1 << cpuid);
#endif
}

void CpuSet::disable(int cpuid)
{
#if defined(__ANDROID__) || defined(__linux__)
    CPU_CLR(cpuid, &cpu_set);
#elif __APPLE__
    policy &= ~(1 << cpuid);
#endif
}

void CpuSet::disable_all()
{
#if defined(__ANDROID__) || defined(__linux__)
    CPU_ZERO(&cpu_set);
#elif __APPLE__
    policy = 0;
#endif
}

bool CpuSet::is_enabled(int cpuid) const
{
#if defined(__ANDROID__) || defined(__linux__)
    return CPU_ISSET(cpuid, &cpu_set);
#elif _MSC_VER
    return true;
#elif __APPLE__
    return policy & (1 << cpuid);
#endif
}

int CpuSet::num_enabled() const
{
#if defined(__ANDROID__) || defined(__linux__)
    int cpu_count = get_cpu_count();
    if (cpu_count < CPU_SETSIZE)
    {
        return CPU_COUNT(&cpu_set);
    }
    else
    {
        int num_enabled = 0;
        for (int i = 0; i < (int)sizeof(cpu_set_t) * 8; i++)
        {
            if (is_enabled(i))
            {
                num_enabled++;
            }
        }
        return num_enabled;
    }
#elif _MSC_VER
    return get_cpu_count();
#elif __APPLE__
    int num_enabled = 0;
    for (int i = 0; i < (int)sizeof(policy) * 8; i++)
    {
        if (is_enabled(i))
        {
            num_enabled++;
        }
    }
    return num_enabled;
#endif
}

static int g_powersave = 0;

static CpuSet g_thread_affinity_mask_all;
static CpuSet g_thread_affinity_mask_little;
static CpuSet g_thread_affinity_mask_big;


#if __APPLE__
static unsigned int get_hw_cpufamily()
{
    unsigned int value = 0;
    size_t len = sizeof(value);
    sysctlbyname("hw.cpufamily", &value, &len, NULL, 0);
    return value;
}

static cpu_type_t get_hw_cputype()
{
    cpu_type_t value = 0;
    size_t len = sizeof(value);
    sysctlbyname("hw.cputype", &value, &len, NULL, 0);
    return value;
}

static cpu_subtype_t get_hw_cpusubtype()
{
    cpu_subtype_t value = 0;
    size_t len = sizeof(value);
    sysctlbyname("hw.cpusubtype", &value, &len, NULL, 0);
    return value;
}

static unsigned int g_hw_cpufamily = get_hw_cpufamily();
static cpu_type_t g_hw_cputype = get_hw_cputype();
static cpu_subtype_t g_hw_cpusubtype = get_hw_cpusubtype();
#endif // __APPLE__

static int setup_thread_affinity_masks()
{
    g_thread_affinity_mask_all.disable_all();

#if defined __ANDROID__ || defined __linux__
    int max_freq_khz_min = INT_MAX;
    int max_freq_khz_max = 0;
    const int cpu_count = get_cpu_count();
    std::vector<int> cpu_max_freq_khz(cpu_count);
    for (int i = 0; i < cpu_count; i++)
    {
        int max_freq_khz = get_max_freq_khz(i);

        //         NCNN_LOGE("%d max freq = %d khz", i, max_freq_khz);

        cpu_max_freq_khz[i] = max_freq_khz;

        if (max_freq_khz > max_freq_khz_max)
            max_freq_khz_max = max_freq_khz;
        if (max_freq_khz < max_freq_khz_min)
            max_freq_khz_min = max_freq_khz;
    }

    int max_freq_khz_medium = (max_freq_khz_min + max_freq_khz_max) / 2;
    if (max_freq_khz_medium == max_freq_khz_max)
    {
        g_thread_affinity_mask_little.disable_all();
        g_thread_affinity_mask_big = g_thread_affinity_mask_all;
        return 0;
    }

    for (int i = 0; i < cpu_count; i++)
    {
        if (cpu_max_freq_khz[i] < max_freq_khz_medium)
            g_thread_affinity_mask_little.enable(i);
        else
            g_thread_affinity_mask_big.enable(i);
    }
#elif __APPLE__
    // affinity info from cpu model
    if (g_hw_cpufamily == CPUFAMILY_ARM_MONSOON_MISTRAL)
    {
        // 2 + 4
        g_thread_affinity_mask_big.enable(0);
        g_thread_affinity_mask_big.enable(1);
        g_thread_affinity_mask_little.enable(2);
        g_thread_affinity_mask_little.enable(3);
        g_thread_affinity_mask_little.enable(4);
        g_thread_affinity_mask_little.enable(5);
    }
    else if (g_hw_cpufamily == CPUFAMILY_ARM_VORTEX_TEMPEST || g_hw_cpufamily == CPUFAMILY_ARM_LIGHTNING_THUNDER || g_hw_cpufamily == CPUFAMILY_ARM_FIRESTORM_ICESTORM || g_hw_cpufamily == CPUFAMILY_ARM_AVALANCHE_BLIZZARD)
    {
        // 2 + 4 or 4 + 4
        if (get_cpu_count() == 6)
        {
            g_thread_affinity_mask_big.enable(0);
            g_thread_affinity_mask_big.enable(1);
            g_thread_affinity_mask_little.enable(2);
            g_thread_affinity_mask_little.enable(3);
            g_thread_affinity_mask_little.enable(4);
            g_thread_affinity_mask_little.enable(5);
        }
        else
        {
            g_thread_affinity_mask_big.enable(0);
            g_thread_affinity_mask_big.enable(1);
            g_thread_affinity_mask_big.enable(2);
            g_thread_affinity_mask_big.enable(3);
            g_thread_affinity_mask_little.enable(4);
            g_thread_affinity_mask_little.enable(5);
            g_thread_affinity_mask_little.enable(6);
            g_thread_affinity_mask_little.enable(7);
        }
    }
    else
    {
        // smp models
        g_thread_affinity_mask_little.disable_all();
        g_thread_affinity_mask_big = g_thread_affinity_mask_all;
    }
#else
    // TODO implement me for other platforms
    g_thread_affinity_mask_little.disable_all();
    g_thread_affinity_mask_big = g_thread_affinity_mask_all;
#endif

    return 0;
}

const CpuSet& get_cpu_thread_affinity_mask(int powersave)
{
    setup_thread_affinity_masks();

    if (powersave == 0)
        return g_thread_affinity_mask_all;

    if (powersave == 1)
        return g_thread_affinity_mask_little;

    if (powersave == 2)
        return g_thread_affinity_mask_big;

    fprintf(stderr, "powersave %d not supported", powersave);

    // fallback to all cores anyway
    return g_thread_affinity_mask_all;
}

void set_omp_num_threads(int num_threads)
{
#ifdef _OPENMP
    omp_set_num_threads(num_threads);
#else
    (void)num_threads;
#endif
}

static int set_sched_affinity(const CpuSet& thread_affinity_mask)
{
#if defined(__ANDROID__) || defined(__linux__)
    // set affinity for thread
#if defined(__BIONIC__)
    pid_t pid = gettid();
#else
    pid_t pid = syscall(SYS_gettid);
#endif

    int syscallret = syscall(__NR_sched_setaffinity, pid, sizeof(cpu_set_t), &thread_affinity_mask.cpu_set);
    if (syscallret)
    {
        fprintf(stderr, "syscall error %d", syscallret);
        return -1;
    }
#endif // defined(__ANDROID__) || defined(__linux__)
    return 0;
}

int set_cpu_thread_affinity(const CpuSet& thread_affinity_mask)
{
#if defined __ANDROID__ || defined __linux__
    int num_threads = thread_affinity_mask.num_enabled();

#ifdef _OPENMP
    // set affinity for each thread
    set_omp_num_threads(num_threads);
    std::vector<int> ssarets(num_threads, 0);
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; i++)
    {
        ssarets[i] = set_sched_affinity(thread_affinity_mask);
    }
    for (int i = 0; i < num_threads; i++)
    {
        if (ssarets[i] != 0)
            return -1;
    }
#else
    int ssaret = set_sched_affinity(thread_affinity_mask);
    if (ssaret != 0)
        return -1;
#endif

    return 0;
#elif __APPLE__

#ifdef _OPENMP
    int num_threads = thread_affinity_mask.num_enabled();

    // set affinity for each thread
    set_omp_num_threads(num_threads);
    std::vector<int> ssarets(num_threads, 0);
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; i++)
    {
        // assign one core for each thread
        int core = -1 - i;
        for (int j = 0; j < (int)sizeof(thread_affinity_mask.policy) * 8; j++)
        {
            if (thread_affinity_mask.is_enabled(j))
            {
                if (core == -1)
                {
                    core = j;
                    break;
                }
                else
                {
                    core++;
                }
            }
        }
        CpuSet this_thread_affinity_mask;
        if (core != -1 - i)
        {
            this_thread_affinity_mask.enable(core);
        }

        ssarets[i] = set_sched_affinity(this_thread_affinity_mask);
    }
    for (int i = 0; i < num_threads; i++)
    {
        if (ssarets[i] != 0)
            return -1;
    }
#else
    int ssaret = set_sched_affinity(thread_affinity_mask);
    if (ssaret != 0)
        return -1;
#endif

    return 0;
#else
    // TODO
    (void)thread_affinity_mask;
    return -1;
#endif
}

#if CPU_HELPER_USE_CPUINFO
#include "cpuinfo.h"

const char* vendor_to_string(enum cpuinfo_vendor vendor) {
    switch (vendor) {
        case cpuinfo_vendor_unknown:
            return "unknown";
        case cpuinfo_vendor_intel:
            return "Intel";
        case cpuinfo_vendor_amd:
            return "AMD";
        case cpuinfo_vendor_huawei:
            return "Huawei";
        case cpuinfo_vendor_hygon:
            return "Hygon";
        case cpuinfo_vendor_arm:
            return "ARM";
        case cpuinfo_vendor_qualcomm:
            return "Qualcomm";
        case cpuinfo_vendor_apple:
            return "Apple";
        case cpuinfo_vendor_samsung:
            return "Samsung";
        case cpuinfo_vendor_nvidia:
            return "Nvidia";
        case cpuinfo_vendor_mips:
            return "MIPS";
        case cpuinfo_vendor_ibm:
            return "IBM";
        case cpuinfo_vendor_ingenic:
            return "Ingenic";
        case cpuinfo_vendor_via:
            return "VIA";
        case cpuinfo_vendor_cavium:
            return "Cavium";
        case cpuinfo_vendor_broadcom:
            return "Broadcom";
        case cpuinfo_vendor_apm:
            return "Applied Micro";
        default:
            return NULL;
    }
}

const char* uarch_to_string(enum cpuinfo_uarch uarch) {
    switch (uarch) {
        case cpuinfo_uarch_unknown:
            return "unknown";
        case cpuinfo_uarch_p5:
            return "P5";
        case cpuinfo_uarch_quark:
            return "Quark";
        case cpuinfo_uarch_p6:
            return "P6";
        case cpuinfo_uarch_dothan:
            return "Dothan";
        case cpuinfo_uarch_yonah:
            return "Yonah";
        case cpuinfo_uarch_conroe:
            return "Conroe";
        case cpuinfo_uarch_penryn:
            return "Penryn";
        case cpuinfo_uarch_nehalem:
            return "Nehalem";
        case cpuinfo_uarch_sandy_bridge:
            return "Sandy Bridge";
        case cpuinfo_uarch_ivy_bridge:
            return "Ivy Bridge";
        case cpuinfo_uarch_haswell:
            return "Haswell";
        case cpuinfo_uarch_broadwell:
            return "Broadwell";
        case cpuinfo_uarch_sky_lake:
            return "Sky Lake";
        case cpuinfo_uarch_palm_cove:
            return "Palm Cove";
        case cpuinfo_uarch_sunny_cove:
            return "Sunny Cove";
        case cpuinfo_uarch_willamette:
            return "Willamette";
        case cpuinfo_uarch_prescott:
            return "Prescott";
        case cpuinfo_uarch_bonnell:
            return "Bonnell";
        case cpuinfo_uarch_saltwell:
            return "Saltwell";
        case cpuinfo_uarch_silvermont:
            return "Silvermont";
        case cpuinfo_uarch_airmont:
            return "Airmont";
        case cpuinfo_uarch_goldmont:
            return "Goldmont";
        case cpuinfo_uarch_goldmont_plus:
            return "Goldmont Plus";
        case cpuinfo_uarch_knights_ferry:
            return "Knights Ferry";
        case cpuinfo_uarch_knights_corner:
            return "Knights Corner";
        case cpuinfo_uarch_knights_landing:
            return "Knights Landing";
        case cpuinfo_uarch_knights_hill:
            return "Knights Hill";
        case cpuinfo_uarch_knights_mill:
            return "Knights Mill";
        case cpuinfo_uarch_k5:
            return "K5";
        case cpuinfo_uarch_k6:
            return "K6";
        case cpuinfo_uarch_k7:
            return "K7";
        case cpuinfo_uarch_k8:
            return "K8";
        case cpuinfo_uarch_k10:
            return "K10";
        case cpuinfo_uarch_bulldozer:
            return "Bulldozer";
        case cpuinfo_uarch_piledriver:
            return "Piledriver";
        case cpuinfo_uarch_steamroller:
            return "Steamroller";
        case cpuinfo_uarch_excavator:
            return "Excavator";
        case cpuinfo_uarch_zen:
            return "Zen";
        case cpuinfo_uarch_zen2:
            return "Zen 2";
        case cpuinfo_uarch_zen3:
            return "Zen 3";
        case cpuinfo_uarch_geode:
            return "Geode";
        case cpuinfo_uarch_bobcat:
            return "Bobcat";
        case cpuinfo_uarch_jaguar:
            return "Jaguar";
        case cpuinfo_uarch_puma:
            return "Puma";
        case cpuinfo_uarch_xscale:
            return "XScale";
        case cpuinfo_uarch_arm7:
            return "ARM7";
        case cpuinfo_uarch_arm9:
            return "ARM9";
        case cpuinfo_uarch_arm11:
            return "ARM11";
        case cpuinfo_uarch_cortex_a5:
            return "Cortex-A5";
        case cpuinfo_uarch_cortex_a7:
            return "Cortex-A7";
        case cpuinfo_uarch_cortex_a8:
            return "Cortex-A8";
        case cpuinfo_uarch_cortex_a9:
            return "Cortex-A9";
        case cpuinfo_uarch_cortex_a12:
            return "Cortex-A12";
        case cpuinfo_uarch_cortex_a15:
            return "Cortex-A15";
        case cpuinfo_uarch_cortex_a17:
            return "Cortex-A17";
        case cpuinfo_uarch_cortex_a32:
            return "Cortex-A32";
        case cpuinfo_uarch_cortex_a35:
            return "Cortex-A35";
        case cpuinfo_uarch_cortex_a53:
            return "Cortex-A53";
        case cpuinfo_uarch_cortex_a55r0:
            return "Cortex-A55r0";
        case cpuinfo_uarch_cortex_a55:
            return "Cortex-A55";
        case cpuinfo_uarch_cortex_a57:
            return "Cortex-A57";
        case cpuinfo_uarch_cortex_a65:
            return "Cortex-A65";
        case cpuinfo_uarch_cortex_a72:
            return "Cortex-A72";
        case cpuinfo_uarch_cortex_a73:
            return "Cortex-A73";
        case cpuinfo_uarch_cortex_a75:
            return "Cortex-A75";
        case cpuinfo_uarch_cortex_a76:
            return "Cortex-A76";
        case cpuinfo_uarch_cortex_a77:
            return "Cortex-A77";
        case cpuinfo_uarch_cortex_a78:
            return "Cortex-A78";
        case cpuinfo_uarch_neoverse_n1:
            return "Neoverse-N1";
        case cpuinfo_uarch_neoverse_v1:
            return "Neoverse-V1";
        case cpuinfo_uarch_neoverse_n2:
            return "Neoverse-N2";
        case cpuinfo_uarch_cortex_x1:
            return "Cortex-X1";
        case cpuinfo_uarch_scorpion:
            return "Scorpion";
        case cpuinfo_uarch_krait:
            return "Krait";
        case cpuinfo_uarch_kryo:
            return "Kryo";
        case cpuinfo_uarch_falkor:
            return "Falkor";
        case cpuinfo_uarch_saphira:
            return "Saphira";
        case cpuinfo_uarch_denver:
            return "Denver";
        case cpuinfo_uarch_denver2:
            return "Denver 2";
        case cpuinfo_uarch_carmel:
            return "Carmel";
        case cpuinfo_uarch_exynos_m1:
            return "Exynos M1";
        case cpuinfo_uarch_exynos_m2:
            return "Exynos M2";
        case cpuinfo_uarch_exynos_m3:
            return "Exynos M3";
        case cpuinfo_uarch_exynos_m4:
            return "Exynos M4";
        case cpuinfo_uarch_exynos_m5:
            return "Exynos M5";
        case cpuinfo_uarch_swift:
            return "Swift";
        case cpuinfo_uarch_cyclone:
            return "Cyclone";
        case cpuinfo_uarch_typhoon:
            return "Typhoon";
        case cpuinfo_uarch_twister:
            return "Twister";
        case cpuinfo_uarch_hurricane:
            return "Hurricane";
        case cpuinfo_uarch_monsoon:
            return "Monsoon";
        case cpuinfo_uarch_mistral:
            return "Mistral";
        case cpuinfo_uarch_vortex:
            return "Vortex";
        case cpuinfo_uarch_tempest:
            return "Tempest";
        case cpuinfo_uarch_lightning:
            return "Lightning";
        case cpuinfo_uarch_thunder:
            return "Thunder";
        case cpuinfo_uarch_firestorm:
            return "Firestorm";
        case cpuinfo_uarch_icestorm:
            return "Icestorm";
        case cpuinfo_uarch_thunderx:
            return "ThunderX";
        case cpuinfo_uarch_thunderx2:
            return "ThunderX2";
        case cpuinfo_uarch_pj4:
            return "PJ4";
        case cpuinfo_uarch_brahma_b15:
            return "Brahma B15";
        case cpuinfo_uarch_brahma_b53:
            return "Brahma B53";
        case cpuinfo_uarch_xgene:
            return "X-Gene";
        case cpuinfo_uarch_dhyana:
            return "Dhyana";
        case cpuinfo_uarch_taishan_v110:
            return "TaiShan v110";
        default:
            return NULL;
    }
}
#endif // CPU_HELPER_USE_CPUINFO

} // namespace plain

int plain::get_cpu_powersave()
{
    return g_powersave;
}

int plain::set_cpu_powersave(int powersave)
{
    if (powersave < 0 || powersave > 2)
    {
        fprintf(stderr, "powersave %d not supported", powersave);
        return -1;
    }

    const CpuSet& thread_affinity_mask = get_cpu_thread_affinity_mask(powersave);

    int ret = set_cpu_thread_affinity(thread_affinity_mask);
    if (ret != 0)
        return ret;

    g_powersave = powersave;

    return 0;
}

void plain::print_cpu_info(int cpu_id)
{
    const int cpu_count = plain::get_cpu_count();
    fprintf(stderr, "there are %d cores, currently use core %d", cpu_count, cpu_id);
    
#if CPU_HELPER_USE_CPUINFO
    const cpuinfo_core* core = cpuinfo_get_current_core();
    const char* vendor_string = vendor_to_string(core->vendor);
    const char* uarch_string = uarch_to_string(core->uarch);

    fprintf(stderr, ", Core=%s, uArch=%s, processor System ID=%d\n",
        vendor_string, uarch_string, core->processor_start
    );
#endif // CPU_HELPER_USE_CPUINFO
    fprintf(stderr, "\n");
}
