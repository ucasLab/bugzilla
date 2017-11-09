	.text
	.global start
	.type    start, %function
start:
	mov r0, #1 // r0 = 1 == x
	mov R1, #2 // r1 = 2 == y
	add R2, R1, R0 // r2 = x + y
	mul R2, R0, R2 // r2 = x(x + y)
	mov R3, #2 // r3 = 2
	mul R2, R2, R3 // r2 = 2x(x+y)
	mov R0, R2 // r0 = r2
	ldr R3, =result // r3 = &result
	str R0, [R3] /* result = r0 */
	ldr R3, [R3]
stop: b stop
	.data
result: .word 0 /* a word location */
