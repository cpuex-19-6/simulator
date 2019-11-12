#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "functions.h"
#include "instruction.h"
#include "io.h"

void io_init(IO *io){
	io->input = NULL;
	io->output = stdout;
	io->ftype_in = BIN;
	io->ftype_out = BIN;
}

void io_set(IO *io, OPTION option){
	if(option.fname_data != NULL){
		if(option.ftype_data == BIN){
			io->ftype_in = BIN;
			io->input = fopen(option.fname_data, "rb");
			printf("using \"%s\" as txt input file\n", option.fname_data);
			if(io->input == NULL)
				printf("cannot open input file\n");
		}
		else{ //TXT
			io->ftype_in = TXT;
			io->input = fopen(option.fname_data, "r");
			printf("using \"%s\" as txt input file\n", option.fname_data);
			if(io->input == NULL)
				printf("cannot open input file\n");
		}
	}
	else {
		printf("NO INPUT FILE!!!!\n");
	}

	if(option.fname_output != NULL){
		if(option.ftype_output == BIN){
			io->ftype_out = BIN;
			io->output = fopen(option.fname_output, "wb");
			if(io->output == NULL)
				printf("cannot open output file\n");
		}
		else{ //TXT
			io->ftype_out = TXT;
			io->output = fopen(option.fname_output, "w");
			if(io->output == NULL)
				printf("cannot open output file\n");
		}
	}
	else {
		printf("NO OUTPUT FILE!!!!\n");
	}
}

void io_close(IO *io){
	if(io->input != NULL)
		fclose(io->input);

	if(io->output != NULL)
		fclose(io->output);
}

void decode_IN(uint32_t instr, INSTR *imp){
	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t f20 = downto(instr, 31, 12); 

	imp->rd_or_imm = rd;


	switch(f20){
		case INW20:
			imp->op = INW;
			break;
		case INF20:
			imp->op = INF;
			break;
		default:
			perror("invalid f20: OP_IN");
			exit(EXIT_FAILURE);
	}
}

void decode_OUT(uint32_t instr, INSTR *imp){
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t f12 = downto(instr, 31, 20); 
	int32_t f8 = downto(instr, 14, 7);

	imp->rs1 = rs1;

	switch(f8){
		case OUTW8:
			if(f12 != OUTW12){
				perror("invalid f12: OUTW8");
				exit(EXIT_FAILURE);
			}

			imp->op = OUTW;
			break;
		case OUTB8:
			if(f12 != OUTW12){
				perror("invalid f12: OUTB8");
				exit(EXIT_FAILURE);
			}

			imp->op = OUTB;
			break;
		default:
			perror("invalid f8: OP_OUT");
			exit(EXIT_FAILURE);
	}
}

void exec_IN(INSTR instr, CPU *cpu, MEMORY *mem, IO *io){
	int32_t rd = instr.rd_or_imm;

	int n;
	if(feof(io->input))printf("already at the end of input file!!!!!!!\n");

	switch(instr.op){
		case INW:{
			int32_t data;
			if(io->ftype_in == BIN){
				n = fread(&data, sizeof(int32_t), 1, io->input);
				if(n <= 0)printf("failed to input: inw: bin\n");
				cpu->x[rd] = data;
			}
			else{ //TXT
				/*
				char tmp[32];
				n = fread(tmp, sizeof(char), 32, io->input);
				if(n <= 0)printf("failed to input: inw: txt\n");
				c2b_32((uint32_t *)&data, tmp, 32);
				cpu->x[rd] = data;
				*/
				int32_t data;
				n = fscanf(io->input, "%d", &data);
				if(n != 1)printf("failed to input: inw: txt\n");
				cpu->x[rd] = data;
			}
			break;}
		case INF:{
			float data;
			if(io->ftype_in == BIN){
				n = fread(&data, sizeof(float), 1, io->input);
				if(n <= 0)printf("failed to input: inf: bin\n");
				cpu->f[rd] = data;
			}
			else{ //TXT
				/*
				char tmp[32];
				n = fread(tmp, sizeof(char), 32, io->input);
				if(n <= 0)printf("failed to input: inf: txt\n");
				c2b_32((uint32_t *)&data, tmp, 32);
				cpu->f[rd] = data;
				*/
				float data;
				n = fscanf(io->input, "%f", &data);
				if(n != 1)printf("failed to input: inf: txt\n");
				cpu->f[rd] = data;
			}
			break;}
		default:
			perror("exec_IN: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

void exec_OUT(INSTR instr, CPU *cpu, MEMORY *mem, IO *io){
	int32_t rs1 = instr.rs1;

	int n;

	switch(instr.op){
		case OUTW:{
			int32_t data = cpu->x[rs1];

			if(io->ftype_out  == BIN){
				n = fwrite(&data, sizeof(int32_t), 1, io->output);
				if( n <= 0)printf("failed to output\n");
			}
			else{ //TXT
				char tmp[32];
				b2c_32(tmp, data);
				n = fwrite(tmp, sizeof(char), 32, io->output);
				if( n <= 0)printf("failed to output\n");
			}
			break;}
		case OUTB:{
			int8_t data = cpu->x[rs1];
			if(io->ftype_out  == BIN){
				n = fwrite(&data, sizeof(int8_t), 1, io->output);
				if( n <= 0)printf("failed to output\n");
			}
			else{ //TXT
				char tmp[8];
				b2c_8(tmp, data);
				n = fwrite(tmp, sizeof(char), 8, io->output);
				if( n <= 0)printf("failed to output\n");
			}
			break;}
		default:
			perror("exec_OUT: invalid instruction");
			exit(EXIT_FAILURE);
	}
}
