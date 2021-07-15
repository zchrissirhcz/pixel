# inline assembler 内联汇编

## 1. 什么是 inline assembly

在 C/C++ 文件中可以嵌入汇编代码。

**代码顺序！= 执行顺序**： 开启优化选项后，编译器的优化规则，可能让代码被执行的顺序和代码出现的顺序不一致。这种不一致未必取得最好的性能（虽然终极目标确实是越来越好）；这样的 gap 使得 inline assembley 仍然有必要存在（但 inline assembly 存在的意义并不仅限于此。）

## 2. inline assembly 语法规则标准

只考虑 ARM 平台的 inline assembly。

编译器： 
- clang： Android NDK 现在应该都是 Clang 了，按说应该看 Clang 的文档中关于 inline assembly 的说明。
- gcc: 关于 gcc 的 inline assembly，无论是它官网还是民间整理，资料都比较多（英文）
- armclang: arm 公司基于 clang 做的编译器， 没用过
    - 通用性？
    - 需要购买吗？

Clang 文档中对 inline assembly 的说明：
- 兼容性：
    - [clang 高度兼容 gcc 的 inline assembly 写法](https://clang.llvm.org/compatibility.html#inline-asm)
    - 个别不支持的报错的，基本上是 GAS 的 bug
- 没找到专门针对 inline assembly 讲解的 clang 文档章节

因此， inline assembly 语法规则:
- 主要参考 GCC 的: 
    - https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
    - https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
- 辅助看看 arm clang 的: https://developer.arm.com/documentation/100067/0612/armclang-Inline-Assembler/Inline-Assembly

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
