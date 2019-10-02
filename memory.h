#ifndef MEMORY_INCLUDE 
#define MEMORY_INCLUDE 

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define INSTR_MEM_SIZE 4194304
#define DATA_MEM_SIZE 4194304

typedef struct memory_t {
	uint32_t *instr;
	uint8_t *data;
} MEMORY;

void mem_init(MEMORY *mem);
void mem_free(MEMORY *mem);

void load_instr(MEMORY *mem, char *filename); //load instruction from file to memory
void load_data(MEMORY *mem, char *filename); //load data from file to memory
#endif
