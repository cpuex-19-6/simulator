#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <fenv.h>

#include "struct.h"
#include "functions.h"
#include "instruction.h"
#include "decode.h"
#include "memory.h"
#include "fpu.h"

//floating point load
void decode_FLW(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t imm = immediate(instr, I);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = imm;

	if(f3 == F3_FLW){
		imp->op = FLW;
	}
	else {
		perror("invalid f3: OP_FLW");
		exit(EXIT_FAILURE);
	}
}

//floating point store
void decode_FSW(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);
	int32_t imm = immediate(instr, S);

	imp->rd_or_imm = imm;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	if(f3 == F3_FSW){
		imp->op = FSW;
	}
	else {
		perror("invalid f3: OP_FSW");
		exit(EXIT_FAILURE);
	}
}

//floating point sign injection
void decode_FSGNJ(int instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	switch(f3){
		case F3_FSGNJ:
			imp->op = FSGNJ;
			break;
		case F3_FSGNJX:
			imp->op = FSGNJX;
			break;
		case F3_FSGNJN:
			imp->op = FSGNJN;
			break;
		default:
			perror("invalid f3: F7_FSGNJ");
			exit(EXIT_FAILURE);
	}
}

void decode_FROUND(int instr, INSTR *imp){
	uint32_t rm = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	
	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	//imp->rs2 = rm;

	switch(rm){
		case RM_RNE:
			imp->op = FROUNDRM_RNE;
			break;
		case RM_RTZ:
			imp->op = FROUNDRM_RTZ;
			break;
		case RM_RDN:
			imp->op = FROUNDRM_RDN;
			break;
		case RM_RUP:
			imp->op = FROUNDRM_RUP;
			break;
		case RM_RMM:
			imp->op = FROUNDRM_RMM;
			break;
		default:
			perror("invalid rm: F7_FROUND");
			exit(EXIT_FAILURE);
	}
}

//convert integer to float
void decode_TOF(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	if(f3 != F3_RM){
		perror("invalid rm: F7_TOF");
		exit(EXIT_FAILURE);
	}

	switch(rs2){
		case RS2_ITOF:
			imp->op = ITOF;
			break;
		/*case RS2_UTOF:
			imp->op = UTOF;
			break;*/
		default:
			perror("invalid rs2: F7_TOF");
			exit(EXIT_FAILURE);
	}
}

//convert float to integer
void decode_TOI(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	if(f3 != F3_RM){
		perror("invalid rm: F7_TOI");
		exit(EXIT_FAILURE);
	}

	switch(rs2){
		case RS2_FTOI:
			imp->op = FTOI;
			break;
		/*case RS2_FTOUI:
			imp->op = FTOUI;
			break;*/
		default:
			perror("invalid rs2: F7_TOI");
			exit(EXIT_FAILURE);
	}
}

//floating point compare
void decode_FCOMPARE(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	imp->rd_or_imm = rd;
	imp->rs1 = rs1;
	imp->rs2_or_imm = rs2;

	switch(f3){
		case F3_FEQ:
			imp->op = FEQ;
			break;
		case F3_FLT:
			imp->op = FLT;
			break;
		case F3_FLE:
			imp->op = FLE;
			break;
		default:
			perror("inavlid f3: F7_FCOMPARE");
			exit(EXIT_FAILURE);
	}
}

