# 基于 XADD 实现引用计数


RAII 用来更好的管理内存资源，而对于 C++11 之前没有内置的指针情况下，OpenCV 的 Mat 类是怎样实现引用计数的？

主要是在 Mat 类的构造函数、析构函数以及它们间接调用的 initEmtpy、create、release等函数里，对 refcount 这个 `int*` 类型变量做记录：引用加一，则 refcount 指向的值 +1；引用减1，则 refcount 指向的值 -1。当释放的时候如果判断 refcount 指向的值等于 0，则释放内存。这样一来，内存既不会泄露，也不会被多次释放。

这其中，refcount 每次的变化只有两种情况：
- refcount 指向的值 +1，用 CV_XADD(refcount, 1) 来实现
- refcount 指向的值 -1，用 CV_XADD(refcount, -1) 来实现

考虑到多线程场景下，同一个 Mat 对象可能被多个线程访问，因此需要确保 CV_XADD 是原子性的

`CV_XADD`是怎样定义的？从源码文件`opencv/modules/core/include/opencv2/core/cvdef.h`可以看到：

```c++
/****************************************************************************************\
*          exchange-add operation for atomic operations on reference counters            *
\****************************************************************************************/

#ifdef CV_XADD
  // allow to use user-defined macro
#elif defined __GNUC__ || defined __clang__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)  && !defined __INTEL_COMPILER
#    ifdef __ATOMIC_ACQ_REL
#      define CV_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define CV_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
       // version for gcc >= 4.7
#      define CV_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define CV_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && !defined RC_INVOKED
#  include <intrin.h>
#  define CV_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
  #ifdef OPENCV_FORCE_UNSAFE_XADD
    CV_INLINE int CV_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
  #else
    #error "OpenCV: can't define safe CV_XADD macro for current platform (unsupported). Define CV_XADD macro through custom port header (see OPENCV_INCLUDE_PORT_FILE)"
  #endif
#endif
```

简单说，如果你不理解 `CV_XADD`，那你就当它是如下函数：
```c++
CV_INLINE int CV_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
```

而这个函数，在多线程场景下是有问题的，会被打断执行，因此不能用，替代品则是各个平台编译器内置的函数，这些函数自身确保了原子性，功能上也和这个naive版本的CV_XADD是一致的。

## CV_XADD 线程安全说明：举例

```c++
#include <thread>
#include <stdio.h>
#include <iostream>

int* refcount;

#include <atomic>
#include <mutex>

// https ://blog.csdn.net/HW140701/article/details/113504936
namespace ThreadSafePrint
{
    static std::mutex m_CoutMutex;
    struct cout
    {
        std::unique_lock<std::mutex> m_Lock;
        cout() :
            m_Lock(std::unique_lock<std::mutex>(m_CoutMutex))
        {

        }

        template<typename T>
        cout& operator<<(const T& message)
        {
            std::cout << message;
            return *this;
        }

        cout& operator<<(std::ostream& (*fp)(std::ostream&))
        {
            std::cout << fp;
            return *this;
        }
    };
}

void do_work()
{
    //printf("thread_id is %d, *refcount is %d\n", std::this_thread::get_id(), *refcount);
    //fprintf(stderr, "thread_id is %d, *refcount is %d\n", std::this_thread::get_id(), *refcount);
    //std::cout << "thead_id is " << std::this_thread::get_id() << ", *refcount is " << *refcount << std::endl;
    ThreadSafePrint::cout() << "thead_id is " << std::this_thread::get_id() << ", *refcount is " << *refcount << std::endl;
    CV_XADD(refcount, 1);
}

int main()
{
    refcount = new int(0);

    std::thread th1(do_work);
    std::thread th2(do_work);
    std::thread th3(do_work);
    std::thread th4(do_work);
    std::thread th5(do_work);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();

    return 0;
}
```

上述代码创建5个线程，以及一个全局对象 refcount 指针；在每个线程内，都调用 `CV_XADD`，使 refcount 指向的值 +1 ，然后每个线程内部打印 refcount 指向的值。

几个细节予以说明：
- 用 C++11 的 std::thread 创建线程，是为了保证跨平台能执行；CV_XADD 本身并不需要依赖 C++11
- 每个线程内部，打印线程 id 和 `*refcount`，是调用线程安全的 `ThreadSafePrint::cout()` 来做的，这是为了避免 printf/std::cout 本身线程不安全（或安全但显示结果顺序乱掉）带来的影响
- 关于线程安全的 std::cout 的替代，上述`ThreadSafePrint::cout()`是从 CSDN 拷贝来的，不一定对；Stack Overflow上的讨论在这儿 https://stackoverflow.com/questions/14718124/how-to-easily-make-stdcout-thread-safe
- 需要自行添加 `CV_XADD` 的定义，从本文最上面提到的 opencv 代码里拷贝过去，或用 naive 实现的函数版
- 好吧，目前并没有说明 CV_XADD 本身的线程不安全、安全问题；反倒是 printf/std::cout/ThreadSafePrint::cout() 的表现会影响 CV_XADD...


TODO: 再研究一下 StackOverFlow 上的那些回答