	.text
	.global start
	.type    start, %function
start:
	MOV R1, #200
	MOV R2, #300
	MOV R3, #0
	MOV R4, #2
loop:
	ADD R3, #1
	LDR R5, [R2]
	ADD R5, R4
	STR R5, [R1]
	ADD R1, #1
	ADD R2, #1
	CMP R3, #10
	BLT loop

stop: b stop
	.data
result: .word 0 /* a word location */
