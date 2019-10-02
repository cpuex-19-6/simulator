#ifndef FUNCS_INCLUDE
#define FUNCS_INCLUDE

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "instruction.h"
#include "cpu.h"
#include "memory.h"

uint32_t downto(uint32_t u, int n, int m);

void exec_instr(uint32_t instr, CPU *cpu, MEMORY *mem);
uint32_t fetch(CPU *cpu, MEMORY *mem);

#endif
