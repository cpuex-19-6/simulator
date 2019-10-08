#ifndef OPTION_INCLUDE
#define OPTION_INCLUDE

#include "cpu.h"

#define COM_LEN 128

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

typedef struct option_t {
	FTYPE ftype_instr;
	FTYPE ftype_data;
	char *fname_instr;
	char *fname_data;
	MODE mode;
	uint32_t reg;
}OPTION;

void c2b_8(uint8_t *dest, char *source, size_t size);
void c2b_32(uint32_t *dest, char *source, size_t size);
void option_init(OPTION *option);
void option_set(int argn, char **arg, OPTION *option);
void option_free(OPTION *option);

void c2b(uint8_t *dest, char *source, size_t size);

void command_parser(char *s, OPTION *option);
void print_reg(uint32_t reg, CPU cpu);

#endif
