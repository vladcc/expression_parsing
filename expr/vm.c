#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vm.h"

#define STACK_SZ 256
#define REGNUM   2
#define next_reg() (reg ^= 1)

int mode, reg, last_num;
bool work = true;
const char * regs[REGNUM] = {"r0", "r1"};

typedef struct vm_type {
	int regs[REGNUM], sp;
	int stack[STACK_SZ];
} vm_type;

vm_type vm;

void vm_push(int num);
void vm_pop(void);
void vm_add(void);
void vm_sub(void);
void vm_mult(void);
void vm_div(void);
//------------------------------------------------------------------------------

void vm_set_mode(int mod)
{
	if (mod < vmINTERPRET || mod > vmCOMPILE)
	{
		fprintf(stderr, "Err: unknown mode < %d >\n", mod);
		exit(EXIT_FAILURE);
	}
	
	mode = mod;
	return;
}
//------------------------------------------------------------------------------

void vm_load_num(int num)
{
	last_num = num;
	return;
}
//------------------------------------------------------------------------------

void vm_exec(int action)
{
	if (work)
	{
		switch (action)
		{
			case vmPUSH: vm_push(last_num); break;
			case vmPOP:  vm_pop();          break;
			case vmADD:  vm_add();          break;
			case vmSUB:  vm_sub();          break;
			case vmMULT: vm_mult();         break;
			case vmDIV:  vm_div();          break;
			default:
				fprintf(stderr, "Err: unknown action < %d >\n", action);
				exit(EXIT_FAILURE);
				break;
		}
	}
	return;
}
//------------------------------------------------------------------------------

void vm_push(int num)
{
	if (vmCOMPILE == mode)
		printf("push %d\n", num);
	else if (vmINTERPRET == mode)
	{
		if (vm.sp >= STACK_SZ)
		{
			fprintf(stderr, "Err: expression too long\n");
			exit(EXIT_FAILURE);
		}
		vm.stack[vm.sp++] = num;
	}
	return;
}
//------------------------------------------------------------------------------

void vm_pop(void)
{
	if (vmCOMPILE == mode)
		printf("pop %s\n", regs[next_reg()]);
	else if (vmINTERPRET == mode)
	{
		if (0 == vm.sp)
		{
			fprintf(stderr, "Err: attempt to pop an empty stack\n");
			exit(EXIT_FAILURE);
		} 
		vm.regs[next_reg()] = vm.stack[--vm.sp];
	}
	return;
}
//------------------------------------------------------------------------------

void vm_add(void)
{
	vm_pop();
	vm_pop();
	
	if (vmCOMPILE == mode)
		printf("add %s %s\n", regs[0], regs[1]), printf("push %s\n", regs[0]);
	else if (vmINTERPRET == mode)
	{
		int tmp = vm.regs[0] + vm.regs[1];
		printf("%d %c %d = %d\n", vm.regs[0], '+', vm.regs[1], tmp);
		vm_push(tmp);
	}	
	return;	
}
//------------------------------------------------------------------------------

void vm_sub(void)
{
	vm_pop();
	vm_pop();

	if (vmCOMPILE == mode)
		printf("sub %s %s\n", regs[0], regs[1]), printf("push %s\n", regs[0]);
	else if (vmINTERPRET == mode)
	{
		int tmp = vm.regs[0] - vm.regs[1];
		printf("%d %c %d = %d\n", vm.regs[0], '-', vm.regs[1], tmp);
		vm_push(tmp);
	}	
	return;	
}
//------------------------------------------------------------------------------

void vm_mult(void)
{
	vm_pop();
	vm_pop();
	
	if (vmCOMPILE == mode)
		printf("mul %s %s\n", regs[0], regs[1]), printf("push %s\n", regs[0]);
	else if (vmINTERPRET == mode)
	{
		int tmp = vm.regs[0] * vm.regs[1];
		printf("%d %c %d = %d\n", vm.regs[0], '*', vm.regs[1], tmp);
		vm_push(tmp);
	}	
	return;	
}
//------------------------------------------------------------------------------

void vm_div(void)
{
	vm_pop();
	vm_pop();
	
	if (vmCOMPILE == mode)
		printf("div %s %s\n", regs[0], regs[1]), printf("push %s\n", regs[0]);
	else if (vmINTERPRET == mode)
	{
		int tmp = vm.regs[0] / vm.regs[1];
		printf("%d %c %d = %d\n", vm.regs[0], '/', vm.regs[1], tmp);
		vm_push(tmp);
	}	
	return;	
}
//------------------------------------------------------------------------------

void vm_stop(void)
{
	work = false;
	return;
}
//------------------------------------------------------------------------------

void vm_reset(void)
{
	work = true;
	vm.regs[0] = vm.regs[1] = vm.sp = 0;
	return;
}
//------------------------------------------------------------------------------
