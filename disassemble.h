#ifndef DISASSEMBLE_INCLUDE
#define DISASSEMBLE_INCLUDE

#include "sim.h"

typedef struct assem_t {
	INSTR_TYPE itype;
	char reg[3];
	int32_t rd;
	int32_t rs1;
	int32_t rs2;
	int32_t imm;
	char mnemonic[10];
	char rm[3];
}ASSEM;

//void assem_init(ASSEM *assem);
void disassem_instr(uint32_t instr, ASSEM *assem);
void print_assembly(ASSEM assem);

#endif
