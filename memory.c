#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#include "struct.h"
#include "functions.h"
#include "memory.h"
#include "decode.h"

void mem_init(MEMORY *mem){	
	mem->instr = calloc(INSTR_MEM_SIZE, sizeof(INSTR));
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

		int n = 0;
		uint32_t *tmp;
		tmp = calloc(INSTR_MEM_SIZE/4, sizeof(uint32_t));

		if(option.ftype_instr == BIN){
			n = load_instr(tmp, option.fname_instr); //loaded instructions
			decode(tmp, n, mem);
		}
		else{ //TXT
			n = load_instr_txt(tmp, option.fname_instr); //loaded bits of instructions
			n = n/32;
			decode(tmp, n, mem);
		}
		printf("loaded instructions: %d\n", n);
	}
}

void mem_free(MEMORY *mem){
	free(mem->instr);
	free(mem->data);
}

int load_instr(uint32_t *instr, char *filename){
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL){
		perror("failed to open file: bin");
		exit(EXIT_FAILURE);
	}	
	int n = 0;

	if ((n = fread(instr, sizeof(uint32_t), INSTR_MEM_SIZE/4, fp)) == 0){ 
	/*サイズとかあってる?ファイルサイズを取得するべき？*/
		perror("failed to load instructions to memory");
		exit(EXIT_FAILURE);
	}
	printf("loaded size : %d byte\n", n*4);
	
	fclose(fp);

	return n; //return loaded instruciton bytes
}

int load_instr_txt(uint32_t *instr, char *filename){
	FILE *fp;
	if((fp = fopen(filename, "r")) == NULL){
		perror("failed to open file: txt");
		exit(EXIT_FAILURE); }
	
	char *tmp;
	tmp = malloc(INSTR_MEM_SIZE*9);
	int n = 0;
	
	if((n = fread(tmp, sizeof(char), INSTR_MEM_SIZE * 9, fp)) == 0){ //read and load txt data, テキストファイルを読むのでちょっと多めに読んでる。
		perror("failed to load txt instructions");
		exit(EXIT_FAILURE);
	}
	printf("loaded size : %d byte\n", n);
	int ans = c2b_32(instr, tmp, n); //convert txt data to binary

	free(tmp);
	fclose(fp);

	return ans; //return loaded instruciton bytes
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

void address_check(int address){
	if(address < 0 || address >= DATA_MEM_SIZE){
		printf("invalid memory access: address %d\n", address);
		exit(EXIT_FAILURE);
	}
}

void address_check_instr(int pc, int oldpc){
	int address = pc /4;
	if(address < 0 || address >= INSTR_MEM_SIZE){
		printf("invalid memory access: pc %d: address %d: oldpc %d\n", pc, address, oldpc);
		exit(EXIT_FAILURE);
	}
}
