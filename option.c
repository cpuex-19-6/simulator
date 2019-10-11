#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "option.h"
#include "disassemble.h"
#include "cpu.h"
#include "memory.h"

//テキストで表されたデータをバイナリに変換　ただし、8ビットずつ
void c2b_8(uint8_t *dest, char *source, size_t size){
	int count = 0;
	uint8_t tmp = 0;

	for(int i = 0; i < size; i++){
		switch(source[i]){
			case '0':
				count++;
				break;
			case '1':
				tmp = tmp | (1 << (8 - count));
				count++;
				break;
			default:
				break;
		}
		if(count == 8){
			*dest = tmp;
			dest++;
			count = 0;
			tmp = 0;
		}
	}	
}

void c2b_32(uint32_t *dest, char *source, size_t size){
	int count = 0;
	uint32_t tmp = 0;

	for(int i = 0; i < size; i++){
		switch(source[i]){
			case '0':
				count++;
				break;
			case '1':
				tmp = tmp | (1 << (31 - count));
				count++;
				break;
			default:
				break;
		}
		if(count == 32){
			*dest = tmp;
			dest++;
			count = 0;
			tmp = 0;
		}
	}	
}

void option_init(OPTION *option){
	option->ftype_instr = BIN;
	option->ftype_data = BIN;
	option->fname_instr = NULL;
	option->fname_data = NULL;
	option->mode = NONE;
	option->reg = 0;
	option->breakpoint.bp = NULL;
	option->breakpoint.num = 0;
	option->to_the_end = 1; //run to the end!!!!
	option->mem_print.mp = NULL;
	option->mem_print.num = 0;
	option->step_n = 0;
}

void option_set(int argn, char **arg, OPTION *option){
	int i = 0;
	
	//parsing command
	while(i < argn){
		if(arg[i][0] == '-'){ //option
			switch(arg[i][1]){
				case 't': //txt file
					option->ftype_instr = TXT;
					i++;
					printf("filename: %s\n", arg[i]);
					option->fname_instr = malloc(strlen(arg[i]));
					strcpy(option->fname_instr, arg[i]);
					printf("option filename: %s\n", option->fname_instr);
					break;
				case 'd': //bin data-file
					option->ftype_data = BIN;
					i++;
					option->fname_data = malloc(strlen(arg[i]));
					strcpy(option->fname_instr, arg[i]);
					break;
				case 'e': //txt data-file
					option->ftype_data = TXT;
					i++;
					option->fname_data = malloc(strlen(arg[i]));
					strcpy(option->fname_instr, arg[i]);
					break;
				default:
					perror("invalid option");
					exit(EXIT_FAILURE);
			}
		}
		else{ //instruction file is binary: default
			option->ftype_instr = BIN;
			option->fname_instr = malloc(strlen(arg[i]));
			strcpy(option->fname_instr, arg[i]);
		}
		i++; //increment arg
	}
}

void option_free(OPTION *option){
	free(option->fname_instr);
	free(option->fname_data);
	if(option->breakpoint.bp != NULL)
		free(option->breakpoint.bp);
	if(option->mem_print.mp != NULL)
		free(option->mem_print.mp);
}

char *read_space(char *s){
	while(*s == ' ')
		s++;
	return s;
}

void command_parser(char *s, OPTION *option){
	int b = 0;

	while(b == 0){

		printf("simulator:>");
		scanf("%s", s);	

		switch(s[0]){
			case 'r':{
				if(s[1] == 'e')option->to_the_end = 1;
				else option->to_the_end = 0;

				option->mode = RUN;
				b = 1;
				break;}
			case 's':{
				if(s[1] == 'n'){
					int d;
					scanf("%d", &d);
					option->step_n = d;
				}
				option->mode = STEP;
				b = 1;
				break;}
			case 'q':
				option->mode = QUIT;
				b = 1;
				break;
			case 'x':{
				int d;
				scanf("%d", &d);
				option->reg = option->reg | (1 << d);
				break;}
			case 'b':{
				if(option->breakpoint.bp == NULL)
					option->breakpoint.bp = malloc(BP_NUM*sizeof(int));
				int d;
				scanf("%d", &d);
				option->breakpoint.num++;
				option->breakpoint.bp[option->breakpoint.num -1] = d;
				break;}
			case 'm':{
				if(option->mem_print.mp == NULL)
					option->mem_print.mp = malloc(MP_NUM*sizeof(MEM_PRINT));
				unsigned int d;
				char c[2];
				scanf("%s", c);
				scanf("%u", &d);
				int i = option->mem_print.num;
				int b = 0;

				switch(c[0]){
					case 'i':
						option->mem_print.mp[i].type = Int;
						break;
					case 'u':
						option->mem_print.mp[i].type = Uint;
						break;
					/*case 'f':
						option->mem_print.mp[i].type = Foat;
						break;*/
					default:
						printf("invalid command\n");
						b = 1;
				}

				 switch(c[1]){
					 case 'b':
						 option->mem_print.mp[i].size = Byte;
						 break;
					 case 'h':
						 option->mem_print.mp[i].size = Half;
						 break;
					 case 'w':
						 option->mem_print.mp[i].size = Word;
						 break;
					 default:
						printf("invalid command\n");
						b = 1;
				 }
				 if(b == 1)break;
				option->mem_print.num++;
				option->mem_print.mp[i].addr = d;
				break;}
			default:
				printf("invalid command\n");
		}
	}
}

void print_reg(uint32_t reg, CPU cpu){
	for(int i = 0; i <  32; i++){
		if(reg & 1){
			printf("x[%2d]: %d\n", i, cpu.x[i]);
		}
		reg = reg >> 1;
	}
	putchar('\n');
}

int bp_check(CPU cpu, BREAKPOINT breakpoint){
	int n = breakpoint.num;
	int pc = cpu.pc;
	for(int i = 0; i < n; i++){
		if(pc == breakpoint.bp[i])
			return pc;
	}
	return 0;
}
