#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "option.h"
#include "disassemble.h"

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
			case 'r':
				option->mode = RUN;
				b = 1;
				break;
			case 's':
				option->mode = STEP;
				b = 1;
				break;
			case 'q':
				option->mode = QUIT;
				b = 1;
				break;
			default:
				printf("invalid command\n");
		}
	}
}
