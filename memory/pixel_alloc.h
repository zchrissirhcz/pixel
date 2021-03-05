#ifndef PIXEL_MEMORY_ALLOC_H
#define PIXEL_MEMORY_ALLOC_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif


#if __AVX__
// the allocated buffer's first address is 32 bytes(256bit) aligned
#define PIXEL_MALLOC_ALIGN 32
#else // SSE/NEON
// the allocated buffer's first address is 16 bytes(128bit) aligned
#define PIXEL_MALLOC_ALIGN 16
#endif

// aligns a pointer to the specified number of bytes
static inline unsigned char** pixel_align_ptr(unsigned char** ptr, int alignbytes)
{
    return (unsigned char**)(((size_t)ptr + alignbytes - 1) & -alignbytes);
}

static inline void* pixel_fast_malloc(size_t size)
{
#if _MSC_VER
    return _aligned_malloc(size, PIXEL_MALLOC_ALIGN);
#elif (defined(__unix__) || defined(__APPLE__)) && _POSIX_C_SOURCE >= 200112L || (__ANDROID__ && __ANDROID_API__ >= 17)
    void* ptr = 0;
    if (posix_memalign(&ptr, PIXEL_MALLOC_ALIGN, size))
        ptr = 0;
    return ptr;
#elif __ANDROID__ && __ANDROID_API__ < 17
    return memalign(PIXEL_MALLOC_ALIGN, size);
#else
    unsigned char* udata = (unsigned char*)malloc(size + sizeof(void*) + PIXEL_MALLOC_ALIGN);
    if (!udata)
        return 0;
    unsigned char** adata = alignPtr((unsigned char**)udata + 1, PIXEL_MALLOC_ALIGN);
    adata[-1] = udata;
    return adata;
#endif
}

static inline void pixel_fast_free(void* ptr)
{
    if (ptr)
    {
#if _MSC_VER
        _aligned_free(ptr);
#elif (defined(__unix__) || defined(__APPLE__)) && _POSIX_C_SOURCE >= 200112L || (__ANDROID__ && __ANDROID_API__ >= 17)
        free(ptr);
#elif __ANDROID__ && __ANDROID_API__ < 17
        free(ptr);
#else
        unsigned char* udata = ((unsigned char**)ptr)[-1];
        free(udata);
#endif
    }
}


#endif // PIXEL_MEMORY_ALLOC_H