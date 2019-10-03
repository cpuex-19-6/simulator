#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"
#include "sim.h"
#include "option.h"

int main(int argc, char *argv[]){
	OPTION option;
	CPU cpu;
	MEMORY mem;

	option_init(&option);
	cpu_init(&cpu);
	mem_init(&mem);

	option_set(argc - 1, argv + 1, &option);
	mem_set(&mem, option);

	uint32_t instr = fetch(&cpu, &mem);

	while(instr != 0){
		exec_instr(instr, &cpu, &mem);
		instr = fetch(&cpu, &mem);
	}

	for(int i = 0; i < 32; i++){
		putchar('\n');
		printf("x[%2d]:	%d\n", i, cpu.x[i]); 
	}

	option_free(&option);
	mem_free(&mem);
	return 0;
}
