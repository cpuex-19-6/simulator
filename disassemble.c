#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "instruction.h"
#include "sim.h"
#include "fpu.h"
#include "disassemble.h"
#include "fpu.h"

void mnemonic_LA(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	switch(f3){
		case F3_A:
			switch(f7){
				case F7_ADD:
					strcpy(assem->mnemonic, "add");
					break;
				case F7_SUB:
					strcpy(assem->mnemonic, "sub");
					break;
				default:
					perror("invalid f7: F3_A");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_OR: 
			switch(f7){
				case F7_OR:
					strcpy(assem->mnemonic, "or");
					break;
				case F7_REM:
					strcpy(assem->mnemonic, "rem");
					break;
				default:
					perror("invalid f7: F3_OR");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_AND: 
			switch(f7){
				case F7_AND:
					strcpy(assem->mnemonic, "and");
					break;
				case F7_REMU:
					strcpy(assem->mnemonic, "remu");
					break;
				default:
					perror("invalid f7: F3_AND");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_XOR:
			switch(f7){
				case F7_XOR:
					strcpy(assem->mnemonic, "xor");
					break;
				case F7_DIV:
					strcpy(assem->mnemonic, "div");
					break;
				default:
					perror("invalid f7: F3_XOR");
					exit(EXIT_FAILURE);
			}
			break;
		case F3_SL:
			if(f7 == F7_SLL){
				strcpy(assem->mnemonic, "sll");
			}
			else {
				perror("invalid f7: F3_SL");
				exit(EXIT_FAILURE);
			}
			break;
		case F3_SR:
			switch(f7){
				case F7_DIVU:
					strcpy(assem->mnemonic, "divu");
					break;
				case F7_SRL:
					strcpy(assem->mnemonic, "srl");
					break;
				case F7_SRA:
					strcpy(assem->mnemonic, "sra");
					break;
				default:
					perror("invalid f7: F3_SL");
					exit(EXIT_FAILURE);
			}
			break;
		default:
			perror("invalid f3: OP_LA");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_LAI(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);
	
	switch(f3){
		case F3_ADDI:
			strcpy(assem->mnemonic, "addi");
			break;
		case F3_ORI:
			strcpy(assem->mnemonic, "ori");
			break;
		case F3_ANDI:
			strcpy(assem->mnemonic, "andi");
			break;
		case F3_XORI:
			strcpy(assem->mnemonic, "xori");
			break;
		case F3_SLI:
			if(f7 == F7_SLLI){
				strcpy(assem->mnemonic, "slli");
			}
			else {
				perror("invalid f7: F3_SLI");
				exit(EXIT_FAILURE);
			}
			break;
		case F3_SRI:
			if(f7 == F7_SRLI){
				strcpy(assem->mnemonic, "srli");

			}
			else if(f7 == F7_SRAI){
				strcpy(assem->mnemonic, "srai");
			}
			else {
				perror("invalid f7: F3_SRI");
				exit(EXIT_FAILURE);
			}
			break;
		default:
			perror("invalid f3: OP_LA");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_LD(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);

	switch(f3){
		case F3_LB:
			strcpy(assem->mnemonic, "lb");
			break;
		case F3_LH:
			strcpy(assem->mnemonic, "lh");
			break;
		case F3_LW:
			strcpy(assem->mnemonic, "lw");
			break;
		case F3_LBU:
			strcpy(assem->mnemonic, "lbu");
			break;
		case F3_LHU:
			strcpy(assem->mnemonic, "lhu");
			break;
		default:
			perror("invalid f3: OP_LD");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_ST(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);

	switch(f3){
		case F3_SB:
			strcpy(assem->mnemonic, "sb");
			break;
		case F3_SH:
			strcpy(assem->mnemonic, "sh");
			break;
		case F3_SW:
			strcpy(assem->mnemonic, "sw");
			break;
		default:
			perror("invalid f3: OP_ST");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_CB(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);

	switch(f3){
		case F3_BEQ:
			strcpy(assem->mnemonic, "beq");
			break;
		case F3_BNE:
			strcpy(assem->mnemonic, "bne");
			break;
		case F3_BLT:
			strcpy(assem->mnemonic, "blt");
			break;
		case F3_BGE:
			strcpy(assem->mnemonic, "bge");
			break;
		case F3_BLTU:
			strcpy(assem->mnemonic, "bltu");
			break;
		case F3_BGEU:
			strcpy(assem->mnemonic, "bgeu");
			break;
		default:
			perror("invalid f3: OP_CB");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_IN(uint32_t instr, ASSEM *assem){
	int32_t f20 = downto(instr, 31, 12); 

	switch(f20){
		case INW:
			strcpy(assem->mnemonic, "inw");
			strcpy(assem->reg, "x");
			break;
		case INF:
			strcpy(assem->mnemonic, "inf");
			strcpy(assem->reg, "f");
			break;
		default:
			perror("invalid f20: OP_IN");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_OUT(uint32_t instr, ASSEM *assem){
	int32_t f12 = downto(instr, 31, 20); 
	int32_t f8 = downto(instr, 14, 7);

	switch(f8){
		case OUTW8:
			if(f12 != OUTW12){
				perror("invalid f12: OUTW8");
				exit(EXIT_FAILURE);
			}
			strcpy(assem->mnemonic, "outw");
			strcpy(assem->reg, "x");
			break;
		case OUTB8:
			if(f12 != OUTW12){
				perror("invalid f12: OUTB8");
				exit(EXIT_FAILURE);
			}
			strcpy(assem->mnemonic, "outb");
			strcpy(assem->reg, "x");
			break;
		default:
			perror("invalid f8: OP_OUT");
			exit(EXIT_FAILURE);
	}
}

void disassem_instr(uint32_t instr, ASSEM *assem){
	uint32_t opcd = downto(instr, 6, 0);

	switch(opcd){
		case OP_LUI:	//U
			assem->itype = U;
			strcpy(assem->mnemonic, "lui");
			strcpy(assem->reg, "x");
			break;
		case OP_LA:	//R
			assem->itype = R;
			mnemonic_LA(instr, assem);
			strcpy(assem->reg, "xxx");
			break;
		case OP_LAI:	//I
			assem->itype = I;
			mnemonic_LAI(instr, assem);
			strcpy(assem->reg, "xx");
			break;
		case OP_LD:	//I
			assem->itype = I;
			mnemonic_LD(instr, assem);
			strcpy(assem->reg, "xx");
			break;
		case OP_ST:	//S
			assem->itype = S;
			mnemonic_ST(instr, assem);
			strcpy(assem->reg, "xx");
			break;
		case OP_AUIPC:	//U
			assem->itype = U;
			strcpy(assem->mnemonic, "auipc");
			strcpy(assem->reg, "x");
			break;
		case OP_JAL:	//J
			assem->itype = J;
			strcpy(assem->mnemonic, "jal");
			strcpy(assem->reg, "x");
			break;
		case OP_JALR:	//I
			assem->itype = I;
			strcpy(assem->mnemonic, "jalr");
			strcpy(assem->reg, "xx");
			break;
		case OP_CB:	//B
			assem->itype = B;
			mnemonic_CB(instr, assem);
			strcpy(assem->reg, "xx");
			break;
		//floating point instruction
		case OP_FLW: //I
			assem->itype = I;
			strcpy(assem->mnemonic, "flw");
			strcpy(assem->reg, "fx");
			break;
		case OP_FSW: //S
			assem->itype = S;
			strcpy(assem->mnemonic, "fsw");
			strcpy(assem->reg, "xf");
			break;
		case OP_FLA: //R or R_sub
			mnemonic_FLA(instr, assem);
			break;
		case OP_IN: //R_sub_sub
			assem->itype = R_sub_sub;
			mnemonic_IN(instr, assem);
			break;
		case OP_OUT: //R_sub_sub
			assem->itype = R_sub_sub_sub;
			mnemonic_OUT(instr, assem);
			break;
		default:
			perror("Unkown instruction");
			exit(EXIT_FAILURE);
	}
	assem->rd = (int32_t)downto(instr, 11, 7);
	assem-> rs1 = (int32_t)downto(instr, 19, 15);
	assem-> rs2 = (int32_t)downto(instr, 24, 20);
	assem->imm = immediate(instr, assem->itype);
}

void print_assembly(ASSEM assem){
	switch(assem.itype){
		case R:
			printf("%-6s %c%-2d, %c%-2d, %c%-2d", assem.mnemonic, assem.reg[0], assem.rd, assem.reg[1] ,assem.rs1, assem.reg[2] ,assem.rs2);
			break;
		case R_sub:
			printf("%-6s %c%-2d, %c%-2d", assem.mnemonic, assem.reg[0], assem.rd, assem.reg[1] ,assem.rs1);
			if(strcmp(assem.mnemonic, "froundrm") == 0)
				printf(", %s", assem.rm);
			break;
		case R_sub_sub:
			printf("%-6s %c%-2d", assem.mnemonic, assem.reg[0], assem.rd);
			break;
		case R_sub_sub_sub:
			printf("%-6s %c%-2d", assem.mnemonic, assem.reg[0], assem.rs1);
			break;
		case I:
			printf("%-6s %c%-2d, %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rd, assem.reg[1], assem.rs1, assem.imm);
			break;
		case S:
			printf("%-6s %c%-2d, %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rs1, assem.reg[1], assem.rs2, assem.imm);
			break;
		case B:
			printf("%-6s %c%-2d, %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rs1, assem.reg[1], assem.rs2, assem.imm);
			break;
		case U:
			printf("%-6s %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rd, assem.imm);
			break;
		default: //J
			printf("%-6s %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rd, assem.imm);
	}
}
