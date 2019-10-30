#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <fenv.h>

#include "functions.h"
#include "cpu.h"
#include "memory.h"
#include "sim.h"
#include "disassemble.h"
#include "fpu.h"

//floating point load
void exec_FLW(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t imm = immediate(instr, I);

	int offset = (cpu->x[rs1] + imm) & ((~0) << 2);

	if(f3 == F3_FLW){
		float data;
	        endian_wrapper(&data, mem->data + offset, sizeof(float));
		cpu->f[rd] = (float)data;
	}
	else {
		perror("invalid f3: OP_FLW");
		exit(EXIT_FAILURE);
	}
}

//floating point store
void exec_FSW(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int32_t imm = immediate(instr, S);

	int offset = (cpu->x[rs1] + imm) & ((~0) << 2);

	if(f3 == F3_FSW){
		float data = cpu->f[rs2];
		endian_wrapper(mem->data + offset, &data, sizeof(float));
	}
	else {
		perror("invalid f3: OP_FSW");
		exit(EXIT_FAILURE);
	}
}

//floating point sign injection
void exec_FSGNJ(int instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int r1 = *((int *)(cpu->f + rs1));
	int r2 = *((int *)(cpu->f + rs2));

	int sign = r2 & (1 << 31);
	int data;

	switch(f3){
		case F3_FSGNJ:
			data = (r1 & (~(1 << 31))) | sign;
			cpu->f[rd] = *((float *)(&data)); 
			break;
		case F3_FSGNJX:
			data = r1 ^ sign;
			cpu->f[rd] = *((float *)(&data)); 
			break;
		case F3_FSGNJN:
			data = (r1 | (1 << 31)) & ~sign;
			cpu->f[rd] = *((float *)(&data)); 
			break;
		default:
			perror("invalid f3: F7_FSGNJ");
			exit(EXIT_FAILURE);
	}
}

void exec_FROUND(int instr, CPU *cpu, MEMORY *mem){
	uint32_t rm = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);

	switch(rm){
		case RM_RNE:
			fesetround(FE_TONEAREST);
			cpu->f[rd] = nearbyint(cpu->f[rs1]);
			break;
		case RM_RTZ:
			fesetround(FE_TOWARDZERO);
			cpu->f[rd] = nearbyint(cpu->f[rs1]);
			break;
			break;
		case RM_RDN:
			cpu->f[rd] = floorf(cpu->f[rs1]);
			break;
		case RM_RUP:
			cpu->f[rd] = ceilf(cpu->f[rs1]);
			break;
		case RM_RMM:
			cpu->f[rd] = roundf(cpu->f[rs1]);
			break;
		default:
			perror("invalid rm: F7_FROUND");
			exit(EXIT_FAILURE);
	}
}

//convert integer to float
void exec_TOF(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	if(f3 != F3_RM){
		perror("invalid rm: F7_TOF");
		exit(EXIT_FAILURE);
	}

	switch(rs2){
		case RS2_ITOF:{
			int32_t data = cpu->x[rs1];
			cpu->f[rd] = (float)data;
			break;}
		case RS2_UTOF:{
			uint32_t data = cpu->x[rs1];
			cpu->f[rd] = (float)data;
			break;}
		default:
			perror("invalid rs2: F7_TOF");
			exit(EXIT_FAILURE);
	}
}

//convert float to integer
void exec_TOI(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	if(f3 != F3_RM){
		perror("invalid rm: F7_TOI");
		exit(EXIT_FAILURE);
	}

	switch(rs2){
		case RS2_FTOI:{
			float data = cpu->f[rs1];
			cpu->x[rd] = (int32_t)data;
			break;}
		case RS2_FTOUI:{
			float data = cpu->f[rs1];
			cpu->x[rd] = (uint32_t)data;
			break;}
		default:
			perror("invalid rs2: F7_TOI");
			exit(EXIT_FAILURE);
	}
}

//floating point compare
void exec_FCOMPARE(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	switch(f3){
		case F3_FEQ:
			if(rd != 0)
				cpu->x[rd] = (cpu->f[rs1] == cpu->f[rs2])? 1 : 0;
			break;
		case F3_FLT:
			if(rd != 0)
				cpu->x[rd] = (cpu->f[rs1] < cpu->f[rs2])? 1 : 0;
			break;
		case F3_FLE:
			if(rd != 0)
				cpu->x[rd] = (cpu->f[rs1] <= cpu->f[rs2])? 1 : 0;
			break;
		default:
			perror("inavlid f3: F7_FCOMPARE");
			exit(EXIT_FAILURE);
	}
}

//floating point arithmetic logic and others
void exec_FLA(uint32_t instr, CPU *cpu, MEMORY *mem){
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
			cpu->f[rd] = cpu->f[rs1] + cpu->f[rs2];	
			break;
		case F7_FSUB:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FSUB");
				exit(EXIT_FAILURE);
			}
			cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];	
			break;
		case F7_FMUL:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FMUL");
				exit(EXIT_FAILURE);
			}
			cpu->f[rd] = cpu->f[rs1] * cpu->f[rs2];	
			break;
		case F7_FDIV:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FDIV");
				exit(EXIT_FAILURE);
			}
			cpu->f[rd] = cpu->f[rs1] / cpu->f[rs2];	
			break;
		case F7_FSQRT:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FSQRT");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_FSQRT){
				cpu->f[rd] = sqrtf(cpu->f[rs1]);
			}
			else {
				perror("invalid rs2: F7_FSQRT");
				exit(EXIT_FAILURE);
			}
			break;
		case F7_FSGNJ:
			exec_FSGNJ(instr, cpu, mem);
			break;
		case F7_FROUND:
			exec_FROUND(instr, cpu, mem);
			break;
		case F7_TOF:
			exec_TOF(instr, cpu, mem);
			break;
		case F7_TOI:
			exec_TOI(instr, cpu, mem);
			break;
		case F7_FMVI:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FMVI");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_FMVI){
				if(rd != 0){
					float tmp = cpu->f[rs1];
					cpu->x[rd] = *((int *)(&tmp));
				}
			}
			break;
		case F7_IMVF:
			if(f3 != F3_RM){
				perror("invalid rm: F7_IMVF");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_IMVF){
				int tmp = cpu->x[rs1];
				cpu->f[rd] = *((float *)(&tmp));
			}
			break;
		case F7_FCOMPARE:
			exec_FCOMPARE(instr, cpu, mem);
			break;
		default:
			perror("invalid f7: OP_FLA");
			exit(EXIT_FAILURE);
	}
}

