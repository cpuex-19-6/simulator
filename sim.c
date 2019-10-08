#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "sim.h"
#include "instruction.h"
#include "cpu.h"
#include "memory.h"
#include "disassemble.h"

uint32_t downto(uint32_t u, int n, int m){
	uint32_t ans = (u << (31-n)) >> (31- n + m);
	return ans;
}

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
		default:
			ans = 0;
	}
	return (int32_t)ans;
}

//arithmetic operation
void exec_LA(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	if(rd != 0){
	switch(f3){
		case F3_A:
			if(f7 == F7_ADD){
				cpu->x[rd] = cpu->x[rs1] + cpu->x[rs2];
			}
			else if (f7 == F7_SUB){
				cpu->x[rd] = cpu->x[rs1] - cpu->x[rs2];
			}
			else exit(EXIT_FAILURE);
			break;
		case F3_OR: 
			cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
			break;
		case F3_AND: 
			cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
			break;
		default:
			exit(EXIT_FAILURE);
	}	
	}
}

//logic and arithmetic
void exec_LAI(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t imm = immediate(instr, I);

	if(rd != 0){
	switch(f3){
		case F3_ADDI: 
			cpu->x[rd] = cpu->x[rs1] + imm;
			break;
		case F3_ORI: 
			cpu->x[rd] = cpu->x[rs1] | imm;
			break;
		case F3_ANDI:
			cpu->x[rd] = cpu->x[rs1] & imm;
			break;
		default:
			exit(EXIT_FAILURE);
	}
	}
}

//load
void exec_LD(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t imm = immediate(instr, I);

	if(rd != 0){
	switch(f3){
		case F3_LB:{
			int8_t data;
		        memcpy(&data, mem->data + rs1 + imm, sizeof(int8_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case F3_LH:{
			int16_t data;
		        memcpy(&data, mem->data + rs1 + imm, sizeof(int16_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case F3_LW:{
			int32_t data;
		        memcpy(&data, mem->data + rs1 + imm, sizeof(int32_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case F3_LBU:{
			uint8_t data;
		        memcpy(&data, mem->data + rs1 + imm, sizeof(uint8_t));
			cpu->x[rd] = (uint32_t)data;
			break;}
		case F3_LHU:{
			uint16_t data;
		        memcpy(&data, mem->data + rs1 + imm, sizeof(uint16_t));
			cpu->x[rd] = (uint32_t)data;
			break;}
		default:
			exit(EXIT_FAILURE);
	}
	}
}

//store 間違っているかも
void exec_ST(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int32_t imm = immediate(instr, S);

	//これでちゃんと下位ビットを取れているのか
	switch(f3){
		case F3_SB:{
			int8_t data = cpu->x[rs2];
			memcpy(mem->data + rs1 + imm, &data, sizeof(int8_t));
			break;}
		case F3_SH:{
			int16_t data = cpu->x[rs2];
			memcpy(mem->data + rs1 + imm, &data, sizeof(int16_t));
			break;}
		case F3_SW:{
			int32_t data = cpu->x[rs2];
			memcpy(mem->data + rs1 + imm, &data, sizeof(int32_t));
			break;}
		default:
			exit(EXIT_FAILURE);
	}
}

//conditional branch
void exec_CB(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);

	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	int32_t imm = immediate(instr, B);

	switch(f3){
		case F3_BEQ:
			if(cpu->x[rs1] == cpu->x[rs2]){
				cpu->pc += imm/4;//pc = pc+imm
			}
			else cpu->pc += 1;// pc = pc+4;
			break;
		case F3_BNE:
			if(cpu->x[rs1] != cpu->x[rs2]){
				cpu->pc += imm/4;//pc = pc+imm
			}
			else cpu->pc += 1;// pc = pc+4;
			break;
		case F3_BLT:
			if(cpu->x[rs1] < cpu->x[rs2]){
				cpu->pc += imm/4;//pc = pc+imm
			}
			else cpu->pc += 1;// pc = pc+4;
			break;
		case F3_BGE:
			if(cpu->x[rs1] >= cpu->x[rs2]){
				cpu->pc += imm/4;//pc = pc+imm
			}
			else cpu->pc += 1;// pc = pc+4;
			break;
		case F3_BLTU:
			if((uint32_t)cpu->x[rs1] < (uint32_t)cpu->x[rs2]){
				cpu->pc += imm/4;//pc = pc+imm
			}
			else cpu->pc += 1;// pc = pc+4;
			break;
		case F3_BGEU:
			if((uint32_t)cpu->x[rs1] >= (uint32_t)cpu->x[rs2]){
				cpu->pc += imm/4;//pc = pc+imm
			}
			else cpu->pc += 1;// pc = pc+4;
			break;
		default:
			exit(EXIT_FAILURE);
	}
}

//rd がゼロレジスタの時は代入させておいて最後に代入がいい?
void exec_instr(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t opcd = downto(instr, 6, 0);
	
	switch(opcd){
		case OP_LUI:{
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, U);
			if(rd != 0)cpu->x[rd] = imm;
			cpu->pc += 1;// pc = pc+4;
			break;}
		case OP_LA: 
			exec_LA(instr, cpu, mem);
			cpu->pc += 1;// pc = pc+4;
			break;
		case OP_LAI:
			exec_LAI(instr, cpu, mem);
			cpu->pc += 1;// pc = pc+4;
			break;
		case OP_LD: 
			exec_LD(instr, cpu, mem);
			cpu->pc += 1;// pc = pc+4;
			break;
		case OP_ST: 
			exec_ST(instr, cpu, mem);
			cpu->pc += 1;// pc = pc+4;
			break;
		case OP_AUIPC:{
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, U);
			if(rd != 0)cpu->x[rd] = cpu->pc + imm/4; //pc = pc+imm
			break;}
		case OP_JAL: {
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, J);
			if(rd != 0)cpu->x[rd] = cpu->pc + 1;// pc = pc+4;
			cpu->pc += imm/4;
			break;}
		case OP_JALR: {
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t rs1 = (int32_t)downto(instr, 19, 15);
			int32_t imm = immediate(instr, I);
			if(rd != 0)cpu->x[rd] = cpu->pc + 1; // pc = pc+4;
			cpu->pc = cpu->x[rs1] + imm/4;
			break;}
		case OP_CB: 
			exec_CB(instr, cpu, mem);
			break;
		default: 
			putchar('\n');
			printf(" 0b");
			print_binary(instr);
			perror("unkown instruction");
			exit(EXIT_FAILURE);
	}	
}

uint32_t fetch(CPU *cpu, MEMORY *mem){
	uint32_t next_instr;
	uint32_t addr_instr = cpu->pc;

	next_instr = mem->instr[addr_instr];

	return next_instr;
}

void run_to_the_end(CPU *cpu, MEMORY *mem){
	uint32_t instr = fetch(cpu, mem);
	
	while(instr != 0){
		exec_instr(instr, cpu, mem);
		instr = fetch(cpu, mem);
	}
}

int step(CPU *cpu, MEMORY *mem, OPTION option){
	uint32_t instr = fetch(cpu, mem);

	if(instr != 0){
		putchar('\n');
		printf("pc: %d:", cpu->pc);
		exec_instr(instr, cpu, mem);

		ASSEM assem;
		//assem_init(&assem);
		disassem_instr(instr, &assem);
		printf(" 0b");
		print_binary(instr);
		printf(": ");
		print_assembly(assem);
		putchar('\n');
		putchar('\n');

		if(option.reg != 0)print_reg(option.reg, *cpu);

		return 0;
	}
	else return 1;
}
