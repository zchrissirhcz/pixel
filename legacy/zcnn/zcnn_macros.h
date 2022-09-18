#pragma once

#include <stdio.h>

static void determine_macros()
{
#if ANDROID
    printf("defined ANDROID\n");
#else
    printf("not defined ANDROID\n");
#endif

#if __linux__ // if ANDROID, then __linux__ is also defined
    printf("defined __linux__\n");
#else
    printf("not defined __linux__\n");
#endif

#if _MSC_VER
    printf("_MSC_VER defined\n");
#else
    printf("_MSC_VER not defined\n");
#endif

#ifdef TI_C66
    print("TI_C66 DSP platform\n");
#endif


#ifdef __ARM_NEON
    printf("defined __ARM_NEON\n");
#endif
}