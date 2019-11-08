#ifndef ANALYSE_INCLUDE
#define ANALYSE_INCLUDE

#include "struct.h"

void state_init(STATE *state);
//void state_free(STATE *state);

void print_state(STATE state);

void state_update(STATE *state, CPU cpu, int rd);

#endif
