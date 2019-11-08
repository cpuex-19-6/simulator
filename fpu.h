#ifndef FPU_INCLUDE
#define FPU_INCLUDE

#include "struct.h"

void exec_FLW(INSTR instr, CPU *cpu, MEMORY *mem);
void exec_FSW(INSTR instr, CPU *cpu, MEMORY *mem);
void exec_FLA(INSTR instr, CPU *cpu, MEMORY *mem);

void decode_FLW(uint32_t instr, INSTR *imp);
void decode_FSW(uint32_t instr, INSTR *imp);
void decode_FLA(uint32_t instr, INSTR *imp);

void mnemonic_FLA(INSTR instr, ASSEM *assem);

#endif
