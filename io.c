#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "instruction.h"
#include "cpu.h"
#include "memory.h"
#include "option.h"
#include "io.h"

void io_init(IO *io){
	io->input = NULL;
	io->output = NULL;
	io->ftype_in = BIN;
	io->ftype_out = BIN;
}

void io_set(IO *io, OPTION option){
	if(option.fname_data != NULL){
		if(option.ftype_data == BIN){
			io->ftype_in = BIN;
			io->input = fopen(option.fname_data, "wb");
			if(io->input == NULL)
				printf("cannot open input file\n");
		}
		else{ //TXT
			io->ftype_in = TXT;
			io->input = fopen(option.fname_data, "wb");
			io->input = fopen(option.fname_data, "w");
			if(io->input == NULL)
				printf("cannot open input file\n");
		}
	}
	if(option.fname_output != NULL){
		if(option.ftype_output == BIN){
			io->ftype_in = BIN;
			io->output = fopen(option.fname_output, "wb");
			if(io->output == NULL)
				printf("cannot open output file\n");
		}
		else{ //TXT
			io->ftype_in = TXT;
			io->output = fopen(option.fname_output, "w");
			if(io->output == NULL)
				printf("cannot open output file\n");
		}
	}
}

void io_close(IO *io){
	if(io->input != NULL)
		fclose(io->input);

	if(io->output != NULL)
		fclose(io->output);
}

void exec_IN(uint32_t instr, CPU *cpu, MEMORY *mem, IO *io){
	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t f20 = downto(instr, 31, 12); 

	int n;

	switch(f20){
		case INW:{
			int32_t data;
			if(io->ftype_in == BIN){
				n = fread(&data, sizeof(int32_t), 1, io->input);
				if(n <= 0)printf("failed to input\n");
				cpu->x[rd] = data;
			}
			else{ //TXT
				char tmp[32];
				n = fread(tmp, sizeof(char), sizeof(int32_t), io->input);
				if(n <= 0)printf("failed to input\n");
				c2b_32((uint32_t *)&data, tmp, sizeof(int32_t));
				cpu->x[rd] = data;
			}
			break;}
		case INF:{
			float data;
			if(io->ftype_in == BIN){
				n = fread(&data, sizeof(float), 1, io->input);
				if(n <= 0)printf("failed to input\n");
				cpu->f[rd] = data;
			}
			else{ //TXT
				char tmp[32];
				n = fread(tmp, sizeof(char), sizeof(float), io->input);
				if(n <= 0)printf("failed to input\n");
				c2b_32((uint32_t *)&data, tmp, sizeof(float));
				cpu->f[rd] = data;
			}
			break;}
		default:
			perror("invalid instruction");
			exit(EXIT_FAILURE);
	}
}

void exec_OUT(uint32_t instr, CPU *cpu, MEMORY *mem, IO *io){
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t f12 = downto(instr, 31, 20); 
	int32_t f8 = downto(instr, 14, 7);

	int n;

	switch(f8){
		case OUTW8:{
			if(f12 != OUTW12){
				perror("invalid instruction");
				exit(EXIT_FAILURE);
			}
			int32_t data = cpu->x[rs1];
			if(io->ftype_out  == BIN){
				n = fwrite(&data, sizeof(int32_t), 1, io->output);
				if( n <= 0)printf("failed to output\n");
			}
			else{ //TXT
				char tmp[32];
				b2c_32(tmp, data);
				fwrite(tmp, sizeof(char), sizeof(int32_t), io->output);
			}
			break;}
		case OUTB8:{
			if(f12 != OUTW12){
				perror("invalid instruction");
				exit(EXIT_FAILURE);
			}
			int8_t data = cpu->x[rs1];
			if(io->ftype_out  == BIN){
				n = fwrite(&data, sizeof(int8_t), 1, io->output);
				if( n <= 0)printf("failed to output\n");
			}
			else{ //TXT
				char tmp[8];
				b2c_8(tmp, data);
				fwrite(tmp, sizeof(char), sizeof(int8_t), io->output);
			}
			break;}
		default:
			perror("invalid instruction");
			exit(EXIT_FAILURE);
	}
}
