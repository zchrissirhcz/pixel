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

TODO: 确定有哪些汇编语句，会影响 Condition Codes

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
