#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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

	if(f3 == F3_FLW){
		float data;
	        endian_wrapper(&data, mem->data + rs1 + imm, sizeof(float));
		cpu->f[rd] = (float)data;
	}
	else exit(EXIT_FAILURE);
}

//floating point store
void exec_FSW(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int32_t imm = immediate(instr, S);

	if(f3 == F3_FSW){
		float data = cpu->f[rs2];
		endian_wrapper(mem->data + rs1 + imm, &data, sizeof(float));
	}
	else exit(EXIT_FAILURE);
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
			exit(EXIT_FAILURE);
	}
}

//convert integer to float
void exec_TOF(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);

	if(f3 != F3_RM)exit(EXIT_FAILURE);

	switch(rs1){
		case RS2_ITOF:{
			int32_t data = cpu->x[rs1];
			cpu->f[rd] = (float)data;
			break;}
		case RS2_UTOF:{
			uint32_t data = cpu->x[rs1];
			cpu->f[rd] = (float)data;
			break;}
		default:
			exit(EXIT_FAILURE);
	}
}

//convert float to integer
void exec_TOI(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);

	if(f3 != F3_RM)exit(EXIT_FAILURE);

	switch(rs1){
		case RS2_FTOI:{
			float data = cpu->f[rs1];
			cpu->x[rd] = (int32_t)data;
			break;}
		case RS2_FTOUI:{
			float data = cpu->f[rs1];
			cpu->x[rd] = (uint32_t)data;
			break;}
		default:
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
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			cpu->f[rd] = cpu->f[rs1] + cpu->f[rs2];	
			break;
		case F7_FSUB:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			cpu->f[rd] = cpu->f[rs1] - cpu->f[rs2];	
			break;
		case F7_FMUL:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			cpu->f[rd] = cpu->f[rs1] * cpu->f[rs2];	
			break;
		case F7_FDIV:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			cpu->f[rd] = cpu->f[rs1] / cpu->f[rs2];	
			break;
		case F7_FSQRT:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			if(rs2 == RS2_FSQRT){
				double tmp = (double)cpu->f[rs1];
				cpu->f[rd] = (float)sqrt(tmp);
			}
			else exit(EXIT_FAILURE);
			break;
		case F7_FSGNJ:
			exec_FSGNJ(instr, cpu, mem);
			break;
		case F7_TOF:
			exec_TOF(instr, cpu, mem);
			break;
		case F7_TOI:
			exec_TOI(instr, cpu, mem);
			break;
		case F7_FMVI:
			if(f3 != F3_FMVI)exit(EXIT_FAILURE);
			if(rs2 == RS2_FMVI){
				int tmp = cpu->x[rs1];
				cpu->f[rd] = *((float *)(&tmp));
			}
			break;
		case F7_IMVF:
			if(f3 != F3_IMVF)exit(EXIT_FAILURE);
			if(rs2 == RS2_IMVF){
				if(rd != 0){
					float tmp = cpu->f[rs1];
					cpu->x[rd] = *((int *)(&tmp));
				}
			}
			break;
		case F7_FCOMPARE:
			exec_FCOMPARE(instr, cpu, mem);
			break;
		default:
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
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fadd");
			}
			break;
		case F7_FSUB:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fsub");
			}
			break;
		case F7_FMUL:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fmul");
			}
			break;
		case F7_FDIV:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			else{
				assem->itype = R;
				strcpy(assem->mnemonic, "fdiv");
			}
			break;
		case F7_FSQRT:
			if(f3 != F3_RM)exit(EXIT_FAILURE);
			if(rs2 == RS2_FSQRT){
				assem->itype = R_sub;
				strcpy(assem->mnemonic, "fsqrt");
			}
			else exit(EXIT_FAILURE);
			break;
		case F7_FSGNJ:
			assem->itype = R;
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
					exit(EXIT_FAILURE);
			}
			break;
		case F7_TOF:
			assem->itype = R_sub;
			switch(rs2){
				case RS2_ITOF:
					strcpy(assem->mnemonic, "itof");
					break;
				case RS2_UTOF:
					strcpy(assem->mnemonic, "utof");
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		case F7_TOI:
			assem->itype = R_sub;
			switch(rs2){
				case RS2_FTOI:
					strcpy(assem->mnemonic, "ftoi");
					break;
				case RS2_FTOUI:
					strcpy(assem->mnemonic, "ftoui");
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		case F7_FMVI:
			if(f3 != F3_FMVI)exit(EXIT_FAILURE);
			if(rs2 == RS2_FMVI){
				assem->itype = R_sub;
				strcpy(assem->mnemonic, "fmvi");
			}
			break;
		case F7_IMVF:
			if(f3 != F3_IMVF)exit(EXIT_FAILURE);
			if(rs2 == RS2_IMVF){
				assem->itype = R_sub;
				strcpy(assem->mnemonic, "imvf");
			}
			break;
		case F7_FCOMPARE:
			assem->itype = R;
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
					exit(EXIT_FAILURE);
			}
			break;
		default:
			exit(EXIT_FAILURE);
	}
}

