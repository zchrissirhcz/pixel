&define constant
.equ N,32768

&allocate array data, with sizeof N*4, staring address divisble by 4
.comm data,N*4,4

&declare function
.global main

&declare archtecture
.arch armv7-a


main: ldr r1,=N
	sub r1, r1, #1	@r1=N-1
	mov r5, #0	@i=0  &设定循环变量计数器的初值
	oloop:cmp r5, r1 @i==N-1?
	beq exito

	mov r3, #0 @j=0
	ldr r2,=data @r2=&data


