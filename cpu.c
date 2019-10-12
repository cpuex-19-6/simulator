#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cpu.h"

void cpu_init(CPU *cpu){
	memset(cpu->x, 0, sizeof(REG)*REG_N);
	memset(cpu->f, 0, sizeof(FREG)*FREG_N);
	cpu->pc = 0;
}
