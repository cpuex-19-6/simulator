#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


#include "functions.h"
#include "sim.h"
#include "instruction.h"
#include "cpu.h"
#include "memory.h"
#include "fpu.h"
#include "disassemble.h"
#include "option.h"
#include "io.h"
#include "analyse.h"

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
void exec_LA(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

	int32_t rd = (int32_t)downto(instr, 11, 7);
	int32_t rs1 = (int32_t)downto(instr, 19, 15);
	int32_t rs2 = (int32_t)downto(instr, 24, 20);

	if(rd != 0){
	switch(f3){
		case F3_A:
			switch(f7){
				case F7_ADD:
					cpu->x[rd] = cpu->x[rs1] + cpu->x[rs2];
					break;
				case F7_SUB:
					cpu->x[rd] = cpu->x[rs1] - cpu->x[rs2];
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		case F3_OR: 
			switch(f7){
				case F7_OR:
					cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
					break;
				case F7_REM:
					cpu->x[rd] = (int32_t)cpu->x[rs1] % (int32_t)cpu->x[rs2];
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		case F3_AND: 
			switch(f7){
				case F7_AND:
					cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
					break;
				case F7_REMU:
					cpu->x[rd] = (uint32_t)cpu->x[rs1] % (uint32_t)cpu->x[rs2];
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		case F3_XOR:
			switch(f7){
				case F7_XOR:
					cpu->x[rd] = cpu->x[rs1] ^ cpu->x[rs2];
					break;
				case F7_DIV:
					cpu->x[rd] = (int32_t)cpu->x[rs1] / (int32_t)cpu->x[rs2];
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		case F3_SL:
			if(f7 == F7_SLL){
				cpu->x[rd] = (unsigned int)cpu->x[rs1] << (unsigned int)(cpu->x[rs2] % 32);
			}
			else exit(EXIT_FAILURE);
			break;
		case F3_SR:
			switch(f7){
				case F7_DIVU:
					cpu->x[rd] = (uint32_t)cpu->x[rs1] / (uint32_t)cpu->x[rs2];
					break;
				case F7_SRL:
					cpu->x[rd] = (unsigned int)cpu->x[rs1] >> (unsigned int)(cpu->x[rs2] % 32);
					break;
				case F7_SRA:
					cpu->x[rd] = (int)cpu->x[rs1] >> (unsigned int)(cpu->x[rs2] % 32);
					break;
				default:
					exit(EXIT_FAILURE);
			}
			break;
		default:
			exit(EXIT_FAILURE);
	}	
	}
}

//logic and arithmetic
void exec_LAI(uint32_t instr, CPU *cpu, MEMORY *mem){
	uint32_t f3 = downto(instr, 14, 12);
	uint32_t f7 = downto(instr, 31, 25);

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
		case F3_XORI:
			cpu->x[rd] = cpu->x[rs1] ^ imm;
			break;
		case F3_SLI:
			if(f7 == F7_SLLI){
				cpu->x[rd] = (unsigned int)cpu->x[rs1] << (imm % 32); 
			}
			else exit(EXIT_FAILURE);
			break;
		case F3_SRI:
			if(f7 == F7_SRLI){
				cpu->x[rd] = (unsigned int)cpu->x[rs1] >> (imm % 32); 

			}
			else if(f7 == F7_SRAI){
				cpu->x[rd] = (int)cpu->x[rs1] >> (imm % 32); 
			}
			else exit(EXIT_FAILURE);
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

	int offset;

	if(rd != 0){
	switch(f3){
		case F3_LB:{
			offset = (cpu->x[rs1] + imm);
			int8_t data;
		        endian_wrapper(&data, mem->data + offset, sizeof(int8_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case F3_LH:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 1);
			int16_t data;
		        endian_wrapper(&data, mem->data + offset, sizeof(int16_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case F3_LW:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 2);
			int32_t data;
		        endian_wrapper(&data, mem->data + offset, sizeof(int32_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case F3_LBU:{
			offset = (cpu->x[rs1] + imm);
			uint8_t data;
		        endian_wrapper(&data, mem->data + offset, sizeof(uint8_t));
			cpu->x[rd] = (uint32_t)data;
			break;}
		case F3_LHU:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 1);
			uint16_t data;
		        endian_wrapper (&data, mem->data + offset, sizeof(uint16_t));
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

	int offset;

	//これでちゃんと下位ビットを取れているのか
	switch(f3){
		case F3_SB:{
			offset = (cpu->x[rs1] + imm);
			int8_t data = cpu->x[rs2];
			endian_wrapper(mem->data + offset, &data, sizeof(int8_t));
			break;}
		case F3_SH:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 1);
			int16_t data = cpu->x[rs2];
			endian_wrapper(mem->data + offset, &data, sizeof(int16_t));
			break;}
		case F3_SW:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 2);
			int32_t data = cpu->x[rs2];
			endian_wrapper(mem->data + offset, &data, sizeof(int32_t));
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
	imm = imm & ((~0) << 2); //immediate wrapping

	switch(f3){
		case F3_BEQ:
			if(cpu->x[rs1] == cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case F3_BNE:
			if(cpu->x[rs1] != cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case F3_BLT:
			if(cpu->x[rs1] < cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case F3_BGE:
			if(cpu->x[rs1] >= cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case F3_BLTU:
			if((uint32_t)cpu->x[rs1] < (uint32_t)cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case F3_BGEU:
			if((uint32_t)cpu->x[rs1] >= (uint32_t)cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		default:
			exit(EXIT_FAILURE);
	}
}

//rd がゼロレジスタの時は代入させておいて最後に代入がいい?
void exec_instr(uint32_t instr, CPU *cpu, MEMORY *mem, IO *io, STATE *state){
	state->instr_num++;

	uint32_t opcd = downto(instr, 6, 0);
	
	switch(opcd){
		case OP_LUI:{
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, U);
			if(rd != 0)cpu->x[rd] = imm;
			cpu->pc += 4;// pc = pc+4;
			break;}
		case OP_LA: 
			exec_LA(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_LAI:
			exec_LAI(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_LD: 
			exec_LD(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_ST: 
			exec_ST(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_AUIPC:{
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, U);
			imm = imm & ((~0) << 2); // immediate wrapping
			if(rd != 0)cpu->x[rd] = cpu->pc + imm; //rd = pc+imm
			cpu->pc += 4;// pc = pc+4;
			break;}
		case OP_JAL: {
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t imm = immediate(instr, J);
			imm = imm & ((~0) << 2); // immediate wrapping
			if(rd != 0)cpu->x[rd] = cpu->pc + 4;// rd = pc+4;
			cpu->pc += imm; //pc = pc + imm
			break;}
		case OP_JALR: {
			int32_t rd = (int32_t)downto(instr, 11, 7);
			int32_t rs1 = (int32_t)downto(instr, 19, 15);
			int32_t imm = immediate(instr, I);
			if(rd != 0)cpu->x[rd] = cpu->pc + 4; // rd = pc+4;
			int address = (cpu->x[rs1] + imm) & ((~0) << 2);
			cpu->pc = address; //pc = rs1 + imm
			break;}
		case OP_CB: 
			exec_CB(instr, cpu, mem);
			break;
		case OP_FLW:
			exec_FLW(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_FSW:
			exec_FSW(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_FLA:
			exec_FLA(instr, cpu, mem);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_IN:
			exec_IN(instr, cpu, mem, io);
			cpu->pc += 4;// pc = pc+4;
			break;
		case OP_OUT:
			exec_OUT(instr, cpu, mem, io);
			cpu->pc += 4;// pc = pc+4;
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

	endian_wrapper(&next_instr, (mem->instr + addr_instr), sizeof(uint32_t));

	return next_instr;
}

int run_to_the_end(CPU *cpu, MEMORY *mem, IO *io, OPTION option, STATE *state){
	uint32_t instr = fetch(cpu, mem);
	int bp;
	
	while(instr != 0){
		exec_instr(instr, cpu, mem, io, state);
		if((option.to_the_end == 0 && (bp = bp_check(*cpu, option.breakpoint)))){
			putchar('\n');
			printf("stopped at pc: %d\n", bp);
			putchar('\n');
			return 0;
		}
		instr = fetch(cpu, mem);
	}
	return 1;
}

int step(CPU *cpu, MEMORY *mem, IO *io, OPTION *option, STATE *state){
	uint32_t instr = fetch(cpu, mem);

	if(instr != 0){
		putchar('\n');
		printf("pc: %d:", cpu->pc);
		exec_instr(instr, cpu, mem, io, state);

		ASSEM assem;
		//assem_init(&assem);
		disassem_instr(instr, &assem);
		printf(" 0b");
		print_binary(instr);
		printf(": ");
		print_assembly(assem);
		putchar('\n');
		putchar('\n');

		if(option->reg != 0)print_reg(*option, *cpu);

		if(option->freg != 0)print_freg(*option, *cpu);

		if(option->mem_print.num != 0)print_mem(option->mem_print, *mem);

		if(option->step_n > 0){
			option->step_n--;
			return step(cpu, mem, io, option, state);
		}
		else return 0;
	}
	else return 1;
}
