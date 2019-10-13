#ifndef FPU_INCLUDE
#define FPU_INCLUDE

#include "cpu.h"
#include "memory.h"
#include "disassemble.h"

void exec_FLW(uint32_t instr, CPU *cpu, MEMORY *mem);
void exec_FSW(uint32_t instr, CPU *cpu, MEMORY *mem);
void exec_FLA(uint32_t instr, CPU *cpu, MEMORY *mem);

void mnemonic_FLA(uint32_t instr, ASSEM *assem);

#endif
