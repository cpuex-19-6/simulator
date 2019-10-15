#ifndef OPTION_INCLUDE
#define OPTION_INCLUDE

#include "cpu.h"

#define COM_LEN 128
#define BP_NUM 30
#define MP_NUM 30

typedef enum ftype_t{
	BIN,
	TXT,
}FTYPE;

typedef enum mode_t{
	NONE,
	RUN,
	STEP,
	QUIT,
}MODE;

typedef enum mem_data_t {
	Int, //int
	Uint, //unsigned int
	Float, //float
}MEM_DATA_TYPE;

typedef enum mem_data_size_t{
	Byte, //byte
	Half, //half
	Word, //word
}MEM_DATA_SIZE;

typedef struct mem_print_sub_t {
	MEM_DATA_TYPE type;
	MEM_DATA_SIZE size;
	uint32_t addr;
}MEM_PRINT_SUB;

typedef struct mem_print_t{
	MEM_PRINT_SUB *mp;
	int num;
}MEM_PRINT;

typedef struct break_point_t{
	int *bp;
	int num;
}BREAKPOINT;

typedef struct option_t {
	FTYPE ftype_instr;
	FTYPE ftype_data;
	char *fname_instr;
	char *fname_data;
	MODE mode;
	uint32_t reg;
	uint32_t reg16;
	uint32_t freg;
	BREAKPOINT breakpoint;
	int to_the_end;
	MEM_PRINT mem_print;
	int step_n;
}OPTION;

void c2b_8(uint8_t *dest, char *source, size_t size);
void c2b_32(uint32_t *dest, char *source, size_t size);
void option_init(OPTION *option);
void option_set(int argn, char **arg, OPTION *option);
void option_free(OPTION *option);

void c2b(uint8_t *dest, char *source, size_t size);

void command_parser(char *s, OPTION *option);
void print_reg(OPTION option, CPU cpu);
void print_freg(OPTION option, CPU cpu);
int bp_check(CPU cpu, BREAKPOINT breakpoint);

#endif
