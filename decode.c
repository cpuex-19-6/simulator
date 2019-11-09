#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "struct.h"
#include "instruction.h"
#include "functions.h"
#include "decode.h"
#include "fpu.h"
#include "io.h"

//即値生成　無駄が多いが、仕様書と見た目が同じになるように
int32_t immediate(uint32_t instr, INSTR_TYPE ip){
	uint32_t im31 = downto(instr, 31, 31);
	uint32_t im30_25 = downto(instr, 30, 25);
	uint32_t im30_20 = downto(instr, 30, 20);
	uint32_t im24_21 = downto(instr, 24, 21);
	uint32_t im20 = downto(instr, 20, 20);
	uint32_t im19_12 = downto(instr, 19, 12);
	uint32_t im11_8 = downto(instr, 11, 8);
	uint32_t im7 = downto(instr, 7, 7);

	uint32_t sign = 0;
	uint32_t ans;

	switch(ip){
		case I:
			if (im31 != 0){
				sign = (~0) << 11;
			}
			ans = sign | (im30_25 << 5) | (im24_21 << 1) | im20;
			break;
		case S: 
			if (im31 != 0){
				sign = (~0) << 11;
			}
			ans = sign | (im30_25 << 5) | (im11_8 << 1) | im7;
			break;
		case B: 
			if (im31 != 0){
				sign = (~0) << 12;
			}
			ans = sign | (im7 << 11) | (im30_25 << 5) | (im11_8 << 1) | 0;
			break;
		case U: 
			ans = (im31 << 31) | (im30_20 << 20) | (im19_12 << 12) | 0;
			break;
		case J: 
			if (im31 != 0){
				sign = (~0) << 20;
			}
			ans = sign | (im19_12 << 12) | (im20 << 11) | (im30_25 << 5) | (im24_21 << 1) | 0;
			break;
		default: //R, R_sub, R_sub_sub, R_sub_sub_sub
			ans = 0;
	}
	return (int32_t)ans;
}

