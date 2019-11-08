#ifndef IO_INCLUDE
#define IO_INCLUDE

#include "struct.h"

void io_init(IO *io);
void io_set(IO *io, OPTION option);
void io_close(IO *io);

void exec_IN(INSTR instr, CPU *cpu, MEMORY *mem, IO *io);
void exec_OUT(INSTR instr, CPU *cpu, MEMORY *mem, IO *io);

void decode_IN(uint32_t instr, INSTR *imp);
void decode_OUT(uint32_t instr, INSTR *imp);

//void mnemonic_IN(uint32_t instr, ASSEM *assem);
//void mnemonic_OUT(uint32_t instr, ASSEM *assem);

#endif
