#ifndef ANALYSE_INCLUDE
#define ANALYSE_INCLUDE

#include "struct.h"

void state_init(STATE *state);
void state_set(STATE *state, OPTION option);
void state_free(STATE *state);

void print_state(STATE state);

void tag_check(TAG *tag, int pc);

void state_update(STATE *state, CPU cpu, INSTR instr);

#endif
