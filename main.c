#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"
#include "sim.h"
#include "option.h"
#include "disassemble.h"

int main(int argc, char *argv[]){
	OPTION option;
	CPU cpu;
	MEMORY mem;

	option_init(&option);
	cpu_init(&cpu);
	mem_init(&mem);

	option_set(argc - 1, argv + 1, &option);
	mem_set(&mem, option);

	char s[COM_LEN];
//	uint32_t instr = fetch(&cpu, &mem);
	int quit = 0;

	while(quit == 0){
		command_parser(s, &option);

		switch(option.mode){
			case RUN:
				run_to_the_end(&cpu, &mem);
				quit = 1;
				break;
			case STEP:
				if(step(&cpu, &mem, option))quit = 1;
				break;
			default: //case QUIT:
				quit = 1;
		}
	}

	if(option.mode != QUIT){
		printf("simulation ended: final state\n");
		for(int i = 0; i < 32; i++){
			printf("x[%2d]:	%d\n", i, cpu.x[i]); 
		}
	}

	option_free(&option);
	mem_free(&mem);
	return 0;
}
