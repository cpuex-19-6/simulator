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
#include "io.h"

typedef enum instr_type_t{
	R,
	R_sub,
	R_sub_sub,
	R_sub_sub_sub,
	I, 
	S, 
	B, 
	U, 
	J,
}INSTR_TYPE;

int32_t immediate(uint32_t instr, INSTR_TYPE ip);

void exec_instr(uint32_t instr, CPU *cpu, MEMORY *mem, IO *io);
uint32_t fetch(CPU *cpu, MEMORY *mem);

int run_to_the_end(CPU *cpu, MEMORY *mem, IO *io, OPTION option);
int step(CPU *cpu, MEMORY *mem, IO *io, OPTION *option);

#endif