//floating point arithmetic logic and others
void decode_FLA(uint32_t instr, INSTR *imp){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	switch(f7){
		case F7_FADD:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FADD");
				exit(EXIT_FAILURE);
			}
			imp->op = FADD;
			imp->rd_or_imm = rd;
			imp->rs1 = rs1;
			imp->rs2_or_imm = rs2;
			break;
		case F7_FSUB:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FSUB");
				exit(EXIT_FAILURE);
			}
			imp->op = FSUB;
			imp->rd_or_imm = rd;
			imp->rs1 = rs1;
			imp->rs2_or_imm = rs2;
			break;
		case F7_FMUL:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FMUL");
				exit(EXIT_FAILURE);
			}
			imp->op = FMUL;
			imp->rd_or_imm = rd;
			imp->rs1 = rs1;
			imp->rs2_or_imm = rs2;
			break;
		case F7_FDIV:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FDIV");
				exit(EXIT_FAILURE);
			}
			imp->op = FDIV;
			imp->rd_or_imm = rd;
			imp->rs1 = rs1;
			imp->rs2_or_imm = rs2;
			break;
		case F7_FSQRT:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FSQRT");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_FSQRT){
				imp->op = FSQRT;
				imp->rd_or_imm = rd;
				imp->rs1 = rs1;
			}
			else {
				perror("invalid rs2: F7_FSQRT");
				exit(EXIT_FAILURE);
			}
			break;
		case F7_FSGNJ:
			decode_FSGNJ(instr, imp);
			break;
		case F7_FROUND:
			decode_FROUND(instr, imp);
			break;
		case F7_TOF:
			decode_TOF(instr, imp);
			break;
		case F7_TOI:
			decode_TOI(instr, imp);
			break;
		case F7_FMVI:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FMVI");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_FMVI){
				imp->op = FMVI;
				imp->rd_or_imm = rd;
				imp->rs1 = rs1;
			}
			break;
		case F7_IMVF:
			if(f3 != F3_RM){
				perror("invalid rm: F7_IMVF");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_IMVF){
				imp->op = IMVF;
				imp->rd_or_imm = rd;
				imp->rs1 = rs1;
			}
			break;
		case F7_FCOMPARE:
			decode_FCOMPARE(instr, imp);
			break;
		default:
			perror("invalid f7: OP_FLA");
			exit(EXIT_FAILURE);
	}
}

//floating point load
void exec_FLW(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t imm = instr.rs2_or_imm;

	int offset = (cpu->x[rs1] + imm) & ((~0) << 2);
	address_check(offset);

	//このチェックはいらない
	if(instr.op == FLW){
		float data;
	        endian_wrapper(&data, mem->data + offset, sizeof(float));
		cpu->f[rd] = (float)data;
	}
	else {
		perror("exec_FLW: invalid instruction");
		exit(EXIT_FAILURE);
	}
}

//floating point store
void exec_FSW(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;
	int32_t imm = instr.rd_or_imm;

	int offset = (cpu->x[rs1] + imm) & ((~0) << 2);
	address_check(offset);

	//このチェックはいらない
	if(instr.op == FSW){
		float data = cpu->f[rs2];
		endian_wrapper(mem->data + offset, &data, sizeof(float));
	}
	else {
		perror("exec_FSW: invalid instruction");
		exit(EXIT_FAILURE);
	}
}

