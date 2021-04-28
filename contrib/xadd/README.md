# 浅析 CV_XADD：以实现 Array 类为例

[TOC]

## 0x0 写在前面

我们尝试实现动态数组类。无论是一维数组，二维矩阵，还是多维张量，数据字段都可以用打扁的一维原生动态数组来表示，辅以必要的维度信息即可完成表示。弄懂一维数组类的RAII，就可以无缝迁移到多维数组类上。

所谓 RAII 个人简单理解为，在类的 ctor（构造函数，constructor，简称ctor）中申请内存。

## 0x1 第一版 Array 类：析构函数两难境地

C++标准用 98/03。

代码文件是 array_v1.cpp

<details>

```c++
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    ~Array();

public:
    float* data;
    int len;
};

Array::Array():
    len(0), data(NULL)
{

}

Array::Array(uint32_t _len):
    len(_len)
{
    data = (float*)malloc(sizeof(float) * len);
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data)
{

}

Array::~Array()
{
    //free(data);
}

int main()
{
    {
        int len = 10;
        Array arr1(len);
        for (int i = 0; i < len; i++) {
            arr1.data[i] = i;
        }

        Array arr2 = arr1;

        // 当程序运行到大括号外，arr1和arr2的生命周期结束，会分别调用析构函数
        // 由于Array类没有实现拷贝构造函数，且成员都是平凡类型，则共享data和len成员
        // 如果Array的析构函数里释放data字段，则造成二次释放触发crash
        // 如果Array的析构函数里不释放data字段，则造成内存泄露
    }
    

    return 0;
}

```

</details>

这里的问题在于，没有实现拷贝构造函数则走编译器生成的默认拷贝构造函数，析构函数陷入两难境地：如果释放data指向的内存则触发二次释放（crash），如果不释放则内存泄露。

## 0x2 第二版 Array 类：refcount 初次尝试

我们了解到引用计数的大概思想，因而 Array 类不进需要 data 成员，还需要一个用来记录 data 被多少个 Array 对象共享着的数量。

简单地在 Array 类定义中增加 int 类型的 refcount；构造函数中如果不做内存分配，则 refcount 初始化为 0；增加了拷贝构造函数，尝试在里面增加 refcount 的值。然而实现拷贝构造函数的时候犯难了：const 引用类型的变量，不能修改它的 refcount 成员。

代码文件时 array_v2.cpp

<details>

```c++
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    ~Array();

public:
    float* data;
    int len;

private:
    int refcount; // 增加了 int 类型的引用计数器
};

Array::Array():
    len(0), data(NULL), refcount(0) // 构造函数，不分配内存，refcount初始化为0
{

}

Array::Array(uint32_t _len):
    len(_len), refcount(0)
{
    data = (float*)malloc(sizeof(float) * len);
    refcount++; // 构造函数，分配内存，refcount初始化为0，分配内存后增加1
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(0) // 构造函数，不分配内存，refcount初始化为0
{

}

// 拷贝构造函数
Array::Array(const Array& arr)
{
    arr.refcount++; // IDE报红，不让改 arr.refcount
}

Array::~Array()
{
    //free(data);
}

int main()
{
    {
        int len = 10;
        Array arr1(len);
        for (int i = 0; i < len; i++) {
            arr1.data[i] = i;
        }

        Array arr2 = arr1;

        // 当程序运行到大括号外，arr1和arr2的生命周期结束，会分别调用析构函数
        // 由于Array类没有实现拷贝构造函数，且成员都是平凡类型，则共享data和len成员
        // 如果Array的析构函数里释放data字段，则造成二次释放触发crash
        // 如果Array的析构函数里不释放data字段，则造成内存泄露
    }
    

    return 0;
}

```

<details>

看来，refcount 的思想有了，不代表能无脑实现基于 refcount 的、内存正确释放的 Array 类。

## 0x3 第三版 Array 类：refcount 类型改 `int*`，拷贝构造函数和析构函数的修正

基于前一版的报错，修改 refcount 类型，从 int 改为 `int *`，那么 refcount 本身也需要申请释放内存；只要 refcount 的值（也就是指向的内存空间的起始地址）不变，拷贝构造函数就能顺利写好。实现如下：

对应代码在 array_v3.cpp

</details>

