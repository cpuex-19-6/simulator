#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "memory.h"

void mem_init(MEMORY *mem){	
	mem->instr = calloc(INSTR_MEM_SIZE, sizeof(uint32_t));
	mem->data = calloc(DATA_MEM_SIZE, sizeof(uint8_t));
	if(mem->instr == NULL)printf("NULL OFF\n");
	memset(mem->instr, 0, sizeof(uint32_t)*INSTR_MEM_SIZE);
	memset(mem->data, 0, sizeof(uint8_t)*DATA_MEM_SIZE);
}

void mem_free(MEMORY *mem){
	free(mem->instr);
	free(mem->data);
}

void load_instr(MEMORY *mem, char *filename){
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL){
		perror("failed to open file");
		exit(EXIT_FAILURE);
	}	
	int n = 0;

	if ((n = fread(mem->instr, sizeof(uint32_t), INSTR_MEM_SIZE, fp)) == 0){ 
	/*サイズとかあってる?ファイルサイズを取得するべき？*/
		perror("failed to load instructions to memory");
		exit(EXIT_FAILURE);
	}
	printf("n: %d\n", n);
	
	fclose(fp);
}

void load_data(MEMORY *mem, char *filename){
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL){
		perror("failed to open file");
		exit(EXIT_FAILURE);
	}	

	if (fread(mem->data, sizeof(uint8_t), DATA_MEM_SIZE, fp) == 0){ 
	/*サイズとかあってる?ファイルサイズを取得するべき？*/
		perror("failed to load data to memory");
		exit(EXIT_FAILURE);
	}
	
	fclose(fp);
}
