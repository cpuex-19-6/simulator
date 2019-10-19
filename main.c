#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"
#include "sim.h"
#include "option.h"
#include "disassemble.h"
#include "io.h"

int main(int argc, char *argv[]){
	OPTION option;
	CPU cpu;
	MEMORY mem;
	IO io;

	option_init(&option);
	cpu_init(&cpu);
	mem_init(&mem);
	io_init(&io);

	option_set(argc - 1, argv + 1, &option);
	mem_set(&mem, option);
	io_set(&io, option);

	char s[COM_LEN];
	int quit = 0;

	while(quit == 0){
		command_parser(s, &option);

		switch(option.mode){
			case RUN:
				if(run_to_the_end(&cpu, &mem, &io, option))quit = 1;
				break;
			case STEP:
				if(step(&cpu, &mem, &io, &option))quit = 1;
				break;
			default: //case QUIT:
				quit = 1;
		}
	}

	if(option.mode != QUIT){
		printf("simulation ended: final state\n\n");
		for(int i = 0; i < 32; i++){
			if(option.reg16 & 1)printf("x[%2d]: 0x%x\n", i, cpu.x[i]);
			else printf("x[%2d]: %d\n", i, cpu.x[i]);
		}
		putchar('\n');
		for(int i = 0; i < 32; i++){
			printf("f[%2d]:	%f\n", i, cpu.f[i]); 
		}
		putchar('\n');
		print_mem(option.mem_print, mem);
	}

	option_free(&option);
	mem_free(&mem);
	io_close(&io);
	return 0;
}
