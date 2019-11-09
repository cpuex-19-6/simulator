#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


#include "struct.h"
#include "functions.h"
#include "instruction.h"
#include "sim.h"
#include "memory.h"
#include "fpu.h"
#include "disassemble.h"
#include "option.h"
#include "io.h"
#include "analyse.h"
#include "decode.h"

//arithmetic operation
void exec_LA(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;

	if(rd != 0){
	switch(instr.op){
		case ADD:
			cpu->x[rd] = cpu->x[rs1] + cpu->x[rs2];
			break;
		case SUB:
			cpu->x[rd] = cpu->x[rs1] - cpu->x[rs2];
			break;
		case OR: 
			cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
			break;
		case REM:
			cpu->x[rd] = (int32_t)cpu->x[rs1] % (int32_t)cpu->x[rs2];
			break;
		case AND: 
			cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
			break;
		case REMU:
			cpu->x[rd] = (uint32_t)cpu->x[rs1] % (uint32_t)cpu->x[rs2];
			break;
		case XOR:
			cpu->x[rd] = cpu->x[rs1] ^ cpu->x[rs2];
			break;
		case DIV:
			cpu->x[rd] = (int32_t)cpu->x[rs1] / (int32_t)cpu->x[rs2];
			break;
		case SLL:
			cpu->x[rd] = (unsigned int)cpu->x[rs1] << (unsigned int)(cpu->x[rs2] % 32);
			break;
		case DIVU:
			cpu->x[rd] = (uint32_t)cpu->x[rs1] / (uint32_t)cpu->x[rs2];
			break;
		case SRL:
			cpu->x[rd] = (unsigned int)cpu->x[rs1] >> (unsigned int)(cpu->x[rs2] % 32);
			break;
		case SRA:
			cpu->x[rd] = (int)cpu->x[rs1] >> (unsigned int)(cpu->x[rs2] % 32);
			break;
		default:
			perror("exec_LA: invalid instruction");
			exit(EXIT_FAILURE);
	}	
	}
}

//logic and arithmetic
void exec_LAI(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t imm = instr.rs2_or_imm;

	if(rd != 0){
	switch(instr.op){
		case ADDI: 
			cpu->x[rd] = cpu->x[rs1] + imm;
			break;
		case ORI: 
			cpu->x[rd] = cpu->x[rs1] | imm;
			break;
		case ANDI:
			cpu->x[rd] = cpu->x[rs1] & imm;
			break;
		case XORI:
			cpu->x[rd] = cpu->x[rs1] ^ imm;
			break;
		case SLLI:
			cpu->x[rd] = (unsigned int)cpu->x[rs1] << (imm % 32); 
			break;
		case SRLI:
			cpu->x[rd] = (unsigned int)cpu->x[rs1] >> (imm % 32); 
			break;
		case SRAI:
			cpu->x[rd] = (int)cpu->x[rs1] >> (imm % 32); 
			break;
		default:
			perror("exec_LAI: invalid instruction");
			exit(EXIT_FAILURE);
	}
	}
}

