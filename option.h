#ifndef OPTION_INCLUDE
#define OPTION_INCLUDE

#include "struct.h"

#define COM_LEN 128
#define BP_NUM 30
#define MP_NUM 30

void option_init(OPTION *option);
void option_set(int argn, char **arg, OPTION *option);
void option_free(OPTION *option);

int command_parser(char *s, OPTION *option);
int command_set(char *s, OPTION *option);

void print_reg(OPTION option, CPU cpu);
void print_freg(OPTION option, CPU cpu);
int bp_check(CPU cpu, BREAKPOINT breakpoint);

#endif
