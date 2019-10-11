#ifndef FUNCS_INCLUDE
#define FUNCS_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "instruction.h"
#include "cpu.h"
#include "memory.h"
#include "option.h"

typedef enum instr_type_t{
	R,
	I, 
	S, 
	B, 
	U, 
	J,
}INSTR_TYPE;

uint32_t downto(uint32_t u, int n, int m);

int32_t immediate(uint32_t instr, INSTR_TYPE ip);

void exec_instr(uint32_t instr, CPU *cpu, MEMORY *mem);
uint32_t fetch(CPU *cpu, MEMORY *mem);

int run_to_the_end(CPU *cpu, MEMORY *mem, OPTION option);
int step(CPU *cpu, MEMORY *mem, OPTION option);

#endif
