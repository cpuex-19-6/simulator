#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "functions.h"
#include "instruction.h"
#include "fpu.h"
#include "disassemble.h"

void mnemonic_LA(INSTR instr, ASSEM *assem){

	switch(instr.op){
		case ADD:
			strcpy(assem->mnemonic, "add");
			break;
		case SUB:
			strcpy(assem->mnemonic, "sub");
			break;
		case OR: 
			strcpy(assem->mnemonic, "or");
			break;
		case REM:
			strcpy(assem->mnemonic, "rem");
			break;
		case AND: 
			strcpy(assem->mnemonic, "and");
			break;
		case REMU:
			strcpy(assem->mnemonic, "remu");
			break;
		case XOR:
			strcpy(assem->mnemonic, "xor");
			break;
		case DIV:
			strcpy(assem->mnemonic, "div");
			break;
		case SLL:
			strcpy(assem->mnemonic, "sll");
			break;
		case DIVU:
			strcpy(assem->mnemonic, "divu");
			break;
		case SRL:
			strcpy(assem->mnemonic, "srl");
			break;
		case SRA:
			strcpy(assem->mnemonic, "sra");
			break;
		default:
			perror("exec_LA: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_LAI(INSTR instr, ASSEM *assem){

	switch(instr.op){
		case ADDI: 
			strcpy(assem->mnemonic, "addi");
			break;
		case ORI: 
			strcpy(assem->mnemonic, "ori");
			break;
		case ANDI:
			strcpy(assem->mnemonic, "andi");
			break;
		case XORI:
			strcpy(assem->mnemonic, "xori");
			break;
		case SLLI:
			strcpy(assem->mnemonic, "slli");
			break;
		case SRLI:
			strcpy(assem->mnemonic, "srli");
			break;
		case SRAI:
			strcpy(assem->mnemonic, "srai");
			break;
		default:
			perror("exec_LAI: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_LD(INSTR instr, ASSEM *assem){

	switch(instr.op){
		case LW:
			strcpy(assem->mnemonic, "lw");
			break;
		/*case LB:
			strcpy(assem->mnemonic, "lb");
			break;
		case LH:
			strcpy(assem->mnemonic, "lh");
			break;
		case LBU:
			strcpy(assem->mnemonic, "lbu");
			break;
		case LHU:
			strcpy(assem->mnemonic, "lhu");
			break;*/
		default:
			perror("invalid f3: OP_LD");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_ST(INSTR instr, ASSEM *assem){

	switch(instr.op){
		case SW:
			strcpy(assem->mnemonic, "sw");
			break;
		/*case SB:
			strcpy(assem->mnemonic, "sb");
			break;
		case SH:
			strcpy(assem->mnemonic, "sh");
			break;*/
		default:
			perror("invalid f3: OP_ST");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_CB(INSTR instr, ASSEM *assem){

	switch(instr.op){
		case BEQ:
			strcpy(assem->mnemonic, "beq");
			break;
		case BNE:
			strcpy(assem->mnemonic, "bne");
			break;
		case BLT:
			strcpy(assem->mnemonic, "blt");
			break;
		case BGE:
			strcpy(assem->mnemonic, "bge");
			break;
		case BLTU:
			strcpy(assem->mnemonic, "bltu");
			break;
		case BGEU:
			strcpy(assem->mnemonic, "bgeu");
			break;
		default:
			perror("invalid f3: OP_CB");
			exit(EXIT_FAILURE);
	}
}

void mnemonic_IN(INSTR instr, ASSEM *assem){

	switch(instr.op){
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

void mnemonic_OUT(INSTR instr, ASSEM *assem){

	switch(instr.op){
		case OUTW:
			strcpy(assem->mnemonic, "outw");
			strcpy(assem->reg, "x");
			break;
		case OUTB:
			strcpy(assem->mnemonic, "outb");
			strcpy(assem->reg, "x");
			break;
		default:
			perror("mnemonic_OUT: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

void disassem_instr(INSTR instr, ASSEM *assem){

	assem->rd_or_imm = instr.rd_or_imm;
	assem->rs1 = instr.rs1;
	assem->rs2_or_imm = instr.rs2_or_imm;

	if(instr.op == LUI){
		assem->itype = U;
		strcpy(assem->mnemonic, "lui");
		strcpy(assem->reg, "x");
	}
	else if(LA_first <= instr.op && instr.op <= LA_last){
		assem->itype = R;
		mnemonic_LA(instr, assem);
		strcpy(assem->reg, "xxx");
	}
	else if(LAI_first <= instr.op && instr.op <= LAI_last){
		assem->itype = I;
		mnemonic_LAI(instr, assem);
		strcpy(assem->reg, "xx");
	}
	else if(instr.op == LW){
		assem->itype = I;
		mnemonic_LD(instr, assem);
		strcpy(assem->reg, "xx");
	}
	else if(instr.op == SW){
		assem->itype = S;
		mnemonic_ST(instr, assem);
		strcpy(assem->reg, "xx");
	}
	else if(instr.op == AUIPC){
		assem->itype = U;
		strcpy(assem->mnemonic, "auipc");
		strcpy(assem->reg, "x");
	}
	else if(instr.op == JAL){
		assem->itype = J;
		strcpy(assem->mnemonic, "jal");
		strcpy(assem->reg, "x");
	}
	else if(instr.op == JALR){
		assem->itype = I;
		strcpy(assem->mnemonic, "jalr");
		strcpy(assem->reg, "xx");
	}
	else if(CB_first <= instr.op && instr.op <= CB_last){
		assem->itype = B;
		mnemonic_CB(instr, assem);
		strcpy(assem->reg, "xx");
	}
	else if(instr.op == FLW){
		assem->itype = I;
		strcpy(assem->mnemonic, "flw");
		strcpy(assem->reg, "fx");
	}
	else if(instr.op == FSW){
		assem->itype = S;
		strcpy(assem->mnemonic, "fsw");
		strcpy(assem->reg, "xf");
	}
	else if(FLA_first <= instr.op && instr.op <= FLA_last){
		mnemonic_FLA(instr, assem);
	}
	else if(instr.op == INW || instr.op == INF){
		assem->itype = R_sub_sub;
		mnemonic_IN(instr, assem);
	}
	else if(instr.op == OUTB || instr.op == OUTW){
		assem->itype = R_sub_sub_sub;
		mnemonic_OUT(instr, assem);
	}
	else if(FCB_first <= instr.op && instr.op <= FCB_last){
		assem->itype = B;
		mnemonic_FCB(instr, assem);
		strcpy(assem->reg, "ff");
	}
	else{
		perror("exec_instr: invalid instruction");
		exit(EXIT_FAILURE);
	}
}

void print_assembly(ASSEM assem){
	switch(assem.itype){
		case R:
			printf("%-6s %c%-2d, %c%-2d, %c%-2d", assem.mnemonic, assem.reg[0], assem.rd_or_imm, assem.reg[1] ,assem.rs1, assem.reg[2] ,assem.rs2_or_imm);
			break;
		case R_sub:
			printf("%-6s %c%-2d, %c%-2d", assem.mnemonic, assem.reg[0], assem.rd_or_imm, assem.reg[1] ,assem.rs1);
			if(strcmp(assem.mnemonic, "froundrm") == 0)
				printf(", %s", assem.rm);
			break;
		case R_sub_sub:
			printf("%-6s %c%-2d", assem.mnemonic, assem.reg[0], assem.rd_or_imm);
			break;
		case R_sub_sub_sub:
			printf("%-6s %c%-2d", assem.mnemonic, assem.reg[0], assem.rs1);
			break;
		case I:
			printf("%-6s %c%-2d, %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rd_or_imm, assem.reg[1], assem.rs1, assem.rs2_or_imm);
			break;
		case S:
			printf("%-6s %c%-2d, %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rs1, assem.reg[1], assem.rs2_or_imm, assem.rd_or_imm);
			break;
		case B:
			printf("%-6s %c%-2d, %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rs1, assem.reg[1], assem.rs2_or_imm, assem.rd_or_imm);
			break;
		case U:
			printf("%-6s %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rd_or_imm, assem.rs2_or_imm);
			break;
		default: //J
			printf("%-6s %c%-2d, %d", assem.mnemonic, assem.reg[0], assem.rd_or_imm, assem.rs2_or_imm);
	}
}
