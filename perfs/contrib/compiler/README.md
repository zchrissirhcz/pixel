[GCC官方在线手册](https://gcc.gnu.org/onlinedocs/gcc/)

[GCC - 二进制兼容性](https://gcc.gnu.org/onlinedocs/gcc/Compatibility.html#Compatibility)

`CMAKE_BUILD_TYPE=Release`情况下，并不会开启`-Ofast`。`-Ofast`会做向量化展开：
```cmake
add_definitions(-Ofast)
```

效果：对如下C++代码：
```c++
void rgb2gray_weighted(const unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t total_len = height * width;
    int16_t gray;
    for(size_t i=0; i<total_len; i++) {
        *gray_buf = (77*rgb_buf[0] + 151*rgb_buf[1] + 28*rgb_buf[2]) >> 8;
        gray_buf++;
        rgb_buf += 3;
    }
}
```

Release（也就是O2）反汇编：
```asm
00000000 <_Z17rgb2gray_weightedPKhjjPh>:
   0:	b5f0      	push	{r4, r5, r6, r7, lr}
   2:	af03      	add	r7, sp, #12
   4:	f84d bd04 	str.w	fp, [sp, #-4]!
   8:	4351      	muls	r1, r2
   a:	f04f 0c97 	mov.w	ip, #151	; 0x97
   e:	244d      	movs	r4, #77	; 0x4d
  10:	b181      	cbz	r1, 34 <_Z17rgb2gray_weightedPKhjjPh+0x34>
  12:	7846      	ldrb	r6, [r0, #1]
  14:	3901      	subs	r1, #1
  16:	7805      	ldrb	r5, [r0, #0]
  18:	7882      	ldrb	r2, [r0, #2]
  1a:	3003      	adds	r0, #3
  1c:	fb06 f60c 	mul.w	r6, r6, ip
  20:	fb15 6504 	smlabb	r5, r5, r4, r6
  24:	ebc2 02c2 	rsb	r2, r2, r2, lsl #3
  28:	eb05 0282 	add.w	r2, r5, r2, lsl #2
  2c:	0a12      	lsrs	r2, r2, #8
  2e:	f803 2b01 	strb.w	r2, [r3], #1
  32:	e7ed      	b.n	10 <_Z17rgb2gray_weightedPKhjjPh+0x10>
  34:	f85d bb04 	ldr.w	fp, [sp], #4
  38:	bdf0      	pop	{r4, r5, r6, r7, pc}
```

`add_definitions(-O3)`优化时，对应的反汇编：
```asm
00000000 <_Z17rgb2gray_weightedPKhjjPh>:
   0:	b5b0      	push	{r4, r5, r7, lr}
   2:	af02      	add	r7, sp, #8
   4:	fb02 fc01 	mul.w	ip, r2, r1
   8:	f1bc 0f00 	cmp.w	ip, #0
   c:	d056      	beq.n	bc <_Z17rgb2gray_weightedPKhjjPh+0xbc>
   e:	f1bc 0f0f 	cmp.w	ip, #15
  12:	d936      	bls.n	82 <_Z17rgb2gray_weightedPKhjjPh+0x82>
  14:	eb0c 014c 	add.w	r1, ip, ip, lsl #1
  18:	4401      	add	r1, r0
  1a:	4299      	cmp	r1, r3
  1c:	bf84      	itt	hi
  1e:	eb03 010c 	addhi.w	r1, r3, ip
  22:	4281      	cmphi	r1, r0
  24:	d82d      	bhi.n	82 <_Z17rgb2gray_weightedPKhjjPh+0x82>
  26:	f02c 0e0f 	bic.w	lr, ip, #15
  2a:	ffc1 0e17 	vmov.i8	d16, #151	; 0x97
  2e:	efc1 1e1c 	vmov.i8	d17, #28	; 0x1c
  32:	eb03 020e 	add.w	r2, r3, lr
  36:	eb0e 014e 	add.w	r1, lr, lr, lsl #1
  3a:	efc4 2e1d 	vmov.i8	d18, #77	; 0x4d
  3e:	4401      	add	r1, r0
  40:	4674      	mov	r4, lr
  42:	f960 450d 	vld3.8	{d20,d22,d24}, [r0]!
  46:	3c10      	subs	r4, #16
  48:	f960 550d 	vld3.8	{d21,d23,d25}, [r0]!
  4c:	ffc6 eca0 	vmull.u8	q15, d22, d16
  50:	ff88 2ca1 	vmull.u8	q1, d24, d17
  54:	ffc7 cca0 	vmull.u8	q14, d23, d16
  58:	ff89 0ca1 	vmull.u8	q0, d25, d17
  5c:	ffc4 e8a2 	vmlal.u8	q15, d20, d18
  60:	ef69 31b9 	vorr	d19, d25, d25
  64:	ffc5 c8a2 	vmlal.u8	q14, d21, d18
  68:	ef68 31b8 	vorr	d19, d24, d24
  6c:	efcc 5480 	vaddhn.i16	d21, q14, q0
  70:	efce 4482 	vaddhn.i16	d20, q15, q1
  74:	f943 4a0d 	vst1.8	{d20-d21}, [r3]!
  78:	d1e3      	bne.n	42 <_Z17rgb2gray_weightedPKhjjPh+0x42>
  7a:	45f4      	cmp	ip, lr
  7c:	bf08      	it	eq
  7e:	bdb0      	popeq	{r4, r5, r7, pc}
  80:	e003      	b.n	8a <_Z17rgb2gray_weightedPKhjjPh+0x8a>
  82:	f04f 0e00 	mov.w	lr, #0
  86:	4601      	mov	r1, r0
  88:	461a      	mov	r2, r3
  8a:	ebac 000e 	sub.w	r0, ip, lr
  8e:	f04f 0c97 	mov.w	ip, #151	; 0x97
  92:	f04f 0e4d 	mov.w	lr, #77	; 0x4d
  96:	784c      	ldrb	r4, [r1, #1]
  98:	3801      	subs	r0, #1
  9a:	780b      	ldrb	r3, [r1, #0]
  9c:	788d      	ldrb	r5, [r1, #2]
  9e:	f101 0103 	add.w	r1, r1, #3
  a2:	fb04 f40c 	mul.w	r4, r4, ip
  a6:	fb13 430e 	smlabb	r3, r3, lr, r4
  aa:	ebc5 04c5 	rsb	r4, r5, r5, lsl #3
  ae:	eb03 0384 	add.w	r3, r3, r4, lsl #2
  b2:	ea4f 2313 	mov.w	r3, r3, lsr #8
  b6:	f802 3b01 	strb.w	r3, [r2], #1
  ba:	d1ec      	bne.n	96 <_Z17rgb2gray_weightedPKhjjPh+0x96>
  bc:	bdb0      	pop	{r4, r5, r7, pc}
```

区别：除了明显的更长的汇编之外，还有向量指令：
```
  4c:	ffc6 eca0 	vmull.u8	q15, d22, d16
  50:	ff88 2ca1 	vmull.u8	q1, d24, d17
  54:	ffc7 cca0 	vmull.u8	q14, d23, d16
  58:	ff89 0ca1 	vmull.u8	q0, d25, d17
  5c:	ffc4 e8a2 	vmlal.u8	q15, d20, d18
  60:	ef69 31b9 	vorr	d19, d25, d25
  64:	ffc5 c8a2 	vmlal.u8	q14, d21, d18
  68:	ef68 31b8 	vorr	d19, d24, d24
  6c:	efcc 5480 	vaddhn.i16	d21, q14, q0
  70:	efce 4482 	vaddhn.i16	d20, q15, q1
  74:	f943 4a0d 	vst1.8	{d20-d21}, [r3]!
  78:	d1e3      	bne.n	42 <_Z17rgb2gray_weightedPKhjjPh+0x42>
```


## 怎样检查当前优化等级？

### 打印`CMAKE_CXX_FLAGS`？
除非是手动把`-O2`这样的优化级别放到`CMAKE_CXX_FLAGS`里，否则并不显示`-O2`。

### compile_commands.json?

设定`set(CMAKE_EXPORT_COMPILE_COMMANDS ON)`后，在compile_commands.json中可以看到。

对于Android平台：
- 当`CMAKE_BUILD_TYPE=Release`时，是`-Oz`
- 当没指定CMAKE_BUILD_TYPE、指定了`-O3`时，是`-O3`
- 当没指定CMAKE_BUILD_TYPE、也没指定类似`add_definitions(-O3)`的优化级别时，找不到`-O`开头的

缺点：需要自己去翻。。有一些不需要看的信息例如宏定义。


## 反汇编
```
set OBJDUMP64=%ANDROID_NDK%/toolchains/aarch64-linux-android-4.9/prebuilt/windows-x86_64/bin/aarch64-linux-android-objdump.exe

set OBJDUMP32=%ANDROID_NDK%/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64/bin/arm-linux-androideabi-objdump.exe

%OBJDUMP64% -d android-arm64\CMakeFiles\testbed.dir\test.cpp.o > android-arm64\CMakeFiles\testbed.dir\test.s

%OBJDUMP32% -d android-arm32\CMakeFiles\testbed.dir\test.cpp.o > android-arm32\CMakeFiles\testbed.dir\test.s
```