```c++
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    ~Array();

public:
    float* data;
    int len;

private:
    int* refcount; // 增加了 int 类型的引用计数器
};

Array::Array():
    len(0), data(NULL), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

Array::Array(uint32_t _len):
    len(_len), refcount(new int(0))
{
    data = (float*)malloc(sizeof(float) * len);
    *refcount = *refcount + 1; // 构造函数，分配内存，refcount 分配内存，然后+1
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

// 拷贝构造函数
Array::Array(const Array& arr):
    refcount(arr.refcount) // 用 arr.refcount赋值给refcount
{
    if(refcount) {
        // 如果 arr.refcount 不等于 NULL，说明 arr 是自己管理 data 数据
        // 那么现在要和 arr 对象共享 data 从而构造一个新的 Array 对象，就需要计数器+1
        *refcount = *refcount + 1;
    }
    len = arr.len;
    data = arr.data;
}

Array::~Array()
{
    // 如果 refcount 不为 NULL，说明 data 字段是需要管理的堆内存
    if (refcount && (*refcount)>0 ) { // 如果 refcount 不为 NULL，并且 *refcount > 0，说明至少有1个Array对象，绑定着data字段
        *refcount = *refcount - 1; // 那么就让计数器减1
        
        if (*refcount == 0) { // 计数器减去1后，值为0，说明没有人再指向data这块内存了
            free(data); // 则释放这块内存
            data = NULL;
            free(refcount); // 并且引用计数器也要释放掉
            refcount = NULL;
        }
    }
}

int main()
{
    {
        int len = 10;
        Array arr1(len);
        for (int i = 0; i < len; i++) {
            arr1.data[i] = i;
        }

        Array arr2 = arr1;

        Array arr3;
        arr3 = arr1;

        Array arr4(len);
        arr4 = arr1;

        // arr1, arr2
        // 当程序运行到大括号外，arr1和arr2的生命周期结束，会分别调用析构函数
        // 由于增加了引用计数器，并且拷贝构造函数、析构函数中，根据引用计数器来释放内存
        // 因此，看起来不会内存泄露或二次释放了（对于单线程的拷贝构造函数，确实如此）

        // arr1, arr3
        // 由于arr3创建时没指定参数，因此不用自己申请和管理内存
        // 把arr1赋值给arr3时，由于没定义复制赋值函数，调用编译器生成的默认的复制赋值函数
        // 也就是 len 和 data 字段，完全相同
        // 这其实不合理：因为明明有 arr1 和 arr3 两个对象在共享 data 字段，计数器的值却是1
        // 虽然析构函数里不会二次释放或内存泄露，但如果arr1的声明周期结束、arr3没结束，
        // 则后续arr3访问data对象，就是非法访问了

        // arr1, arr4
        // 这里的问题在于，arr4创建的时候指定了len参数，因此需要自行创建和管理自己的data，也就是堆内存
        // 而由于没有实现Array类的复制赋值函数，调用了编译器生成的默认复制赋值函数
        // 因此，arr1的len和data字段赋值给arr4对象，而arr4对象原来的data则无人管理
        // 这就造成了内存泄露
    }
    

    return 0;
}
```

</details>

在 `main()` 函数中测试，拷贝构造函数和析构函数有所更新，它们先前不正确实现导致的内存泄露或二次释放问题得以解决；然而来了新的问题，如果是通过复制赋值函数进行操作，仍然可能导致内存泄露或非法访问。

## 0x4 第四版 Array 类：复制赋值函数的修正

对应代码在 array_v4.cpp

<details>

