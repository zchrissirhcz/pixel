#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// modules/core/include/opencv2/core/cvdef.h

/****************************************************************************************\
*          exchange-add operation for atomic operations on reference counters            *
\****************************************************************************************/

// #ifdef CV_XADD
//   // allow to use user-defined macro
// #elif defined __GNUC__ || defined __clang__
// #  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)  && !defined __INTEL_COMPILER
// #    ifdef __ATOMIC_ACQ_REL
// #      define CV_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
// #    else
// #      define CV_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
// #    endif
// #  else
// #    if defined __ATOMIC_ACQ_REL && !defined __clang__
//        // version for gcc >= 4.7
// #      define CV_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
// #    else
// #      define CV_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
// #    endif
// #  endif
// #elif defined _MSC_VER && !defined RC_INVOKED
// #  include <intrin.h>
// #  define CV_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
// #else
//   #ifdef OPENCV_FORCE_UNSAFE_XADD
//     CV_INLINE CV_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
//   #else
//     #error "OpenCV: can't define safe CV_XADD macro for current platform (unsupported). Define CV_XADD macro through custom port header (see OPENCV_INCLUDE_PORT_FILE)"
//   #endif
// #endif



//TODO: https://en.cppreference.com/w/cpp/atomic/atomic/fetch_add 基于这里的例子验证XADD的必要性
#define MY_INLINE inline

MY_INLINE int MY_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }

class Array
{
public:
    Array();
    Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr);
    ~Array();

    void release();

    Array& operator=(const Array& arr);
public:
    float* data;
    uint32_t len;


    //! pointer to the reference counter;
    // when matrix points to user-allocated data, the pointer is NULL
    int* refcount;
};

Array::Array():
    len(0), data(NULL), refcount(0)
{
}

Array::Array(uint32_t _len):
    len(_len)
{
    data = (float*)malloc(sizeof(float)*len);
    refcount = 0;
}

Array::Array(uint32_t _len, float* _data):
    len(_len), data(_data), refcount(0)
{
}

void Array::release()
{
    if( refcount && MY_XADD(refcount, -1) == 1 )
    {
        free(data);
    }
    data = NULL;
    refcount = 0;
}

Array::~Array()
{
    release();
}

Array& Array::operator=(const Array& arr)
{
    if (this!=&arr)
    {
        if (arr.refcount) {
            MY_XADD(arr.refcount, 1);
        }
        release();
        len = arr.len;
        data = arr.data;
    }
    return *this;
}

Array::Array(const Array& arr):
    refcount(arr.refcount)
{
    if (refcount) {
        MY_XADD(refcount, 1);
    }
    len = arr.len;
    data = arr.data;
}

int main()
{
    {
        Array arr(10);
        for(int i=0; i<arr.len; i++) {
            arr.data[i] = i;
        }

        Array b = arr;
    }

    return 0;
}