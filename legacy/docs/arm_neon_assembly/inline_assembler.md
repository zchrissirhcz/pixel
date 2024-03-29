# inline assembler 内联汇编

## 1. 什么是 inline assembly

在 C/C++ 文件中可以嵌入汇编代码。包括 Basic inline assembly 和 **Extended inline assembly** (带 operands 和 clobber).

代码顺序！= 执行顺序. 编译器会做优化。

关注的是 ARM 平台的 Extended inline assembly.

## 2. inline assembly 语法规则标准

- GCC 官方手册中 Extended-Asm:  https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
- arm clang 手册: https://developer.arm.com/documentation/100067/0612/armclang-Inline-Assembler/Inline-Assembly
- [clang 高度兼容 gcc 的 inline assembly 写法](https://clang.llvm.org/compatibility.html#inline-asm)
- x86 inline assembly 教程： https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html

## 3. inline assembly 具体语法

### clobber 说明

**armclang**

（https://developer.arm.com/documentation/100067/0612/armclang-Inline-Assembler/Inline-assembly-statements-within-a-function/Clobber-list）

"memory"
    This string tells the compiler that the assembly code might modify any memory, not just variables that are included in the output constraints. 

"cc"
    This string tells the compiler that the assembly code might modify any of the condition flags N, Z, C, or V. In AArch64 state, these condition flags are in the NZCV register. In AArch32 state, these condition flags are in the CPSR register. 

**gcc**

https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html：

"cc"
    The "cc" clobber indicates that the assembler code modifies the flags register. On some machines, GCC represents the condition codes as a specific hardware register; "cc" serves to name this register. On other machines, condition code handling is different, and specifying "cc" has no effect. But it is valid no matter what the target.

"memory"
    The "memory" clobber tells the compiler that the assembly code performs memory reads or writes to items other than those listed in the input and output operands (for example, accessing the memory pointed to by one of the input parameters). To ensure memory contains correct values, GCC may need to flush specific register values to memory before executing the asm. Further, the compiler does not assume that any values read from memory before an asm remain unchanged after that asm; it reloads them as needed. Using the "memory" clobber effectively forms a read/write memory barrier for the compiler. 

也就是说， "cc" 对应 32位的 CPSR 、 64位的 NZCV 状态寄存器。Condition Codes 简称 cc。

### Condition Codes

CPSR (Current Program Status Register)
>ARM v6/v7 maintains a status register called the CPSR (current program status register) that holds four status bits, negative (N), zero (Z), carry (C), and overflow (O). These bits can be used for conditional execution of subsequent instructions.

(https://www.sciencedirect.com/topics/computer-science/current-program-status-register)

所有的数据处理指令(data processing instruction)，如果带了“S”后缀，就会根据执行结果，更新CPSR中的ALU状态标志。

CMP, CMN, TST, TEQ 这些指令不需要带后缀S，它们本身就是会更新flags的。

CPSR寄存器包含了如下的ALU状态标志位：
N    当操作的结果是Negative的时候，设置这一位
Z    当操作的结果是Zero的时候，设置这一位
C    当操作的结果有Carry的时候，设置这一位
V    当操作的结果有oVerflow（溢出）的时候，设置这一位

Carry位在这些时候才会设置：
- 加法操作，结果超过 2^32
- 减法操作，结果是正的
- 作为移动或逻辑操作中，barrel shifter操作的结果

Overflow位在这些时候才会设置：
- 加法、减法、比较操作的结果，大于等于 2^31， 或小于 -2^31




Condition Codes table:

| Sufix   |  Flags                       |   Meaning        |
|-------- | ---------------------------- | ---------------- |
| eq      | z set                        | Equal            |
| ne      | z clear                      | Not equal        |
| hs      | c set                        | Unsigned >=      |
| lo      | c clear                      | Unsigned <       |
| mi      | n set                        | Negative         |
| pl      | n clear                      | Positive or zero |
| vs      | v set                        | Overflow         |
| vc      | v clear                      | No overflow      |
| hi      | c set and z clear            | Unsigned >       |
| ls      | c clear and z set            | Unsigned <=      |
| ge      | n and v the same             | Signed >=        |
| lt      | n and v different            | Signed <         |
| gt      | z clear, or n and v the same | Signed >         |
| le      | z set, or n and v different  | Signed <=        |

(Table 1.1 from "Embedded Systems - ARM Programming and Optimization")


问题探讨：如果一个汇编指令修改了 CPSR， 但是 clobber 里没有写明 cc ， 并且 asm volatile 中的汇编指令也不依赖于 CPSR， 是否 OK？

个人认为有影响，原因：
- 上下文切换（进程线程切换）时，其他进程线程可能依赖于 CPSR
- 中断
（参考了 http://www.cxyzjd.com/article/xiaojsj111/14648233 ，但不完全认同）


TODO: 如何检查 CPSR 中的 oVerflow 位？

TODO：如果计算结果确实溢出了，能否影响到下一条指令？

### Label 的使用

ncnn 中用的都是数字 label， MNN 作者 JXT 的[博客](https://blog.csdn.net/jxt1234and2010/article/details/104012746) 则是用的 “字符串” 的 label。

博客里字符串 label 的使用，是在单独的 .S 中，没问题。但放到 inline assembly 中就容易出问题；我遇到的情况是，结合了博客的流水重排 和 ncnn 的 inline assembly 方式，字符串 label 被编译器判断为 重复定义 label:

>error: invalid symbol redefinition

https://stackoverflow.com/questions/68670074/invalid-symbol-redefinition-for-arm-inline-assembly

https://stackoverflow.com/questions/3898435/labels-in-gcc-inline-assembly

解决方法：
- 第一种， 修改 inline assembly 中的字符串 label 为数字 label；并且注意 `bne 1f` 和 `bne 1b` 的区别
    - b 表示 backward， f 表示 forward， 它们的区别是跳转 label 的查找方向
```c++
        asm volatile(
            "movi v0.8b, #77 \n"
            "movi v1.8b, #151 \n"
            "movi v2.8b, #28 \n"
            "prfm pldl1keep, [%0, #512] \n"
            "ld3  {v3.16b-v5.16b}, [%0], #48 \n"
            "ext  v6.16b, v3.16b, v3.16b, #8 \n"
            "ext  v7.16b, v4.16b, v4.16b, #8 \n"
            "ext  v8.16b, v5.16b, v5.16b, #8 \n"
            "subs %w2, %w2, #1 \n"
            "ble 1f \n"
            "0: \n"
            "prfm  pldl1keep, [%0, #512] \n"
            "ld3   {v3.16b-v5.16b}, [%0], #48 \n"
            "ext   v6.16b, v3.16b, v3.16b, #8 \n"
            "ext   v7.16b, v4.16b, v4.16b, #8 \n"
            "ext   v8.16b, v5.16b, v5.16b, #8  \n"
            "umull v9.8h,  v3.8b, v0.8b \n"
            "umull v10.8h, v6.8b, v0.8b  \n"
            "umull v11.8h, v4.8b, v1.8b  \n"
            "umlal v9.8h,  v5.8b, v2.8b  \n"
            "umull v12.8h, v7.8b, v1.8b  \n"
            "umlal v10.8h, v8.8b, v2.8b  \n"
            "addhn v13.8b,  v9.8h,  v11.8h \n"
            "addhn2 v13.16b, v10.8h, v12.8h \n"
            "subs %w2, %w2, #1 \n"
            "st1 {v13.16b}, [%1], #16 \n"
            "bne 0b \n"
            "1: \n"
            "umull  v9.8h,  v3.8b, v0.8b \n"
            "umull  v10.8h, v6.8b, v0.8b \n"
            "umull  v11.8h, v4.8b, v1.8b \n"
            "umlal  v9.8h,  v5.8b, v2.8b \n"
            "umull  v12.8h, v7.8b, v1.8b \n"
            "umlal  v10.8h, v8.8b, v2.8b \n"
            "addhn  v13.8b,  v9.8h,  v11.8h \n"
            "addhn2 v13.16b, v10.8h, v12.8h \n"
            "st1    {v13.16b}, [%1], #16 \n"
            : "=r"(rgb_line), // %0
              "=r"(gray_line), // %1
              "=r"(nn) // %2
            : "0"(rgb_line),
              "1"(gray_line),
              "2"(nn)
            : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"
        );
```
- 第二种， 字符串 label 增加 `%=` 后缀，注意在声明 label 和 跳转 label 时，都加
```c++
        asm volatile(
            "movi v0.8b, #77 \n"
            "movi v1.8b, #151 \n"
            "movi v2.8b, #28 \n"
            "prfm pldl1keep, [%0, #512] \n"
            "ld3  {v3.16b-v5.16b}, [%0], #48 \n"
            "ext  v6.16b, v3.16b, v3.16b, #8 \n"
            "ext  v7.16b, v4.16b, v4.16b, #8 \n"
            "ext  v8.16b, v5.16b, v5.16b, #8 \n"
            "subs %w2, %w2, #1 \n"
            "ble L4End%= \n"
            "L4Loop%= : \n"
            "prfm  pldl1keep, [%0, #512] \n"
            "ld3   {v3.16b-v5.16b}, [%0], #48 \n"
            "ext   v6.16b, v3.16b, v3.16b, #8 \n"
            "ext   v7.16b, v4.16b, v4.16b, #8 \n"
            "ext   v8.16b, v5.16b, v5.16b, #8  \n"
            "umull v9.8h,  v3.8b, v0.8b \n"
            "umull v10.8h, v6.8b, v0.8b  \n"
            "umull v11.8h, v4.8b, v1.8b  \n"
            "umlal v9.8h,  v5.8b, v2.8b  \n"
            "umull v12.8h, v7.8b, v1.8b  \n"
            "umlal v10.8h, v8.8b, v2.8b  \n"
            "addhn v13.8b,  v9.8h,  v11.8h \n"
            "addhn2 v13.16b, v10.8h, v12.8h \n"
            "subs %w2, %w2, #1 \n"
            "st1 {v13.16b}, [%1], #16 \n"
            "bne L4Loop%= \n"
            "L4End%= : \n"
            "umull  v9.8h,  v3.8b, v0.8b \n"
            "umull  v10.8h, v6.8b, v0.8b \n"
            "umull  v11.8h, v4.8b, v1.8b \n"
            "umlal  v9.8h,  v5.8b, v2.8b \n"
            "umull  v12.8h, v7.8b, v1.8b \n"
            "umlal  v10.8h, v8.8b, v2.8b \n"
            "addhn  v13.8b,  v9.8h,  v11.8h \n"
            "addhn2 v13.16b, v10.8h, v12.8h \n"
            "st1    {v13.16b}, [%1], #16 \n"
            : "=r"(rgb_line), // %0
              "=r"(gray_line), // %1
              "=r"(nn) // %2
            : "0"(rgb_line),
              "1"(gray_line),
              "2"(nn)
            : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13"
        );
```

那么，为什么会出现，有些函数用字符串 label 就可以，有些就不可以呢？经验证，是`template<int b_idx>`导致的，尽管只特化一个实现，但是，编译器做的处理和普通函数的处理是不一样的。去掉template就可以编译过了。

坚持用 字符串 label？可以，只需要修改 `some_label` 为 `some_label%=`.



https://stackoverflow.com/tags/inline-assembly/info