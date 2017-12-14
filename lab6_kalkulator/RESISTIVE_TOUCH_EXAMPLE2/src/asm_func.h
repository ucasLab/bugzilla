/*
 * asm_func.h
 *
 * Created: 2017-12-14 11:58:40
 *  Author: student
 */ 


#ifndef ASM_FUNC_H_
#define ASM_FUNC_H_


int asm_add(int i, int j)
{
	int res = 0;
	__asm ("ADD %[result], %[input_i], %[input_j]"
	: [result] "=r" (res)
	: [input_i] "r" (i), [input_j] "r" (j)
	);
	return res;
}

int asm_sub(int i, int j)
{
	int res = 0;
	__asm ("SUB %[result], %[input_i], %[input_j]"
	: [result] "=r" (res)
	: [input_i] "r" (i), [input_j] "r" (j)
	);
	return res;
}

int asm_mul(int i, int j)
{
	int res = 0;
	__asm ("MUL %[result], %[input_i], %[input_j]"
	: [result] "=r" (res)
	: [input_i] "r" (i), [input_j] "r" (j)
	);
	return res;
}

int asm_div(int i, int j)
{
	int res = 0;
	__asm ("SDIV %[result], %[input_i], %[input_j]"
	: [result] "=r" (res)
	: [input_i] "r" (i), [input_j] "r" (j)
	);
	return res;
}


#endif /* ASM_FUNC_H_ */