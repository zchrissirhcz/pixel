#ifdef _MSC_VER
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

#include <thread> // 用于验证多线程 MY_XADD 是否安全的 test 代码编写
#include <mutex> // 用于验证锁操作对 MY_XADD 是否有效的尝试


 #ifdef MY_XADD
   // allow to use user-defined macro
 #elif defined __GNUC__ || defined __clang__
 #  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)  && !defined __INTEL_COMPILER
 #    ifdef __ATOMIC_ACQ_REL
 #      define MY_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
 #    else
 #      define MY_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
 #    endif
 #  else
 #    if defined __ATOMIC_ACQ_REL && !defined __clang__
        // version for gcc >= 4.7
 #      define MY_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
 #    else
 #      define MY_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
 #    endif
 #  endif
 #elif defined _MSC_VER && !defined RC_INVOKED
 #  include <intrin.h>
 #  define MY_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
 #else
   #ifdef OPENCV_FORCE_UNSAFE_XADD
     CV_INLINE MY_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
   #else
     #error "OpenCV: can't define safe MY_XADD macro for current platform (unsupported). Define MY_XADD macro through custom port header (see OPENCV_INCLUDE_PORT_FILE)"
   #endif
 #endif


template <class T>
class Refcount
{
public:
    Refcount();
    explicit Refcount(uint32_t _len);
    Refcount(uint32_t _len, void* _data);
    Refcount(uint32_t _len, void* _data, int* _refcount);
    virtual ~Refcount();

public:
    void* data;
    uint32_t len;
    int* refcount;

    void addref();
    void release();
};

class Array: public Refcount<float>
{
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    Array& operator=(const Array& arr); // 复制赋值构造函数
    virtual ~Array();
public:
    uint32_t len;
};

template<class T>
Refcount<T>::Refcount():
    len(0), data(NULL), refcount(NULL)
{

}

template<class T>
Refcount<T>::Refcount(uint32_t _len):
    len(_len), refcount(new int(0))
{
    if (len>0) {
        data = (T*)malloc(sizeof(T)*len);
        addref();
    }
}

template<class T>
Refcount<T>::Refcount(uint32_t _len, void* _data):
    len(_len), data(_data), refcount(NULL)
{

}

template<class T>
Refcount<T>::Refcount(uint32_t _len, void* _data, int* _refcount):
    len(_len), data(_data), refcount(_refcount)
{
    addref();
}

template<class T>
void Refcount<T>::addref()
{
    if (refcount) {
        MY_XADD(refcount, 1);
    }
}

template<class T>
void Refcount<T>::release()
{
    if (refcount && MY_XADD(refcount, -1) == 1) {
        delete(refcount);
        refcount = NULL;

        free(data);
        data = NULL;
    }
}

template<class T>
Refcount<T>::~Refcount()
{
    release();
}


Array::Array():
    Refcount()
{
}

Array::Array(uint32_t _len)
    :Refcount(_len)
{
}

Array::Array(uint32_t _len, float* _data)
    :Refcount(_len, _data)
{
}

// 拷贝构造函数
Array::Array(const Array& arr)
    :Refcount(arr.len, arr.data, arr.refcount)
{
}

// 复制赋值函数
Array& Array::operator=(const Array& arr)
{
    // 当前array对象有两种状态：refcount不为空，或为空
    // 传入的参数对象arr也有两种状态：refcount不为空，或为空
    // 因此一共有4种状态
    if (this != &arr)
    {
        release();
        refcount = arr.refcount;
        addref();

        data = arr.data;
        len = arr.len;
    }

    // 如果传入的arr就是自己，那什么都不应该做
    // 如果前面的refcount和其他成员的赋值弄好了，也在这里返回
    return *this;
}

Array::~Array()
{
}


// 引用计数测试 - 拷贝构造函数
void test_copy_ctor()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);
        Array arr2 = arr1;
        Array arr3 = arr1;
    }

    {
        Array arr1;
        Array arr2 = arr1;
        Array arr3 = arr2;
    }

    float* data = (float*)malloc(sizeof(float) * 10);
    {
        Array arr1(10, data);
        Array arr2 = arr1;
    }
    free(data);
}

// 引用计数测试 - 复制赋值函数,case1
// 当前计数为0，参数计数也为0
void test_assign_op_case1()
{
    {
        int len = 10;
        float* data = (float*)malloc(sizeof(float) * len);
        {
            Array arr1(len, data);
            Array arr2;

            arr2 = arr1;
        }
        free(data);
    }

    {
        Array arr1;
        Array arr2;
        arr2 = arr1;
    }

    {
        Array arr1;
        arr1 = arr1;
    }
}

// 引用计数测试 - 复制赋值函数,case2
// 当前计数为0，参数计数不为0
void test_assign_op_case2()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2;

        arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);

        Array arr2;
        Array arr3;

        arr2 = arr1;
        arr3 = arr1;
    }
}

// 引用计数测试 - 复制赋值函数,case2
// 当前计数不为0，参数计数为0
void test_assign_op_case3()
{
    {
        Array arr1;

        int len = 10;
        Array arr2(len);

        arr2 = arr1;
    }

    {
        Array arr1;

        int len = 10;
        Array arr2(len);

        Array arr3 = arr2;

        arr2 = arr1;
    }
}

// 引用计数测试 - 复制赋值函数,case2
// 当前计数不为0，参数计数不为0
void test_assign_op_case4()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2(len);

        arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);

        Array arr2 = arr1;

        Array arr3 = Array(len);

        arr2 = arr3;
    }

    {
        Array arr1(10);
        arr1 = arr1;
    }
}

Array g_arr;

void do_work()
{
    int loop_counter = 2000000; // 循环次数；过少则不容易触发线程不安全问题，可以比1000再大一些
    // 触发的问题有三种（均在VS2019 x64 Debug模式下测试发现）：
    // 第一种：内存泄露，vld会报告出来(loop_counter=1000或2000时出现)；非必现，但概率很高
    // 第二种：运行表现为死循环，长时间不出结果 (loop_counter等于2000或更大值时出现），中等概率
    // 第三种：非法内存访问，表现为VS弹窗 Debug Assertion Failed. (loop_counter=2000时出现）
    //
    // 在Release 模式下，相对更难复现问题，需要增加loop_counter到200000，或更大，才能发现“异常结果”
    //   (返回值不等于0，等于-1073740940)
    while (loop_counter--) {
        Array tmp_arr = g_arr;
    }
}


void test_multithread()
{
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();//开始时间

    std::thread th1(do_work);
    std::thread th2(do_work);
    std::thread th3(do_work);
    std::thread th4(do_work);
    std::thread th5(do_work);
    std::thread th6(do_work);
    std::thread th7(do_work);
    std::thread th8(do_work);
    std::thread th9(do_work);
    std::thread th10(do_work);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();

    std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();//结束时间
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << "共耗时：" << time_span.count() << " s" << std::endl; // 耗时
}


int main()
{

    //test_copy_ctor();
    //test_assign_op_case1();
    //test_assign_op_case2();
    //test_assign_op_case3();
    //test_assign_op_case4();

    g_arr = Array(10);
    test_multithread();

    //test_thread_safe();

    return 0;
}
