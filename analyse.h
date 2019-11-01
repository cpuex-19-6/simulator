#ifndef ANALYSE_INCLUDE
#define ANALYSE_INCLUDE

#include "cpu.h"

typedef struct state_t{
	long long unsigned int instr_num;
	unsigned int heap_top;
	//unsigned int heap_bottom;
	//unsigned int stack_top;
	unsigned int stack_bottom;
}STATE;

void state_init(STATE *state);
//void state_free(STATE *state);

void print_state(STATE state);

void state_update(STATE *state, CPU cpu, int rd);

#endif
