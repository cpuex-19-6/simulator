#include <stdio.h>

#include "analyse.h"
#include "memory.h"
#include "cpu.h"

void state_init(STATE *state){
	state->instr_num = 0;
	//state->heap_top = 0;
	state->heap_bottom = 0;
	state->stack_top = DATA_MEM_SIZE;
	//state->stack_bottom = DATA_MEM_SIZE;
}

void print_state(STATE state){
	printf("Analysed data\n");
	putchar('\n');
	printf("total executed instructions: %llu\n", state.instr_num);
	putchar('\n');
	printf("memory access area:\n");
	printf("heap: %d byte\n", state.heap_bottom + 1);
	printf("stack: %d byte\n", DATA_MEM_SIZE - state.stack_top);
	putchar('\n');
}

void state_update(STATE *state, CPU cpu, int rd){
	state->instr_num++;

	switch(rd){
		case 2:
			if(cpu.x[2] > state->stack_top)
				state->stack_top = cpu.x[2];
			break;
		case 3:
			if(cpu.x[3] < state->heap_bottom)
				state->heap_bottom = cpu.x[3];
			break;
		default:
			{}
	}
}