//floating point arithmetic logic and others mnemonic
void mnemonic_FLA(uint32_t instr, ASSEM *assem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	switch(f7){
		case F7_FADD:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FADD");
				exit(EXIT_FAILURE);
			}
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fadd");
				strcpy(assem->reg, "fff");
			}
			break;
		case F7_FSUB:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FSUB");
				exit(EXIT_FAILURE);
			}
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fsub");
				strcpy(assem->reg, "fff");
			}
			break;
		case F7_FMUL:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FMUL");
				exit(EXIT_FAILURE);
			}
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fmul");
				strcpy(assem->reg, "fff");
			}
			break;
		case F7_FDIV:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FDIV");
				exit(EXIT_FAILURE);
			}
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fdiv");
				strcpy(assem->reg, "fff");
			}
			break;
		case F7_FSQRT:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FSQRT");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_FSQRT){
				assem->itype = R_sub;
				strcpy(assem->mnemonic, "fsqrt");
				strcpy(assem->reg, "ff");
			}
			else exit(EXIT_FAILURE);
			break;
		case F7_FSGNJ:
			assem->itype = R;
			strcpy(assem->reg, "fff");
			switch(f3){
				case F3_FSGNJ:
					strcpy(assem->mnemonic, "fsgnj");
					break;
				case F3_FSGNJX:
					strcpy(assem->mnemonic, "fsgnjx");
					break;
				case F3_FSGNJN:
					strcpy(assem->mnemonic, "fsgnjn");
					break;
				default:
					perror("invalid f3: F7_FSGNJ");
					exit(EXIT_FAILURE);
			}
			break;
		case F7_FROUND:
			if(rs2 == RS2_FROUND){
				switch(f3){
					case RM_RNE:
					strcpy(assem->rm, "RNE");
						break;
					case RM_RTZ:
					strcpy(assem->rm, "RTZ");
						break;
					case RM_RDN:
					strcpy(assem->rm, "RDN");
						break;
					case RM_RUP:
					strcpy(assem->rm, "RUP");
						break;
					case RM_RMM:
					strcpy(assem->rm, "RMM");
						break;
					default:
						perror("invalid rm: F7_FSGNJ");
						exit(EXIT_FAILURE);
				}
				assem->itype = R;
				strcpy(assem->mnemonic, "froundrm");
				strcpy(assem->reg, "ff");
			}
		case F7_TOF:
			assem->itype = R_sub;
			strcpy(assem->reg, "fx");
			switch(rs2){
				case RS2_ITOF:
					strcpy(assem->mnemonic, "itof");
					break;
				case RS2_UTOF:
					strcpy(assem->mnemonic, "utof");
					break;
				default:
					perror("invalid rs2: F7_TOF");
					exit(EXIT_FAILURE);
			}
			break;
		case F7_TOI:
			assem->itype = R_sub;
			strcpy(assem->reg, "xf");
			switch(rs2){
				case RS2_FTOI:
					strcpy(assem->mnemonic, "ftoi");
					break;
				case RS2_FTOUI:
					strcpy(assem->mnemonic, "ftoui");
					break;
				default:
					perror("invalid rs2: F7_TOI");
					exit(EXIT_FAILURE);
			}
			break;
		case F7_FMVI:
			if(f3 != F3_RM){
				perror("invalid rm: F7_FMVI");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_FMVI){
				assem->itype = R_sub;
				strcpy(assem->mnemonic, "fmvi");
				strcpy(assem->reg, "xf");
			}
			else {
				perror("invalid f3: OP_FMVI");
				exit(EXIT_FAILURE);
			}
			break;
		case F7_IMVF:
			if(f3 != F3_RM){
				perror("invalid rm: F7_IMVF");
				exit(EXIT_FAILURE);
			}
			if(rs2 == RS2_IMVF){
				assem->itype = R_sub;
				strcpy(assem->mnemonic, "imvf");
				strcpy(assem->reg, "fx");
			}
			else {
				perror("invalid f3: OP_FMVI");
				exit(EXIT_FAILURE);
			}
			break;
		case F7_FCOMPARE:
			assem->itype = R;
			strcpy(assem->reg, "xff");
			switch(f3){
				case F3_FEQ:
					strcpy(assem->mnemonic, "feq");
					break;
				case F3_FLT:
					strcpy(assem->mnemonic, "flt");
					break;
				case F3_FLE:
					strcpy(assem->mnemonic, "fle");
					break;
				default:
					perror("invalid f3: F7_FCOMPARE");
					exit(EXIT_FAILURE);
			}
			break;
		default:
			perror("invalid f7: OP_FLA");
			exit(EXIT_FAILURE);
	}
}

