#include <stdio.h>

#include "analyse.h"

void state_init(STATE *state){
	state->instr_num = 0;
}

void print_state(STATE state){
	printf("Analysed data\n");
	putchar('\n');
	printf("total executed instructions: %llu\n", state.instr_num);
	putchar('\n');
}
