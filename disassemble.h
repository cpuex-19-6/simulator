#ifndef DISASSEMBLE_INCLUDE
#define DISASSEMBLE_INCLUDE

#include "struct.h"

//void assem_init(ASSEM *assem);
void disassem_instr(INSTR instr, ASSEM *assem);
void print_assembly(ASSEM assem);

#endif
