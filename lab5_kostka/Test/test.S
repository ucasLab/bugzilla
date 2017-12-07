/*
 *
 *	Gorzkowski i Kazimierczak
 *
 */


	.text
	.global start
	.type    start, %function
	.type 	 gcd, 	%function
start:


	//czwarte zadanie
zad_4:
	mov r0, #11
	mov r1, #7
	bl gcd
	mov r10, r0


	//trzecie zadanie
zad_3:
	mov r3, #10
	mov r0, #1
	mov r1, #1
loop3:
	mov r2, r1
	mov r1, r0
	add	r0, r1, r2
	sub r3, #1
	cmp r3, #0
	bne loop3


	//drugie zadanie
zad_2:
	ldr r1, =tab_a
	ldr r2, =tab_b
	sub r0, r2, r1
	asr	r0, #2
	ldr r4, =c
	ldr r4, [r4]
	mov r3, #0

loop2:
	add r3, #1
	ldr r5, [r2], #4
	add r5, r4
	str r5, [r1], #4
	cmp r3, r0
	bne loop2


	// pierwsze zadanie
zad_1:
	ldr r0, =x
	ldr	r0, [r0]
	ldr r1, =y
	ldr r1, [r1]
	add r2, r0, r1
	sub r3, r0, r1
	mul r0, r2, r2
	mul r1, r2, r3
	add r0, r1


stop: b stop


gcd:
	cmp r0, r1
	beq label4_ret
	bmi substraction2
	sub r0, r1
	b gcd
substraction2:
	sub r1, r0
	b gcd
label4_ret:
	bx lr


	.data
result: .word 0 /* a word location */
x:		.word	7
y:		.word	3
tab_a:	.word	1, 2, 3, 4, 5, 6, 7, 8, 9, 10
tab_b: 	.word	11, 12, 13, 14, 15, 16, 17,18, 19, 20
c:		.word	100
