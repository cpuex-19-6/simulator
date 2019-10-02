#ifndef CPU_INCLUDE
#define CPU_INCLUDE

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef int32_t REG; //register
#define REG_N 32

typedef struct cpu_t {
	REG x[REG_N];
	unsigned int pc;
} CPU;

void cpu_init(CPU *cpu);

#endif
