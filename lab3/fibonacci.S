	.text
	.global start
	.type    start, %function
start:
	MOV R0, #0 // adres poczatkowy ciagu
	MOV R1, #0
	MOV R2, #1
fibo:
	MOV R3, #0
	ADC R3, R1
	ADC R3, R2
	BCS stop
	MOV R1, R2
	MOV R2, R3
	STR R3, [R0]
	ADD R0, #16
	B fibo

stop: b stop
	.data
result: .word 0 /* a word location */
