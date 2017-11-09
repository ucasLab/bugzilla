	.text
	.global start
	.type    start, %function
start:
	MOV R0, #0
	MOV R1, #5
	MOV R2, #2
gcd:
while:
	CMP R1, R2
	BEQ save
	CMP R1, R2
	BLE else
	SUB R1, R2
	B while
else:
	SUB R2, R1
	B while
save:
	STR R1, [R0]
stop: b stop
	.data
result: .word 0 /* a word location */
