#ifndef ANALYSE_INCLUDE
#define ANALYSE_INCLUDE

typedef struct state_t{
	long long unsigned int instr_num;
}STATE;

void state_init(STATE *state);
//void state_free(STATE *state);

void print_state(STATE state);

#endif
