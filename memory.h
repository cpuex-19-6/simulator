#ifndef MEMORY_INCLUDE 
#define MEMORY_INCLUDE 

#include <stdint.h>

#include "struct.h"

#define INSTR_MEM_SIZE 131072 //number of 8 bit cells :4byte * 1 << 15
#define DATA_MEM_SIZE 2*2097152 //number of 8 bit cells :1byte * (1 << 21)*2

void mem_init(MEMORY *mem);
void mem_set(MEMORY *mem, OPTION option);
void mem_free(MEMORY *mem);

int load_instr(uint32_t *mem, char *filename); //load instruction from binary file to memory
int load_instr_txt(uint32_t *mem, char *filename); //load instruction from txt file to memory

void print_mem(MEM_PRINT mem_print, MEMORY mem);

void address_check(int adderss);
void address_check_instr(int pc, int oldpc);

#endif