//arithmetic operation
void decode_LA(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	switch(f3){
		case F3_A:
			switch(f7){
				case F7_ADD:
					imp->op = ADD;
					break;
				case F7_SUB:
					imp->op = SUB;
					break;
				default:
					perror("invalid f7: F3_A");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_OR: 
			switch(f7){
				case F7_OR:
					imp->op = OR;
					break;
				case F7_REM:
					imp->op = REM;
					break;
				default:
					perror("invalid f7: F3_OR");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_AND: 
			switch(f7){
				case F7_AND:
					imp->op = AND;
					break;
				case F7_REMU:
					imp->op = REMU;
					break;
				default:
					perror("invalid f7: F3_AND");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_XOR:
			switch(f7){
				case F7_XOR:
					imp->op = XOR;
					break;
				case F7_DIV:
					imp->op = DIV;
					break;
				default:
					perror("invalid f7: F3_XOR");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_SL:
			if(f7 == F7_SLL){
				imp->op = SLL;
			}
			else {
				perror("invalid f7: F3_SL");
				exit(EXIT_FAILURE);
			}
			break;
		case F3_SR:
			switch(f7){
				case F7_DIVU:
					imp->op = DIVU;
					break;
				case F7_SRL:
					imp->op = SRL;
					break;
				case F7_SRA:
					imp->op = SRA;
					break;
				default:
					perror("invalid f7: F3_SR");
					exit(EXIT_FAILURE);
			}
			break;
		default:
			perror("invalid f3: OP_LA");
			exit(EXIT_FAILURE);
	}	
}

//logic and arithmetic
void decode_LAI(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t imm = immediate(instr, I);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = imm;

	switch(f3){
		case F3_ADDI: 
			imp->op = ADDI;
			break;
		case F3_ORI: 
			imp->op = ORI;
			break;
		case F3_ANDI:
			imp->op = ANDI;
			break;
		case F3_XORI:
			imp->op = XORI;
			break;
		case F3_SLI:
			if(f7 == F7_SLLI){
				imp->op = SLLI;
			}
			else {
				perror("invalid f7: F3_SLI");
				exit(EXIT_FAILURE);
			}
			break;
		case F3_SRI:
			if(f7 == F7_SRLI){
				imp->op = SRLI;

			}
			else if(f7 == F7_SRAI){
				imp->op = SRAI;
			}
			else {
				perror("invalid f7: F3_SRI");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			perror("invalid f3: OP_LAI");
			exit(EXIT_FAILURE);
	}
}

//load
void decode_LD(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t imm = immediate(instr, I);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = imm;

	switch(f3){
		case F3_LW:
			imp->op = LW;
			break;
		/*case F3_LB:
			imp->op = LB;
			break;
		case F3_LH:
			imp->op = LH;
			break;
		case F3_LW:
			imp->op = LW;
			break;
		case F3_LBU:
			imp->op = LBU;
			break;
		case F3_LHU:
			imp->op = LHU;
			break;*/
		default:
			perror("invalid f3: OP_LD");
			exit(EXIT_FAILURE);
	}
}

//store 間違っているかも
void decode_ST(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int32_t imm = immediate(instr, S);

	imp->rd_or_imm = imm;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;


	switch(f3){
		case F3_SW:
			imp->op = SW;
			break;
		/*case F3_SB:
			break;
		case F3_SH:
			break;*/
		default:
			perror("invalid f3: OP_ST");
			exit(EXIT_FAILURE);
	}
}

//conditional branch
void decode_CB(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int32_t imm = immediate(instr, B);
	imm = imm & ((~0) << 2); //immediate wrapping

	imp->rd_or_imm = imm;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	switch(f3){
		case F3_BEQ:
			imp->op = BEQ;
			break;
		case F3_BNE:
			imp->op = BNE;
			break;
		case F3_BLT:
			imp->op = BLT;
			break;
		case F3_BGE:
			imp->op = BGE;
			break;
		case F3_BLTU:
			imp->op = BLTU;
			break;
		case F3_BGEU:
			imp->op = BGEU;
			break;
		default:
			perror("invalid f3: OP_CB");
			exit(EXIT_FAILURE);
	}
}

//rd がゼロレジスタの時は代入させておいて最後に代入がいい?
void decode_instr(uint32_t instr, INSTR *imp){
	uint32_t opcd = downto(instr, 6, 0);

	if(instr == 0){
		imp->op = END;
	}
	else{
	switch(opcd){
		case OP_LUI:{
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, U);

			imp->op = LUI;
			imp->rd_or_imm = rd;
			imp->rs2_or_imm = imm;
			break;}
		case OP_LA: 
			decode_LA(instr, imp);
			break;
		case OP_LAI:
			decode_LAI(instr, imp);
			break;
		case OP_LD: 
			decode_LD(instr, imp);
			break;
		case OP_ST: 
			decode_ST(instr, imp);
			break;
		case OP_AUIPC:{
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, U);
			imm = imm & ((~0) << 2); // immediate wrapping

			imp->op = AUIPC;
			imp->rd_or_imm = rd;
			imp->rs2_or_imm = imm;
			break;}
		case OP_JAL: {
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, J);
			imm = imm & ((~0) << 2); // immediate wrapping

			imp->op = JAL;
			imp->rd_or_imm = rd;
			imp->rs2_or_imm = imm;
			break;}
		case OP_JALR: {
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t rs1 = (int32_t)downto(instr, 19, 15);
			int32_t imm = immediate(instr, I);

			imp->op = JALR;
			imp->rd_or_imm = rd;
			imp->rs1 = rs1;
			imp->rs2_or_imm = imm;
			break;}
		case OP_CB: 
			decode_CB(instr, imp);
			break;
		case OP_FLW:
			decode_FLW(instr, imp);
			break;
		case OP_FSW:
			decode_FSW(instr, imp);
			break;
		case OP_FLA:
			decode_FLA(instr, imp);
			break;
		case OP_IN:
			decode_IN(instr, imp);
			break;
		case OP_OUT:
			decode_OUT(instr, imp);
			break;
		default: 
			putchar('\n');
			printf(" 0b");
			print_binary(instr);
			perror("unkown instruction");
			exit(EXIT_FAILURE);
	}	
	}
}

void decode(uint32_t *instrs, int size, MEMORY *mem){
	for(int i = 0; i < size; i++){
		decode_instr(instrs[i], mem->instr + i);
	}
}
