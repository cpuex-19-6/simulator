#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "instruction.h"
#include "analyse.h"
#include "memory.h"

void state_init(STATE *state){
	state->instr_num = 0;
	state->tags = NULL;
	state->heap_top = 0;
	//state->heap_bottom = 0;
	//state->stack_top = DATA_MEM_SIZE;
	state->stack_bottom = DATA_MEM_SIZE;
	state->op = malloc(sizeof(long long int)*TOTAL_INSTR);
}

void tag_set(TAG **tag, FILE *fp, char *s, unsigned int *n){
	if(fscanf(fp, "%s%u", s, n) == 2){
	
	if(*tag == NULL)
		*tag = malloc(sizeof(TAG));

	strcpy((*tag)->label, s);
	(*tag)->pc = *n;
	(*tag)->count = 0;
	(*tag)->next = NULL;

	tag_set(&((*tag)->next), fp, s, n);
	}
}
void free_tags(TAG *tag){
	if(tag != NULL){
		TAG *next;
		next = tag->next;
		free(tag);
		free_tags(next);
	}
}

void state_set(STATE *state, OPTION option){
	if(option.fname_tags != NULL){

		FILE *fp;
		if((fp = fopen(option.fname_tags, "r")) == NULL){
			perror("failed to open tag file");
			exit(EXIT_FAILURE);
		}
		printf("using \"%s\" as tag file\n", option.fname_tags);
		char s[101];
		unsigned int n;
		tag_set(&(state->tags), fp, s, &n);

		fclose(fp);
	}
}

void state_free(STATE *state){
	free(state->op);
	free_tags(state->tags);
}

void print_op(int op){
	switch(op){
		case LUI:
			printf("lui");
			break;
		case ADD:
			printf("add");
			break;
		case SUB:
			printf("sub");
			break;
		case DIV:
			printf("div");
			break;
		case DIVU:
			printf("divu");
			break;
		case REM:
			printf("rem");
			break;
		case REMU:
			printf("remu");
			break;
		case SLL:
			printf("sll");
			break;
		case XOR:
			printf("xor");
			break;
		case SRL:
			printf("srl");
			break;
		case SRA:
			printf("sra");
			break;
		case OR:
			printf("or");
			break;
		case AND:
			printf("and");
			break;
		case ADDI:
			printf("addi");
			break;
		case SLLI:
			printf("slli");
			break;
		case XORI:
			printf("xori");
			break;
		case SRLI:
			printf("srli");
			break;
		case SRAI:
			printf("srai");
			break;
		case ORI:
			printf("ori");
			break;
		case ANDI:
			printf("andi");
			break;
		case LW:
			printf("lw");
			break;
		case SW:
			printf("sw");
			break;
		case AUIPC:
			printf("auipc");
			break;
		case JAL:
			printf("jal");
			break;
		case JALR:
			printf("jalr");
			break;
		case BEQ:
			printf("beq");
			break;
		case BNE:
			printf("bne");
			break;
		case BLT:
			printf("blt");
			break;
		case BGE:
			printf("bge");
			break;
		case BLTU:
			printf("bltu");
			break;
		case BGEU:
			printf("bgeu");
			break;
		case FLW:
			printf("flw");
			break;
		case FSW:
			printf("fsw");
			break;
		case FADD:
			printf("fadd");
			break;
		case FSUB:
			printf("fub");
			break;
		case FMUL:
			printf("fmul");
			break;
		case FDIV:
			printf("fdiv");
			break;
		case FSQRT:
			printf("fsqrt");
			break;
		case FSGNJ:
			printf("fsgnj");
			break;
		case FSGNJN:
			printf("fsgnjn");
			break;
		case FSGNJX:
			printf("fsgnjx");
			break;
		case FROUNDRM_RNE:
			printf("fround_RNE");
			break;
		case FROUNDRM_RTZ:
			printf("fround_RTZ");
			break;
		case FROUNDRM_RDN:
			printf("fround_RDN");
			break;
		case FROUNDRM_RUP:
			printf("fround_RUP");
			break;
		case FROUNDRM_RMM:
			printf("fround_RMM");
			break;
		case ITOF:
			printf("itof");
			break;
		case FTOI:
			printf("ftoi");
			break;
		case FMVI:
			printf("fmvi");
			break;
		case IMVF:
			printf("imvf");
			break;
		case FEQ:
			printf("feq");
			break;
		case FLT:
			printf("flt");
			break;
		case FLE:
			printf("fle");
			break;
		case INW:
			printf("inw");
			break;
		case INF:
			printf("inf");
			break;
		case OUTB:
			printf("outb");
			break;
		case OUTW:
			printf("outw");
			break;
		default:
			perror("print_op: unknown instruction");
			exit(EXIT_FAILURE);
	}
}

void print_tag(TAG *tag){
	if(tag != NULL){
		printf("#%s\t: %llu\n", tag->label, tag->count);
		print_tag(tag->next);
	}
}

void print_state(STATE state){
	printf("Analysed data\n");
	putchar('\n');
	printf("memory access area:\n");
	printf("heap: %d byte\n", state.heap_top);
	printf("stack: %d byte\n", DATA_MEM_SIZE - state.stack_bottom);
	putchar('\n');

	printf("total executed instructions: %llu\n", state.instr_num);
	putchar('\n');

	printf("number of times each instruction executed:\n");
	putchar('\n');
	for(int i = 0; i < TOTAL_INSTR; i++){
		if(state.op[i] > 0){
			print_op(i);
			printf("\t:%llu\n", state.op[i]);
		}
	}
	putchar('\n');

	if(state.tags != NULL){
		printf("number of times each tag visited: \n");
		putchar('\n');
		print_tag(state.tags);
		putchar('\n');
	}
	
}

void tag_check(TAG *tag, int pc){
	if(tag != NULL){
		if(tag->pc == pc)
			tag->count++;
		else tag_check(tag->next, pc);
	}
}

void state_update(STATE *state, CPU cpu, INSTR instr){
	state->instr_num++;

	state->op[instr.op] = state->op[instr.op] + 1;

	int rd = instr.rd_or_imm;

	switch(rd){
		case 2:
			if(cpu.x[2] < state->stack_bottom)
				state->stack_bottom = cpu.x[2];
			break;
		case 3:
			if(cpu.x[3] > state->heap_top)
				state->heap_top = cpu.x[3];
			break;
		default:
			{}
	}

	if(cpu.x[2] <= cpu.x[3]){
		perror("stack pointer < heap pointer");
		exit(EXIT_FAILURE);
	}
}
