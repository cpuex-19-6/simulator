#ifndef IO_INCLUDE
#define IO_INCLUDE

#include "cpu.h"
#include "memory.h"
#include "option.h"


typedef struct io_t {
	FTYPE ftype_in;
	FILE *input;
	FTYPE ftype_out;
	FILE *output;
}IO;

void io_init(IO *io);
void io_set(IO *io, OPTION option);
void io_close(IO *io);

void exec_IN(uint32_t instr, CPU *cpu, MEMORY *mem, IO *io);
void exec_OUT(uint32_t instr, CPU *cpu, MEMORY *mem, IO *io);

//void mnemonic_IN(uint32_t instr, ASSEM *assem);
//void mnemonic_OUT(uint32_t instr, ASSEM *assem);

#endif