```C++
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    Array& operator=(const Array& arr); // 复制赋值构造函数
    ~Array();

public:
    float* data;
    int len;

private:
    int* refcount; // 增加了 int 类型的引用计数器
};

Array::Array():
    len(0), data(NULL), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

Array::Array(uint32_t _len):
    len(_len), refcount(new int(0))
{
    data = (float*)malloc(sizeof(float) * len);
    *refcount = *refcount + 1; // 构造函数，分配内存，refcount 分配内存，然后+1
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

// 拷贝构造函数
Array::Array(const Array& arr):
    refcount(arr.refcount) // 用 arr.refcount赋值给refcount
{
    if(refcount) {
        // 如果 arr.refcount 不等于 NULL，说明 arr 是自己管理 data 数据
        // 那么现在要和 arr 对象共享 data 从而构造一个新的 Array 对象，就需要计数器+1
        *refcount = *refcount + 1;
    }
    len = arr.len;
    data = arr.data;
}

Array& Array::operator=(const Array& arr)
{
    // 当前array对象有两种状态：refcount不为空，或为空
    // 传入的参数对象arr也有两种状态：refcount不为空，或为空
    // 因此一共有4种状态
    if (this != &arr)
    {
        //0 0
        if (this->refcount == NULL && arr.refcount == NULL) {
            data = arr.data;
            len = arr.len;
        }

        //0 1
        else if (this->refcount == NULL && arr.refcount != NULL) {
            data = arr.data;
            len = arr.len;
            refcount = arr.refcount;
            *refcount = *refcount + 1;
        }

        //1 0
        else if (this->refcount != NULL && arr.refcount == NULL) {
            // 首先让this->refcount减少一次引用
            *refcount = *refcount - 1;
            // 如果refcount=0了，那应该释放data和refcount
            if (*refcount == 0) {
                free(data);
                free(refcount);
                data = NULL;
                refcount = NULL;
            }

            // 然后从arr 赋值
            data = arr.data;
            len = arr.len;
        }

        //1 1
        else if (this->refcount != NULL && arr.refcount != NULL) {
            *refcount = *refcount - 1;
            if (*refcount == 0) {
                free(data);
                free(refcount);
                data = NULL;
                refcount = NULL;
            }

            data = arr.data;
            len = arr.len;
            refcount = arr.refcount;
            *refcount = *refcount + 1;
        }
    }

    // 如果传入的arr就是自己，那什么都不应该做
    // 如果前面的refcount和其他成员的赋值弄好了，也在这里返回
    return *this; 
}

Array::~Array()
{
    // 如果 refcount 不为 NULL，说明 data 字段是需要管理的堆内存
    if (refcount && (*refcount)>0 ) { // 如果 refcount 不为 NULL，并且 *refcount > 0，说明至少有1个Array对象，绑定着data字段
        *refcount = *refcount - 1; // 那么就让计数器减1
        
        if (*refcount == 0) { // 计数器减去1后，值为0，说明没有人再指向data这块内存了
            free(data); // 则释放这块内存
            data = NULL;
            free(refcount); // 并且引用计数器也要释放掉
            refcount = NULL;
        }
    }
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


int main()
{

    test_copy_ctor();
    test_assign_op_case1();
    test_assign_op_case2();
    test_assign_op_case3();
    test_assign_op_case4();

    return 0;
}

```

</details>

这一版实现了复制赋值函数，里面根据当前对象和参数对象的refcount情况分别讨论；为了验证正确性，每一种case增加了测试代码。经测试，没有内存泄露，也没有二次释放。（如果发现有遗漏的case，欢迎指出）

## 0x5 第五版 Array 类：封装为addref() 和 release()函数

发现上述代码中存在重复代码片段，`*refcount = *refcount + 1`出现了4次，`*refcount = *refcount - 1`出现了三次。考虑封装到函数中统一调用。

代码文件是 array_v5.cpp

<details>

```c++
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    Array& operator=(const Array& arr); // 复制赋值构造函数
    ~Array();

public:
    float* data;
    int len;

private:
    int* refcount; // 引用计数器
    void addref();
    void release();
};

// 引用计数器增加1次
void Array::addref() {
    if (refcount) {
        *refcount = *refcount + 1;
    }
}

// 引用计数器减少1次
// 如果计数器等于0，则释放data和refcount并且置NULL
void Array::release() {
    if (refcount && (*refcount) > 0) { // 如果 refcount 不为 NULL，并且 *refcount > 0，说明至少有1个Array对象，绑定着data字段
        // 那么就让当前对象计数器减1，并检查按需释放
        *refcount = *refcount - 1;
        if (*refcount == 0) {
            free(data);
            delete(refcount);
            data = NULL;
            refcount = NULL;
        }
    }
}

Array::Array():
    len(0), data(NULL), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

Array::Array(uint32_t _len):
    len(_len), refcount(new int(0))
{
    data = (float*)malloc(sizeof(float) * len);
    addref(); // 构造函数，分配内存，refcount 分配内存，然后+1
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

// 拷贝构造函数
Array::Array(const Array& arr):
    refcount(arr.refcount) // 用 arr.refcount赋值给refcount
{
    addref();

    len = arr.len;
    data = arr.data;
}

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
    release();
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


int main()
{

    test_copy_ctor();
    test_assign_op_case1();
    test_assign_op_case2();
    test_assign_op_case3();
    test_assign_op_case4();

    return 0;
}

```

