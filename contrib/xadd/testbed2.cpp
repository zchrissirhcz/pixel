#include <iostream>
#include <thread>

#include <atomic>
#include <mutex>

// https ://blog.csdn.net/HW140701/article/details/113504936
// https://stackoverflow.com/questions/14718124/how-to-easily-make-stdcout-thread-safe
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


#if 0

#define CV_INLINE inline
CV_INLINE int CV_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }

#else


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

#endif


std::atomic<long long> data;
void do_work()
{
    std::cout << "data is " << data << std::endl;
    data.fetch_add(1, std::memory_order_relaxed);
}

int* refcount;

void do_work2()
{
    

    //printf("thread_id is %d, *refcount is %d\n", std::this_thread::get_id(), *refcount);
    //fprintf(stderr, "thread_id is %d, *refcount is %d\n", std::this_thread::get_id(), *refcount);
    //std::cout << "thead_id is " << std::this_thread::get_id() << ", *refcount is " << *refcount << std::endl;
    //ThreadSafePrint::cout() << "thead_id is " << std::this_thread::get_id() << ", *refcount is " << *refcount << std::endl;

    CV_XADD(refcount, 1);
    
}
 
void test1()
{
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

    std::cout << "Result:" << data << '\n';
}

void test2()
{
    refcount = new int(0);

    std::thread th1(do_work2);
    std::thread th2(do_work2);
    std::thread th3(do_work2);
    std::thread th4(do_work2);
    std::thread th5(do_work2);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();

    std::cout << "Result:" << *refcount << '\n';
}

int main() {
    //test1();
    test2();

    return 0;
}