//floating point sign injection
void exec_FSGNJ(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;

	int r1 = *((int *)(cpu->f + rs1));
	int r2 = *((int *)(cpu->f + rs2));

	int sign = r2 & (1 << 31);
	int data;

	switch(instr.op){
		case FSGNJ:
			data = (r1 & (~(1 << 31))) | sign;
			cpu->f[rd] = *((float *)(&data)); 
			break;
		case FSGNJX:
			data = r1 ^ sign;
			cpu->f[rd] = *((float *)(&data)); 
			break;
		case FSGNJN:
			data = (r1 | (1 << 31)) & ~sign;
			cpu->f[rd] = *((float *)(&data)); 
			break;
		default:
			perror("exec_FSGNJ: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

void exec_FROUND(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;

	switch(instr.op){
		case FROUNDRM_RNE:
			fesetround(FE_TONEAREST);
			cpu->f[rd] = nearbyint(cpu->f[rs1]);
			break;
		case FROUNDRM_RTZ:
			fesetround(FE_TOWARDZERO);
			cpu->f[rd] = nearbyint(cpu->f[rs1]);
			break;
		case FROUNDRM_RDN:
			cpu->f[rd] = floorf(cpu->f[rs1]);
			break;
		case FROUNDRM_RUP:
			cpu->f[rd] = ceilf(cpu->f[rs1]);
			break;
		case FROUNDRM_RMM:
			cpu->f[rd] = roundf(cpu->f[rs1]);
			break;
		default:
			perror("exec_FROUND: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

//convert integer to float
void exec_TOF(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;

	switch(instr.op){
		case ITOF:{
			int32_t data = cpu->x[rs1];
			cpu->f[rd] = (float)data;
			break;}
		/*case UTOF:{
			uint32_t data = cpu->x[rs1];
			cpu->f[rd] = (float)data;
			break;}*/
		default:
			perror("exec_TOF: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

//convert float to integer
void exec_TOI(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;

	switch(instr.op){
		case FTOI:{
			float data;
			fesetround(FE_TONEAREST);
			data = nearbyint(cpu->f[rs1]);
			cpu->x[rd] = (int32_t)data;
			break;}
		/*case FTOUI:{
			float data = cpu->f[rs1];
			cpu->x[rd] = (uint32_t)data;
			break;}*/
		default:
			perror("exec_TOI: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

//floating point compare
void exec_FCOMPARE(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;

	switch(instr.op){
		case FEQ:
			if(rd != 0)
				cpu->x[rd] = (cpu->f[rs1] == cpu->f[rs2])? 1 : 0;
			break;
		case FLT:
			if(rd != 0)
				cpu->x[rd] = (cpu->f[rs1] < cpu->f[rs2])? 1 : 0;
			break;
		case FLE:
			if(rd != 0)
				cpu->x[rd] = (cpu->f[rs1] <= cpu->f[rs2])? 1 : 0;
			break;
		default:
			perror("exec_FCOMPARE: inavlid instruction");
			exit(EXIT_FAILURE);
	}
}

//floating point arithmetic logic and others
void exec_FLA(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;

	if(instr.op == FADD){
		cpu->f[rd] = cpu->f[rs1] + cpu->f[rs2];	
	}
	else if(instr.op == FSUB){
		cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];	
	}
	else if(instr.op == FMUL){
		cpu->f[rd] = fmul(cpu->f[rs1], cpu->f[rs2]);	
	}
	else if(instr.op == FDIV){
		cpu->f[rd] = fdiv(cpu->f[rs1], cpu->f[rs2]);
	}
	else if(instr.op == FSQRT){
		cpu->f[rd] = fsqr(cpu->f[rs1]);
	}
	else if(FSGNJ_first <= instr.op && instr.op <= FSGNJ_last){
		exec_FSGNJ(instr, cpu, mem);
	}
	else if(FROUND_first <= instr.op && instr.op <= FROUND_last){
		exec_FROUND(instr, cpu, mem);
	}
	else if(instr.op == ITOF){
		exec_TOF(instr, cpu, mem);
	}
	else if(instr.op == FTOI){
		exec_TOI(instr, cpu, mem);
	}
	else if(instr.op == FMVI){
		if(rd != 0){
			float tmp = cpu->f[rs1];
			cpu->x[rd] = *((int *)(&tmp));
		}
	}
	else if(instr.op == IMVF){
		int tmp = cpu->x[rs1];
		cpu->f[rd] = *((float *)(&tmp));
	}
	else if(FCOMPARE_first <= instr.op && instr.op <= FCOMPARE_last){
		exec_FCOMPARE(instr, cpu, mem);
	}
	else {
		perror("exec_FLA: invalid instruction");
		exit(EXIT_FAILURE);
	}
}

//floating point arithmetic logic and others mnemonic
void mnemonic_FLA(INSTR instr, ASSEM *assem){
	
	if(instr.op == FADD){
		assem->itype = R;
		strcpy(assem->mnemonic, "fadd");
		strcpy(assem->reg, "fff");
	}
	else if(instr.op == FSUB){
		assem->itype = R;
		strcpy(assem->mnemonic, "fsub");
		strcpy(assem->reg, "fff");
	}
	else if(instr.op == FMUL){
		assem->itype = R;
		strcpy(assem->mnemonic, "fmul");
		strcpy(assem->reg, "fff");
	}
	else if(instr.op == FDIV){
		assem->itype = R;
		strcpy(assem->mnemonic, "fdiv");
		strcpy(assem->reg, "fff");
	}
	else if(instr.op == FSQRT){
		assem->itype = R_sub;
		strcpy(assem->mnemonic, "fsqrt");
		strcpy(assem->reg, "ff");
	}
	else if(FSGNJ_first <= instr.op && instr.op <= FSGNJ_last){
		assem->itype = R;
		strcpy(assem->reg, "fff");
		switch(instr.op){
			case FSGNJ:
				strcpy(assem->mnemonic, "fsgnj");
				break;
			case FSGNJX:
				strcpy(assem->mnemonic, "fsgnjx");
				break;
			case FSGNJN:
				strcpy(assem->mnemonic, "fsgnjn");
				break;
			default:
				perror("mnemonic_FSGNJ: invalid instruction");
				exit(EXIT_FAILURE);
		}
	}
	else if(FROUND_first <= instr.op && instr.op <= FROUND_last){
		assem->itype = R;
		strcpy(assem->mnemonic, "froundrm");
		strcpy(assem->reg, "ff");
		switch(instr.op){
			case FROUNDRM_RNE:
				strcpy(assem->rm, "RNE");
				break;
			case FROUNDRM_RTZ:
				strcpy(assem->rm, "RTZ");
				break;
			case FROUNDRM_RDN:
				strcpy(assem->rm, "RDN");
				break;
			case FROUNDRM_RUP:
				strcpy(assem->rm, "RUP");
				break;
			case FROUNDRM_RMM:
				strcpy(assem->rm, "RMM");
				break;
			default:
				perror("mnemonic_FROUND: invalid instruction");
				exit(EXIT_FAILURE);
		}
	}
	else if(instr.op == ITOF){
		assem->itype = R_sub;
		strcpy(assem->reg, "fx");
		strcpy(assem->mnemonic, "itof");
	}
	else if(instr.op == FTOI){
		assem->itype = R_sub;
		strcpy(assem->reg, "xf"); 
		strcpy(assem->mnemonic, "ftoi");
	}
	else if(instr.op == FMVI){
		assem->itype = R_sub;
		strcpy(assem->mnemonic, "fmvi");
		strcpy(assem->reg, "xf");
	}
	else if(instr.op == IMVF){
		assem->itype = R_sub;
		strcpy(assem->mnemonic, "imvf");
		strcpy(assem->reg, "fx");
	}
	else if(FCOMPARE_first <= instr.op && instr.op <= FCOMPARE_last){
		assem->itype = R;
		strcpy(assem->reg, "xff");
		switch(instr.op){
			case FEQ:
				strcpy(assem->mnemonic, "feq");
				break;
			case FLT:
				strcpy(assem->mnemonic, "flt");
				break;
			case FLE:
				strcpy(assem->mnemonic, "fle");
				break;
			default:
				perror("mnemonic_FCOMPARE: invalid instruction");
				exit(EXIT_FAILURE);
		}
	}
	else {
		perror("exec_FLA: invalid instruction");
		exit(EXIT_FAILURE);
	}
}



/* Reimplementation of FPU */

FREG fmul(FREG r1, FREG r2) {
	uni rs1_uni, rs2_uni, rd_uni;

	rs1_uni.f = r1;
	rs2_uni.f = r2;

	bitset<32> rs1(rs1_uni.i);
	bitset<32> rs2(rs2_uni.i);

	bool s1 = rs1[31];
	bool s2 = rs2[31];
	
	uint32_t e1 = downto(rs1.to_ulong(), 30, 23);
	uint32_t e2 = downto(rs2.to_ulong(), 30, 23);

	uint32_t m1 = downto(rs1.to_ulong(), 22, 0);
	uint32_t m2 = downto(rs2.to_ulong(), 22, 0);

	uint64_t m1a;
	uint64_t m2a;
	uint32_t e1a;
	uint32_t e2a;

	m1a = (e1 == 0) ? m1 : (0b1 << 23) + m1;
	m2a = (e2 == 0) ? m2 : (0b1 << 23) + m2;

	e1a = (e1 == 0) ? 0b1 : e1;
	e2a = (e2 == 0) ? 0b1 : e2;

	uint32_t esum;
	esum = e1a + e2a;

	bool sy = (!s1 && s2) || (s1 && !s2);

	uint32_t eyd;
	eyd = e1a + e2a + 0b10000010;

	uint32_t shifts;
	shifts = (0b01111111 <= (e1a + e2a)) ? 0b0 : (0b01111111 - (e1a + e2a));

	bitset<48> myd;
	bitset<48> myd_shifts;

	myd = m1a * m2a;
	myd_shifts = myd >> shifts;

	uint32_t se;
	se = (myd[47] == 1) ? 0 :
              (myd[46] == 1) ? 1 :
              (myd[45] == 1) ? 2 :
              (myd[44] == 1) ? 3 :
              (myd[43] == 1) ? 4 :
              (myd[42] == 1) ? 5 :
              (myd[41] == 1) ? 6 :
              (myd[40] == 1) ? 7 :
              (myd[39] == 1) ? 8 :
              (myd[38] == 1) ? 9 :
              (myd[37] == 1) ? 10 :
              (myd[36] == 1) ? 11 :
              (myd[35] == 1) ? 12 :
              (myd[34] == 1) ? 13 :
              (myd[33] == 1) ? 14 :
              (myd[32] == 1) ? 15 :
              (myd[31] == 1) ? 16 :
              (myd[30] == 1) ? 17 :
              (myd[29] == 1) ? 18 :
              (myd[28] == 1) ? 19 :
              (myd[27] == 1) ? 20 :
              (myd[26] == 1) ? 21 :
              (myd[25] == 1) ? 22 :
              (myd[24] == 1) ? 23 :
              (myd[23] == 1) ? 24 :
              (myd[22] == 1) ? 25 :
              (myd[21] == 1) ? 26 :
              (myd[20] == 1) ? 27 :
              (myd[19] == 1) ? 28 :
              (myd[18] == 1) ? 29 :
              (myd[17] == 1) ? 30 :
              (myd[16] == 1) ? 31 :
              (myd[15] == 1) ? 32 :
              (myd[14] == 1) ? 33 :
              (myd[13] == 1) ? 34 :
              (myd[12] == 1) ? 35 :
              (myd[11] == 1) ? 36 :
              (myd[10] == 1) ? 37 :
              (myd[9] == 1) ? 38 :
              (myd[8] == 1) ? 39 :
              (myd[7] == 1) ? 40 :
              (myd[6] == 1) ? 41 :
              (myd[5] == 1) ? 42 :
              (myd[4] == 1) ? 43 :
              (myd[3] == 1) ? 44 :
              (myd[2] == 1) ? 45 :
              (myd[1] == 1) ? 46 :
              (myd[0] == 1) ? 47 : 48;

	int eyf;
	uint32_t eyr;
	uint32_t ey;
	bitset<48> myf;
	bitset<23> myr;
	bitset<23> my;


	eyf = eyd - se;
	eyr = (eyf > 0) ? downto(eyf, 7, 0) : 0b0;

	myf = (eyf > 0) ? (myd << se) : (myd << (downto(eyd, 4, 0) - 1));
	myr = downto((myf >> 24).to_ulong(), 22, 0);

	bool udf = (0b010000000 > (e1a + e2a));
	bool udf_just = (0b01111111 == (e1a + e2a));

	ey = (udf_just == 0b1 && myd[47] == 0b1) ? 0b00000001 :
              (udf == 0b1) ? 0b00000000 : eyr;
	my = (udf == 0b1) ? downto((myd_shifts >> 24).to_ulong(), 22, 0) : myr;

	rd_uni.i = (((e1 == 0b0) && (m1 == 0b0)) || ((e2 == 0b0) && (m2 == 0b0))) ? (sy << 31) :   // 0をかける場合
              ((sy << 31) + (ey << 23) + my.to_ulong());

	return rd_uni.f;
}


FREG fdiv(FREG r1, FREG r2) {
	uni rs1_uni, rs2_uni, rd_uni;

	rs1_uni.f = r1;
	rs2_uni.f = r2;

	bitset<32> rs1(rs1_uni.i);
	bitset<32> rs2(rs2_uni.i);

	bool s1 = rs1[31];
	bool s2 = rs2[31];

	uint32_t e1 = downto(rs1.to_ulong(), 30, 23);
	uint32_t e2 = downto(rs2.to_ulong(), 30, 23);

	uint32_t m1 = downto(rs1.to_ulong(), 22, 0);
	uint32_t m2 = downto(rs2.to_ulong(), 22, 0);

	bool sy = (!s1 && s2) || (s1 && !s2);
	bool flg = (e2 >= 0b01111111);

	uint32_t diff;
	diff = flg ? (e2 - 0b01111111) : (0b01111111 - e2);

	uint32_t e;
	e = (downto(m2, 22, 11) == 0) ? (flg ? (0b01111111 - diff) : (0b01111111 + diff)) : 
                                    (flg ? (0b01111111 - diff - 0b1) : (0b01111111 + diff - 0b1));
															
	bool under_flg = (e == 0);

	uint32_t init[4096];
	uint64_t MODULE_BEGIN = 0b0000000000000;
	uint64_t MODULE_END   = 0b1000000000000;
	uint64_t DIVIDED = MODULE_END << 24;

	for (uint64_t i = MODULE_BEGIN; i < MODULE_END; i = i+1) {
		init[i] = (DIVIDED / (MODULE_END + i));
	}

	uint32_t m;
	m = downto(init[downto(m2, 22, 11)], 22, 0);
	
	uint32_t inv0;
	inv0 = under_flg ? ((s2 << 31) + (0b1 << 23) + m) : ((s2 << 31) + (e << 23) + m);

	uint32_t rs2_tmp;
	rs2_tmp = under_flg ? ((rs2[31] << 31) + ((downto(rs2.to_ulong(), 30, 23)-0b1) << 23) + (downto(rs2.to_ulong(), 22, 0))) : rs2.to_ulong();
	uint32_t inv1_left;
	inv1_left = (s2 << 31) + ((downto(inv0, 30, 23)+0b1) << 23) + downto(inv0, 22, 0);

	uni rs2_tmp_uni, inv0_uni, inv1_right_tmp1_uni;
	rs2_tmp_uni.i = rs2_tmp;
	inv0_uni.i = inv0;

	// fmul u1
	inv1_right_tmp1_uni.f = fmul(rs2_tmp_uni.f, inv0_uni.f);

	bitset<32> inv1_right_tmp2;
	uni inv1_right_tmp2_uni;

	// fmul u2
	inv1_right_tmp2_uni.f = fmul(inv1_right_tmp1_uni.f, inv0_uni.f);
	inv1_right_tmp2 = inv1_right_tmp2_uni.i;

	uint32_t inv1_right;
	inv1_right = (~inv1_right_tmp2[31] << 31) + downto(inv1_right_tmp2.to_ulong(), 30, 0);

	uni inv1_left_uni, inv1_right_uni, inv1_uni;
	inv1_left_uni.i = inv1_left;
	inv1_right_uni.i = inv1_right;

	// fadd u3
	inv1_uni.f = inv1_left_uni.f + inv1_right_uni.f;

	bitset<32> rdy;
	uni rdy_uni;

	// fmul u4
	rdy_uni.f = fmul(rs1_uni.f, inv1_uni.f);
	rdy = rdy_uni.i;

	rd_uni.i = ((e1 == 0) && (m1 == 0)) ? (sy << 31) : // 割られる数が0の場合
              under_flg ? 
              ((downto(rdy.to_ulong(), 30, 23) == 0) ? (rdy[31] << 31 + downto(rdy.to_ulong(), 22, 1)) : 
               (downto(rdy.to_ulong(), 30, 23) == 1) ? ((rdy[31] << 31) + (0b1 << 22) + downto(rdy.to_ulong(), 22, 1)) :
               ((rdy[31] << 31) + ((downto(rdy.to_ulong(), 30, 23)-0b1) << 23) + downto(rdy.to_ulong(), 22, 0))) 
              : rdy.to_ulong();

	return rd_uni.f;
}


FREG fsqr(FREG r1) {
	uni rs1_uni, rd_uni;
	rs1_uni.f = r1;

	uint32_t rs1 = rs1_uni.i;

	uint32_t e1 = downto(rs1, 30, 23);
	bitset<23> m1(downto(rs1, 22, 0));
	uint32_t m1s;
	m1s = m1.to_ulong() >> 1;

	bool flg = (e1 >= 0b01111111);

	bitset<8> diff;
	diff = flg ? (e1 - 0b01111111) : (0b01111111 - e1);

	bool even = (diff[0] == 0b0);

	uint32_t e;
	e = (even && m1[22] == 0b0) ? (flg ? (0b01111111 + (diff.to_ulong() >> 1)) : (0b01111111 - (diff.to_ulong() >> 1))) : 
             (even  && m1[22] == 0b1) ? (flg ? (0b01111111 + (diff.to_ulong() >> 1)) : (0b01111111 - (diff.to_ulong() >> 1))) :
             (!even && m1[22] == 0b0) ? (flg ? (0b01111111 + ((diff.to_ulong() + 0b1) >> 1) - 0b1) : (0b01111111 - ((diff.to_ulong() + 0b1) >> 1))) :
                                        (flg ? (0b01111111 + ((diff.to_ulong() + 0b1) >> 1)) : (0b01111111 - ((diff.to_ulong() - 0b1) >> 1)));

	uint32_t m;
	m = (even && m1[22] == 0b0) ? (flg ? (0b01 << 21) : (0b01 << 21)) :
             (even  && m1[22] == 0b1) ? (flg ? (0b10 << 21) : (0b10 << 21))  :
             (!even && m1[22] == 0b0) ? (flg ? (0b11 << 21) : (0b11 << 21)) :
                                         (flg ? (0b00 << 21) : (0b00 << 21));

	uint32_t rd1;
	rd1 = (e << 23) + m;

	uint32_t a;
	a = (e1 > 0b1) ? (((e1 - 0b1) << 23) + m1.to_ulong()) :
             (e1 == 0b1) ? ((0b1 << 22) + m1s) :
             m1s;

	uint32_t rd2l;
	rd2l = ((downto(rd1, 30, 23) - 0b1) << 23) + downto(rd1, 22, 0);

	uni a_uni, rd1_uni, rd2r_uni;
	a_uni.i = a;
	rd1_uni.i = rd1;

	// fdiv u1
	rd2r_uni.f = fdiv(a_uni.f, rd1_uni.f);

	uint32_t rd2;
	uni rd2l_uni, rd2_uni;
	rd2l_uni.i = rd2l;

	// fadd u2
	rd2_uni.f = rd2l_uni.f + rd2r_uni.f;
	rd2 = rd2_uni.i;

	uint32_t rd3l;
	rd3l = ((downto(rd2, 30, 23) - 0b1) << 23) + downto(rd2, 22, 0);

	uni rd3r_uni;

	// fdiv u3
	rd3r_uni.f = fdiv(a_uni.f, rd2_uni.f);

	uint32_t rd3;
	uni rd3l_uni, rd3_uni;
	rd3l_uni.i = rd3l;

	// fadd u4
	rd3_uni.f = rd3l_uni.f + rd3r_uni.f;
	rd3 = rd3_uni.i;

	uni rd4r_uni;

	// fdiv u5
	rd4r_uni.f = fdiv(a_uni.f, rd3_uni.f);

	uint32_t rd4l;
	rd4l = ((downto(rd3, 30, 23) - 0b1) << 23) + downto(rd3, 22, 0);

	uint32_t rd4;
	uni rd4l_uni, rd4_uni;
	rd4l_uni.i = rd4l;

	// fadd u6
	rd4_uni.f = rd4l_uni.f + rd4r_uni.f;
	rd4 = rd4_uni.i;

	rd_uni.i = (rs1 == 0b0) ? 0b0 : rd4;
	return rd_uni.f;

}