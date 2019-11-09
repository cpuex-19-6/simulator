#ifndef DECODE_INCLUDE
#define DECODE_INCLUDE

#include "struct.h"

int32_t immediate(uint32_t instr, INSTR_TYPE ip);

void decode_instr(uint32_t instr, INSTR *imp);

void decode(uint32_t *instrs, int size, MEMORY *mem);

#endif