</details>


## 0x6 第六版 Array 类：计数器增减的统一写法 MY_XADD

上一个版本已经做了“相同代码封装为函数”的操作，但还不够；计数器每次要么+1要么-1，也可以统一封装为一个函数。

这个封装出来的函数叫做 `MY_XADD`，由于需要改变`*refcount`，因此需要传入`int* refcount`，以及改变多少`int delta`。

先前的`release()`函数定义如下：
```c++
void Array::release() {
    if (refcount && (*refcount) > 0) { // 如果 refcount 不为 NULL，并且 *refcount > 0，说明至少有1个Array对象，绑定着data字段
        // 那么就让当前对象计数器减1，并检查按需释放
        *refcount = *refcount - 1;
        if (*refcount == 0) {
            free(data);
            delete(refcount);
            data = NULL;
            refcount = NULL;
        }
    }
}
```

其实有点蹩脚：对`*refcount`的检查有两次，对应两个 if 判断。如果在第一次 if 判断的时候顺带让`*refcount`减1，则第一次 if 只需要判断“减1”之前是否等于1。

代码文件是 array_v6.cpp，关键代码如下：

```c++
static int MY_XADD(int* addr, int delta) {
    int tmp = *addr;
    *addr += delta;
    return tmp;
}

// 引用计数器增加1次
void Array::addref() {
    if (refcount) {
        MY_XADD(refcount, 1);
    }
}

// 引用计数器减少1次
// 如果计数器等于0，则释放data和refcount并且置NULL
void Array::release() {
    // 如果 refcount 不为 NULL，那么：
    //    更新计数器： *refcount = *refcount - 1
    //    判断如果原来的计数器值为1，那么现在计数器的值为0，应当释放相关内存
    if (refcount && MY_XADD(refcount, -1)==1) {
        // end for refcount
        delete(refcount);
        refcount = NULL;

        // end for Array object's shared heap memory variable(s)
        free(data);
        data = NULL;
    }
}
```

## 0x7 第七版 Array 类：多线程不安全的情况

如果两个或多个 Array 对象共享 data 数据，并且这些 Array 对象是处于不同线程中的，那么对于refcount以及data的访问，都可能不安全。简单说，`MY_XADD`的实现过于naive，容易被“打断”，一部分在A线程执行，另一部分在B线程执行。

我们尝试构造测试代码，复现这个问题。完整代码在 array_v7，关键代码如下：

```c++

Array g_arr;

void do_work()
{
    int loop_counter = 20000; // 循环次数；过少则不容易触发线程不安全问题，可以比1000再大一些
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
}

int main()
{
    g_arr = Array(10);
    test_multithread();
    return 0;
}

```

上述代码是开启10个线程，每个线程都去执行 `do_work()` 函数，它里面会从全局 Array 对象 `g_arr` 赋值给一个临时的 Array 对象 `tmp_arr`。

经测试，vs2019 x64 的 Debug 模式下，当 `test_multithread()`里的循环次数增大到2000时，就能容易触发内存泄露、非法访问、长时间跑不出结果（似乎是死锁？）情况。

而在 vs2019 x64 的 Release 模式下，复现问题难度增加，循环次数需要更大，才能跑出上述“非正常”情况。


## 0x8 第八版 Array 类：refcount 用 volatile 就安全了吗？

就上述测试函数`test_multithread()`来说，对 Array 对象的 data 共享访问，都是根据 refcount 判断 OK 才访问的。很容易找到一些“祖传”代码或博客，说“用volatile就可以保证线程安全”。我们假设 volatile 修饰 refcount 后确实能解决问题，那么 `test_multithread()`期待的结果是不再触发内存泄露、访问越界、长时间跑不出结果。

对应的完整代码文件是 array_v8.cpp，关键代码如下：
```c++
class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    Array& operator=(const Array& arr); // 复制赋值构造函数
    ~Array();

public:
    float* data;
    int len;

private:
    volatile int* refcount; // 引用计数器。使用 volatile 修饰，希望能解决多线程访问冲突
    void addref();
    void release();
};

static int MY_XADD(volatile int* addr, int delta) {
    int tmp = *addr;
    *addr += delta;
    return tmp;
}
```

