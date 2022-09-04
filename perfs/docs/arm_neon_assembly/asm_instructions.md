[TOC]

## 1. arm 汇编格式

ARM汇编只有一种格式：
```
指令  目的操作数  源操作数
```

intel: (x86平台)
指令  目的操作数  源操作数

at&t: （x86，power，VAX等）
指令  源操作数  目的操作数

- https://stackoverflow.com/questions/43574163/why-is-gnu-as-syntax-different-between-x86-and-arm
- https://blog.csdn.net/yazhouren/article/details/73692879

## 2. ARM 寄存器
37 个寄存器：
- 30个通用寄存器，32-bit
- pc(program counter)
- CPSR(Current Program Status Register)
- 5个SPSRs(Saved Program Status Registers)


更基础内容，需要看：
-《ARM Developer Suite Assembler Guide》
- 《ARM Assembly Language Fundamentals and Techniques》
- https://developer.arm.com/documentation/dui0473/c/writing-arm-assembly-language
