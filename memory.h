#ifndef MEMORY_INCLUDE 
#define MEMORY_INCLUDE 

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "option.h"

#define INSTR_MEM_SIZE 131072 //number of 8 bit cells 4byte * 1 << 15
#define DATA_MEM_SIZE 2097152 //number of 8 bit cells 1byte * 1 << 21

typedef struct memory_t {
	uint8_t *instr;
	uint8_t *data;
} MEMORY;

void mem_init(MEMORY *mem);
void mem_set(MEMORY *mem, OPTION option);
void mem_free(MEMORY *mem);

void load_instr(MEMORY *mem, char *filename); //load instruction from binary file to memory
void load_data(MEMORY *mem, char *filename); //load data from binary file to memory

void load_instr_txt(MEMORY *mem, char *filename); //load instruction from txt file to memory
void load_data_txt(MEMORY *mem, char *filename); //load data from txt file to memory

void print_mem(MEM_PRINT mem_print, MEMORY mem);

void address_check(int adderss);

#endif
