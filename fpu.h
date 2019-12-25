#ifndef FPU_INCLUDE
#define FPU_INCLUDE

#include "struct.h"
#include <bitset>

using namespace std;

typedef union {
	float f;
	uint32_t i;
} uni ;

void exec_FLW(INSTR instr, CPU *cpu, MEMORY *mem);
void exec_FSW(INSTR instr, CPU *cpu, MEMORY *mem);
void exec_FLA(INSTR instr, CPU *cpu, MEMORY *mem);
void exec_FCB(INSTR instr, CPU *cpu, MEMORY *mem);

void decode_FLW(uint32_t instr, INSTR *imp);
void decode_FSW(uint32_t instr, INSTR *imp);
void decode_FLA(uint32_t instr, INSTR *imp);
void decode_FCB(uint32_t instr, INSTR *imp);

void mnemonic_FLA(INSTR instr, ASSEM *assem);
void mnemonic_FCB(INSTR instr, ASSEM *assem);

FREG fmul(FREG, FREG);
FREG fdiv(FREG, FREG);
FREG fsqr(FREG);

#endif
