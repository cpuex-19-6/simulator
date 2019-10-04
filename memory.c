#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "option.h"
#include "memory.h"

void mem_init(MEMORY *mem){	
	mem->instr = calloc(INSTR_MEM_SIZE, sizeof(uint32_t));
	mem->data = calloc(DATA_MEM_SIZE, sizeof(uint8_t));
	if(mem->instr == NULL)printf("NULL OFF\n");
	memset(mem->instr, 0, sizeof(uint32_t)*INSTR_MEM_SIZE);
	memset(mem->data, 0, sizeof(uint8_t)*DATA_MEM_SIZE);
}

void mem_set(MEMORY *mem, OPTION option){
	if(option.fname_instr == NULL){
		perror("No instruction file");
		exit(EXIT_FAILURE);
	}
	else{
		if(option.ftype_instr == BIN){
			load_instr(mem, option.fname_instr);
		}
		else{ //TXT
			load_instr_txt(mem, option.fname_instr);
		}
	}

	if(option.fname_data != NULL){
		if(option.ftype_data == BIN){
			load_data(mem, option.fname_data);
		}
		else{ //TXT
			load_data_txt(mem, option.fname_data);
		}
	}
}

void mem_free(MEMORY *mem){
	free(mem->instr);
	free(mem->data);
}

void load_instr(MEMORY *mem, char *filename){
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL){
		perror("failed to open file: bin");
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

void load_instr_txt(MEMORY *mem, char *filename){
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		perror("failed to open file: txt");
		exit(EXIT_FAILURE);
	}
	
	char tmp[INSTR_MEM_SIZE];
	int n = 0;
	
	if((n = fread(tmp, sizeof(char), INSTR_MEM_SIZE * 32, fp)) == 0){ //read and load txt data
		perror("failed to load txt instructions");
		exit(EXIT_FAILURE);
	}
	c2b((uint8_t *)mem->instr, tmp, n); //convert txt data to binary
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

void load_data_txt(MEMORY *mem, char *filename){
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		perror("failed to open file");
		exit(EXIT_FAILURE);
	}
	
	char tmp[DATA_MEM_SIZE];
	int n = 0;
	
	if((n = fread(tmp, sizeof(char), DATA_MEM_SIZE * 8, fp)) == 0){
		perror("failed to load txt data");
		exit(EXIT_FAILURE);
	}
	c2b(mem->data, tmp, n);
	fclose(fp);
}
