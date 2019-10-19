#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#include "functions.h"
#include "option.h"
#include "memory.h"

void mem_init(MEMORY *mem){	
	mem->instr = calloc(INSTR_MEM_SIZE, sizeof(uint8_t));
	mem->data = calloc(DATA_MEM_SIZE, sizeof(uint8_t));
	memset(mem->instr, 0, sizeof(uint8_t)*INSTR_MEM_SIZE);
	memset(mem->data, 0, sizeof(uint8_t)*DATA_MEM_SIZE);
}

void mem_set(MEMORY *mem, OPTION option){
	if(option.fname_instr == NULL){
		perror("No instruction file");
		exit(EXIT_FAILURE);
	}
	else{
		printf("using \"%s\" as instruction file\n", option.fname_instr);
		if(option.ftype_instr == BIN){
			load_instr(mem, option.fname_instr);
		}
		else{ //TXT
			load_instr_txt(mem, option.fname_instr);
		}
	}

	/*if(option.fname_data != NULL){
		printf("using \"%s\" as data file\n", option.fname_data);
		if(option.ftype_data == BIN){
			load_data(mem, option.fname_data);
		}
		else{ //TXT
			load_data_txt(mem, option.fname_data);
		}
	}*/
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

	if ((n = fread(mem->instr, sizeof(uint8_t), INSTR_MEM_SIZE, fp)) == 0){ 
	/*サイズとかあってる?ファイルサイズを取得するべき？*/
		perror("failed to load instructions to memory");
		exit(EXIT_FAILURE);
	}
	printf("loaded size : %d byte\n", n);
	
	fclose(fp);
}

void load_instr_txt(MEMORY *mem, char *filename){
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		perror("failed to open file: txt");
		exit(EXIT_FAILURE); }
	
	char *tmp;
	tmp = malloc(INSTR_MEM_SIZE*8);
	int n = 0;
	
	if((n = fread(tmp, sizeof(char), INSTR_MEM_SIZE * 8, fp)) == 0){ //read and load txt data
		perror("failed to load txt instructions");
		exit(EXIT_FAILURE);
	}
	c2b_8(mem->instr, tmp, n); //convert txt data to binary
	//c2b_32(mem->instr, tmp, n); //convert txt data to binary
	printf("loaded size : %d byte\n", n);

	free(tmp);
	fclose(fp);
}

void load_data(MEMORY *mem, char *filename){
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL){
		perror("failed to open file");
		exit(EXIT_FAILURE);
	}	
	int n = 0;

	if ((n = fread(mem->data, sizeof(uint8_t), DATA_MEM_SIZE, fp)) == 0){ 
	/*サイズとかあってる?ファイルサイズを取得するべき？*/
		perror("failed to load data to memory");
		exit(EXIT_FAILURE);
	}
	printf("loaded size : %d byte\n", n);
	
	fclose(fp);
}

void load_data_txt(MEMORY *mem, char *filename){
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		perror("failed to open file");
		exit(EXIT_FAILURE);
	}
	
	char *tmp;
	tmp = malloc(DATA_MEM_SIZE*8);
	int n = 0;
	
	if((n = fread(tmp, sizeof(char), DATA_MEM_SIZE * 8, fp)) == 0){
		perror("failed to load txt data");
		exit(EXIT_FAILURE);
	}
	printf("loaded size : %d byte\n", n);
	c2b_8(mem->data, tmp, n);
	fclose(fp);
}

void print_mem_sub(MEM_PRINT_SUB mp, MEMORY mem){
	switch(mp.type){
		case Int:{
			int content;
			int len;
			switch(mp.size){
				case Byte:{
					int8_t d;
					endian_wrapper(&d, (void *)(mem.data + mp.addr), sizeof(int8_t));
					content = d;
					len = sizeof(int8_t);
					break;}
				case Half:{
				       int16_t d;
					endian_wrapper(&d, (void *)(mem.data + mp.addr), sizeof(int16_t));
					content = d;
					len = sizeof(int16_t);
					break;}
				default:{ //Word
					int32_t d;
					endian_wrapper(&d, (void *)(mem.data + mp.addr), sizeof(int32_t));
					content = d;
					len = sizeof(int32_t);
					}
			}
			printf("mem[%d-%d]: (int%d) %d", mp.addr, mp.addr + len-1, len*8, content);
			break;}
		case Uint:{
			unsigned int content;
			int len;
			switch(mp.size){
				case Byte:{
					uint8_t d;
					endian_wrapper(&d, mem.data + mp.addr, sizeof(uint8_t));
					content = d;
					len = sizeof(uint8_t);
					break;}
				case Half:{
				        uint16_t d;
					endian_wrapper(&d, mem.data + mp.addr, sizeof(uint16_t));
					content = d;
					len = sizeof(uint16_t);
					break;}
				default:{ //Word
					uint32_t d;
					endian_wrapper(&d, mem.data + mp.addr, sizeof(uint32_t));
					content = d;
					len = sizeof(uint32_t);
					}
			}
			printf("mem[%u-%d]: (uint%d) %u", mp.addr, mp.addr + len-1, len*8, content);
			break;}
		default:{ //Float
			float content;
			int len = sizeof(float);
			endian_wrapper(&content, mem.data + mp.addr, sizeof(float));
			printf("mem[%u-%d]: (float) %f", mp.addr, mp.addr + len-1, content);
			}
	}
}

void print_mem(MEM_PRINT mem_print, MEMORY mem){
	for(int i = 0; i < mem_print.num; i++){
		print_mem_sub(mem_print.mp[i], mem);
		putchar('\n');
	}
	putchar('\n');
}