//load
void exec_LD(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rd = instr.rd_or_imm;
	int32_t rs1 = instr.rs1;
	int32_t imm = instr.rs2_or_imm;

	int offset;
	uint8_t* address;

	if(rd != 0){
	switch(instr.op){
		case LW:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 2);
			address_check(offset);
			address = mem->data + offset;
			int32_t data;
		        endian_wrapper(&data, address, sizeof(int32_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		/*case LB:{
			offset = (cpu->x[rs1] + imm);
			address_check(offset);
			address = mem->data + offset;
			int8_t data;
		        endian_wrapper(&data, address, sizeof(int8_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case LH:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 1);
			address_check(offset);
			address = mem->data + offset;
			int16_t data;
		        endian_wrapper(&data, address, sizeof(int16_t));
			cpu->x[rd] = (int32_t)data;
			break;}
		case LBU:{
			offset = (cpu->x[rs1] + imm);
			address_check(offset);
			address = mem->data + offset;
			uint8_t data;
		        endian_wrapper(&data, address, sizeof(uint8_t));
			cpu->x[rd] = (uint32_t)data;
			break;}
		case LHU:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 1);
			address_check(offset);
			address = mem->data + offset;
			uint16_t data;
		        endian_wrapper (&data, address, sizeof(uint16_t));
			cpu->x[rd] = (uint32_t)data;
			break;}*/
		default:
			perror("exec_LD: invalid instruction");
			exit(EXIT_FAILURE);
	}
	}
}

//store
void exec_ST(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;
	int32_t imm = instr.rd_or_imm;

	int offset;
	uint8_t* address;

	switch(instr.op){
		case SW:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 2);
			address_check(offset);
			address = mem->data + offset;
			int32_t data = cpu->x[rs2];
			endian_wrapper(address, &data, sizeof(int32_t));
			break;}
		/*case SB:{
			offset = (cpu->x[rs1] + imm);
			address_check(offset);
			address = mem->data + offset;
			int8_t data = cpu->x[rs2];
			endian_wrapper(address, &data, sizeof(int8_t));
			break;}
		case SH:{
			offset = (cpu->x[rs1] + imm) & ((~0) << 1);
			address_check(offset);
			address = mem->data + offset;
			int16_t data = cpu->x[rs2];
			endian_wrapper(address, &data, sizeof(int16_t));
			break;}*/
		default:
			perror("exec_ST: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

//conditional branch
void exec_CB(INSTR instr, CPU *cpu, MEMORY *mem){
	int32_t rs1 = instr.rs1;
	int32_t rs2 = instr.rs2_or_imm;
	int32_t imm = instr.rd_or_imm;

	switch(instr.op){
		case BEQ:
			if(cpu->x[rs1] == cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case BNE:
			if(cpu->x[rs1] != cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case BLT:
			if(cpu->x[rs1] < cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case BGE:
			if(cpu->x[rs1] >= cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case BLTU:
			if((uint32_t)cpu->x[rs1] < (uint32_t)cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		case BGEU:
			if((uint32_t)cpu->x[rs1] >= (uint32_t)cpu->x[rs2]){
				cpu->pc += imm;//pc = pc+imm
			}
			else cpu->pc += 4;// pc = pc+4;
			break;
		default:
			perror("exec_CB: invalid instruction");
			exit(EXIT_FAILURE);
	}
}

//rd がゼロレジスタの時は代入させておいて最後に代入がいい?
void exec_instr(INSTR instr, CPU *cpu, MEMORY *mem, IO *io, STATE *state){

	tag_check(state->tags, cpu->pc);

	if(instr.op == LUI){
		int32_t rd = instr.rd_or_imm;
		int32_t imm = instr.rs2_or_imm;
		if(rd != 0)cpu->x[rd] = imm;
		cpu->pc += 4;// pc = pc+4;
	}
	else if(LA_first <= instr.op && instr.op <= LA_last){
		exec_LA(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(LAI_first <= instr.op && instr.op <= LAI_last){
		exec_LAI(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == LW){
		exec_LD(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == SW){
		exec_ST(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == AUIPC){
		int32_t rd = instr.rd_or_imm;
		int32_t imm = instr.rs2_or_imm;
		if(rd != 0)cpu->x[rd] = cpu->pc + imm; //rd = pc+imm
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == JAL){
		int32_t rd = instr.rd_or_imm;
		int32_t imm = instr.rs2_or_imm;
		if(rd != 0)cpu->x[rd] = cpu->pc + 4;// rd = pc+4;
		cpu->pc += imm; //pc = pc + imm
	}
	else if(instr.op == JALR){
		int32_t rd = instr.rd_or_imm;
		int32_t rs1 = instr.rs1;
		int32_t imm = instr.rs2_or_imm;
		if(rd != 0)cpu->x[rd] = cpu->pc + 4; // rd = pc+4;
		int address = (cpu->x[rs1] + imm) & ((~0) << 2);
		cpu->pc = address; //pc = rs1 + imm
	}
	else if(CB_first <= instr.op && instr.op <= CB_last){
		exec_CB(instr, cpu, mem);
	}
	else if(instr.op == FLW){
		exec_FLW(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == FSW){
		exec_FSW(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(FLA_first <= instr.op && instr.op <= FLA_last){
		exec_FLA(instr, cpu, mem);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == INW || instr.op == INF){
		exec_IN(instr, cpu, mem, io);
		cpu->pc += 4;// pc = pc+4;
	}
	else if(instr.op == OUTB || instr.op == OUTW){
		exec_OUT(instr, cpu, mem, io);
		cpu->pc += 4;// pc = pc+4;
	}
	else{
		perror("exec_instr: invalid instruction");
		exit(EXIT_FAILURE);
	}
	
	state_update(state, *cpu, instr);

}

INSTR fetch(CPU *cpu, MEMORY *mem){
	uint32_t addr_instr = (cpu->pc)/4;
	return mem->instr[addr_instr];
}

int run_to_the_end(CPU *cpu, MEMORY *mem, IO *io, OPTION option, STATE *state){
	INSTR instr = fetch(cpu, mem);
	int bp;
	
	while(instr.op != END){
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
	INSTR instr = fetch(cpu, mem);

	if(instr.op != END){
		putchar('\n');
		printf("pc: %d: ", cpu->pc);
		exec_instr(instr, cpu, mem, io, state);

		ASSEM assem;
		//assem_init(&assem);
		disassem_instr(instr, &assem);
		//printf(" 0b");
		//print_binary(instr);
		//printf(": ");
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
