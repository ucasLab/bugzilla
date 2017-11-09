	.text
	.global start
	.type    start, %function
start:
	LDR R1, =tab1
	LDR R2, =tab2
	MOV R3, #0
	MOV R4, #2
loop:
	ADD R3, #1
	LDRB R5, [R2]
	ADD R5, R4
	STRB R5, [R1]
	ADD R1, #1
	ADD R2, #1
	CMP R3, #10
	BLT loop

stop: b stop
	.data
tab1: .byte 0,0,0,0,0,0,0,0,0,0 /* a word location */
tab2: .byte 1,2,3,4,5,6,7,8,9,10 /* a word location */
