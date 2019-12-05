#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "struct.h"
#include "cpu.h"
#include "memory.h"

void cpu_init(CPU *cpu){
	memset(cpu->x, 0, sizeof(REG)*REG_N);
	memset(cpu->f, 0, sizeof(FREG)*FREG_N);
	cpu->x[2] = DATA_MEM_SIZE;
	cpu->x[3] = 0;
	cpu->pc = 0;
}
