#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "sim.h"
#include "disassemble.h"

void mnemonic_LA(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	switch(f3){
		case F3_A:
			if(f7 == F7_ADD){
				strcpy(assem->mnemonic, "add");
			}
			else if(f7 == F7_SUB){
				strcpy(assem->mnemonic, "sub");
			}
			else{
				perror("Unkown instruction");
				exit(EXIT_FAILURE);
			}
			break;
		case F3_OR:
			strcpy(assem->mnemonic, "or");
			break;
		case F3_AND:
			strcpy(assem->mnemonic, "and");
			break;
		default:
			perror("Unkown indtruction");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_LAI(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);
	
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
		default:
			perror("Unkown indtruction");
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
			perror("Unkown indtruction");
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
			perror("Unkown indtruction");
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
			perror("Unkown indtruction");
			exit(EXIT_FAILURE);
	}
}

void disassem_instr(uint32_t instr, ASSEM *assem){
	uint32_t opcd = downto(instr, 6, 0);

	switch(opcd){
		case OP_LUI:	//U
			assem->itype = U;
			strcpy(assem->mnemonic, "lui");
			break;
		case OP_LA:	//R
			assem->itype = R;
			mnemonic_LA(instr, assem);
			break;
		case OP_LAI:	//I
			assem->itype = I;
			mnemonic_LAI(instr, assem);
			break;
		case OP_LD:	//I
			assem->itype = I;
			mnemonic_LD(instr, assem);
			break;
		case OP_ST:	//S
			assem->itype = S;
			mnemonic_ST(instr, assem);
			break;
		case OP_AUIPC:	//U
			assem->itype = U;
			strcpy(assem->mnemonic, "auipc");
			break;
		case OP_JAL:	//J
			assem->itype = J;
			strcpy(assem->mnemonic, "jal");
			break;
		case OP_JALR:	//I
			assem->itype = I;
			strcpy(assem->mnemonic, "jalr");
			break;
		case OP_CB:	//B
			assem->itype = B;
			mnemonic_CB(instr, assem);
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
			printf("%-6s x%-2d, x%-2d, x%-2d", assem.mnemonic, assem.rd, assem.rs1, assem.rs2);
			break;
		case I:
			printf("%-6s x%-2d, x%-2d, %d", assem.mnemonic, assem.rd, assem.rs1, assem.imm);
			break;
		case S:
			printf("%-6s x%-2d, x%-2d, %d", assem.mnemonic, assem.rs1, assem.rs2, assem.imm);
			break;
		case B:
			printf("%-6s x%-2d, x%-2d, %d", assem.mnemonic, assem.rs1, assem.rs2, assem.imm);
			break;
		case U:
			printf("%-6s x%-2d, %d", assem.mnemonic, assem.rd, assem.imm);
			break;
		default: //J
			printf("%-6s x%-2d, %d", assem.mnemonic, assem.rd, assem.imm);
	}
}

void print_binary(uint32_t instr){
	uint32_t check = 1 << 31;
	char s[33];

	for(int i = 0; i < 32; i++){
		if(check & instr){
			s[i] = '1';
		}
		else {
			s[i] = '0';
		}
		instr = instr << 1;
	}
	s[32] = '\0';

	printf("%s", s);
}
