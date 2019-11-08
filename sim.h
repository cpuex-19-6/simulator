#ifndef FUNCS_INCLUDE
#define FUNCS_INCLUDE

#include <stdint.h>

#include "struct.h"

void exec_instr(INSTR instr, CPU *cpu, MEMORY *mem, IO *io, STATE *state);
INSTR fetch(CPU *cpu, MEMORY *mem);

int run_to_the_end(CPU *cpu, MEMORY *mem, IO *io, OPTION option, STATE *state);
int step(CPU *cpu, MEMORY *mem, IO *io, OPTION *option, STATE *state);

#endif