经实际测试（依然在VS2019 x64 debug/release环境下），仍然能触发内存泄露。时间关系，没有测试其他两种非正常情况是否也会发生。

也就是：用反证法证明了一个事情：C++中，volatile不能解决多线程访问冲突问题。这是因为volatile无法保证原子性。

**实际上：`volatile`仅仅是说，从内存而不是寄存器取出变量的值，也就是阻止了 refcount 变量被编译器优化。这和线程安全没有任何关系。编译器优化后的代码是乱序执行的，稍微复杂一些的代码中，会根据 refcount 变量的不同值，来取另一个变量 result 的值，这个 result 对应的代码行也许根本就没有被执行（CPU乱序执行导致的），导致取出来的结果是错误的，而这种错误的排查是非常困难的。**

## 0x9 第九版 MY_XADD 原子性探讨

实际上前一个版本中，对 refcount 的数据类型读写，大方向是正确的；只不过需要清楚，要保证线程安全的访问和修改 refcount，应该用原子操作，而不是无脑的认为“volatile保安全”。

确保原子性访问，我知道这几种方法：
- 访问前加锁，访问后解锁。例如 std::mutex（C++11）
- 用 std::atomic 类型（C++11）
- 平台特定 API

**基于std::mutex的锁操作**

基于 std::mutex ，在 MY_XADD 里的开始处加锁，return 前解锁。

完整代码在 array_v9_std_mutex.cpp， 关键代码如下：
```c++
std::mutex g_mutex;

static int MY_XADD(int* addr, int delta) {
    g_mutex.lock();

    int tmp = *addr;
    *addr += delta;
    
    g_mutex.unlock();
    return tmp;
}
```

**基于std::atomic的操作**

完整代码在 array_v9_std_atomic.cpp， 关键代码如下：

```c++
class Array{
    ...
    std::atomic<int*>refcount;
    ...
}

static int MY_XADD(std::atomic<int>* addr, int delta) {
    int tmp = *addr;
    *addr += delta;

    return tmp;
}

// 拷贝构造函数
Array::Array(const Array& arr) :
    refcount(arr.refcount.load()) // 用 arr.refcount赋值给refcount
{
    addref();

    len = arr.len;
    data = arr.data;
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
        refcount = arr.refcount.load();
        addref();

        data = arr.data;
        len = arr.len;
    }

    // 如果传入的arr就是自己，那什么都不应该做
    // 如果前面的refcount和其他成员的赋值弄好了，也在这里返回
    return *this;
}
```

**基于特定平台 API 的操作**

完整代码在 array_v9_system_api.cpp， 关键代码如下：

```c++
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
```

其中，对于 MSVC，根据特定 API，定义 MY_XADD 为：
```c++
# define MY_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
```

分别基于上述方案实现的 MY_XADD，在 `do_work()` 函数中设定循环次数为 `int loop_counter = 2000000;`，分别测量三种方案的时间开销。

Windows 下测量得到：

| 方案                    | 耗时        |
| ----------------------- | --------   |
| std::mutex              | 4.39776 s  |
| std::atomic             | 7.4746 s   |
| _InterlockedExchangeAdd | 0.713134 s |







```
无锁大部分时间不如整一个高性能spin lock 来的快速且逻辑自洽不埋坑……比如atomic 的int要赋值或者拷贝给正常的int 就要格外小心……但不是所有队友都会小心……
```

```
不应将atomic和lock free混淆。atomic只是一个声明，表示“对此数据的访问是原子性的”，但是c++标准并不指定atomic的实现，atomic可能是由lock free实现，也可能由锁来实现。参考http://http://15418.courses.cs.cmu.edu/fall2017/lecture/lockfree/slide_006
```


还有一些不太了解、似乎也可以用的方法：
- spin lock
- 




我们用 C++11 中提供的 std::mutex 来做加锁解锁操作。



## 0xx References

[C++性能榨汁机之无锁编程](https://zhuanlan.zhihu.com/p/38664758)

`float* data`共享数据，内存问题：
- 内存泄露
- 内存重复释放

基于 RAII 