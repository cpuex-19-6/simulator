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
		cpu->f[rd] = cpu->f[rs1] * cpu->f[rs2];	
	}
	else if(instr.op == FDIV){
		cpu->f[rd] = fdiv(cpu->f[rs1], cpu->f[rs2]);
	}
	else if(instr.op == FSQRT){
		cpu->f[rd] = sqrtf(cpu->f[rs1]);
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

FREG fdiv(FREG r1, FREG r2) {
	uni rs1_uni, rs2_uni, rd_uni;

	rs1_uni.f = r1;
	rs2_uni.f = r2;

	bitset<32> rs1(rs1_uni.i);
	bitset<32> rs2(rs2_uni.i);

	bool s1 = rs1[31];
	bool s2 = rs2[31];

	bitset<8> e1(downto(rs1.to_ulong(), 30, 23));
	bitset<8> e2(downto(rs2.to_ulong(), 30, 23));

	bitset<23> m1(downto(rs1.to_ulong(), 22, 0));
	bitset<23> m2(downto(rs2.to_ulong(), 22, 0));

	bool sy = (!s1 && s2) || (s1 && !s2);
	bool flg = (e2.to_ulong() >= 0b01111111);

	bitset<8> diff;
	diff = flg ? (e2.to_ulong() - 0b01111111) : (0b01111111 - e2.to_ulong());

	bitset<8> e;
	e = (downto(m2.to_ulong(), 22, 11) == 0) ? (flg ? (0b01111111 - diff.to_ulong()) : (0b01111111 + diff.to_ulong())) : 
                                    (flg ? (0b01111111 - diff.to_ulong() - 0b1) : (0b01111111 + diff.to_ulong() - 0b1));
															
	bool under_flg = (e.to_ulong() == 0);

	bitset<24> init[4096];
	uint64_t MODULE_BEGIN = 0b0000000000000;
	uint64_t MODULE_END   = 0b1000000000000;
	uint64_t DIVIDED = MODULE_END << 24;

	for (uint64_t i = MODULE_BEGIN; i < MODULE_END; i = i+1) {
		init[i] = (DIVIDED / (MODULE_END + i));
	}

	bitset<23> m;
	m = downto(init[downto(m2.to_ulong(), 22, 11)].to_ulong(), 22, 0);
	
	bitset<32> inv0;
	inv0 = under_flg ? ((s2 << 31) + (0b1 << 23) + m.to_ulong()) : ((s2 << 31) + (e.to_ulong() << 23) + m.to_ulong());

	bitset<32> rs2_tmp;
	rs2_tmp = under_flg ? ((rs2[31] << 31) + ((downto(rs2.to_ulong(), 30, 23)-0b1) << 23) + (downto(rs2.to_ulong(), 22, 0))) : rs2;
	bitset<32> inv1_left;
	inv1_left = (s2 << 31) + ((downto(inv0.to_ulong(), 30, 23)+0b1) << 23) + downto(inv0.to_ulong(), 22, 0);

	bitset<32> inv1_right_tmp1;
	uni rs2_tmp_uni, inv0_uni, inv1_right_tmp1_uni;
	rs2_tmp_uni.i = rs2_tmp.to_ulong();
	inv0_uni.i = inv0.to_ulong();

	// fmul u1
	inv1_right_tmp1_uni.f = rs2_tmp_uni.f * inv0_uni.f;
	inv1_right_tmp1 = inv1_right_tmp1_uni.i;

	bitset<32> inv1_right_tmp2;
	uni inv1_right_tmp2_uni;

	// fmul u2
	inv1_right_tmp2_uni.f = inv1_right_tmp1_uni.f * inv0_uni.f;
	inv1_right_tmp2 = inv1_right_tmp2_uni.i;

	bitset<32> inv1_right;
	inv1_right = (~inv1_right_tmp2[31] << 31) + downto(inv1_right_tmp2.to_ulong(), 30, 0);

	bitset<32> inv1;
	uni inv1_left_uni, inv1_right_uni, inv1_uni;
	inv1_left_uni.i = inv1_left.to_ulong();
	inv1_right_uni.i = inv1_right.to_ulong();

	// fadd u3
	inv1_uni.f = inv1_left_uni.f + inv1_right_uni.f;
	inv1 = inv1_uni.i;

	bitset<32> rdy;
	uni rdy_uni;

	// fmul u4
	rdy_uni.f = rs1_uni.f + inv1_uni.f;
	rdy = rdy_uni.i;

	rd_uni.i = ((e1.to_ulong() == 0) && (m1.to_ulong() == 0)) ? (sy << 31) : // 割られる数が0の場合
              under_flg ? 
              ((downto(rdy.to_ulong(), 30, 23) == 0) ? (rdy[31] << 31 + downto(rdy.to_ulong(), 22, 1)) : 
               (downto(rdy.to_ulong(), 30, 23) == 1) ? ((rdy[31] << 31) + (0b1 << 22) + downto(rdy.to_ulong(), 22, 1)) :
               ((rdy[31] << 31) + ((downto(rdy.to_ulong(), 30, 23)-0b1) << 23) + downto(rdy.to_ulong(), 22, 0))) 
              : rdy.to_ulong();

	return rd_uni.f;
}