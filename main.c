#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"
#include "sim.h"

int main(int argc, char *argv[]){
	CPU cpu;
	MEMORY mem;

	cpu_init(&cpu);
	mem_init(&mem);

	if(argc > 1){
		load_instr(&mem, argv[1]); //load instruction from file
		if(argc > 2){
			load_data(&mem, argv[2]); //load data from file
		}
	}
	else{
		perror("missing instruction file");
		exit(EXIT_FAILURE);
	}

	printf("mem %d \n", mem.instr[0]);
	uint32_t instr = fetch(&cpu, &mem);
	printf("instr %d \n", instr);

	while(instr != 0){
		exec_instr(instr, &cpu, &mem);
		instr = fetch(&cpu, &mem);
	}

	for(int i = 0; i < 32; i++){
		putchar('\n');
		printf("x[%2d]:	%d\n", i, cpu.x[i]); 
	}

	mem_free(&mem);
	return 0;
}
