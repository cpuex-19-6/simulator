#ifndef STRUCT_INCLUDE
#define STRUCT_INCLUDE

#include <stdio.h>
#include <stdint.h>
#include <string.h>

//CPU

typedef int32_t REG; //register
#define REG_N 32

typedef float FREG; //floating point register
#define FREG_N 32

typedef struct cpu_t {
	REG x[REG_N];
	FREG f[FREG_N];
	unsigned int pc;
} CPU;

//instruction

typedef struct instr_t{
	int op;
	int rd_or_imm;
	int rs1;
	int rs2_or_imm;
}INSTR;

typedef enum instr_type_t{
	R,
	R_sub,
	R_sub_sub,
	R_sub_sub_sub,
	I, 
	S, 
	B, 
	U, 
	J,
}INSTR_TYPE;

//memory

typedef struct memory_t {
	INSTR *instr;
	uint8_t *data;
} MEMORY;

//option

typedef enum ftype_t{
	BIN,
	TXT,
}FTYPE;

enum MODE{
	NONE,
	RUN,
	STEP,
	QUIT,
};

typedef enum mem_data_t {
	Int, //int
	Uint, //unsigned int
	Float, //float
}MEM_DATA_TYPE;

typedef enum mem_data_size_t{
	Byte, //byte
	Half, //half
	Word, //word
}MEM_DATA_SIZE;

typedef struct mem_print_sub_t {
	MEM_DATA_TYPE type;
	MEM_DATA_SIZE size;
	uint32_t addr;
}MEM_PRINT_SUB;

typedef struct mem_print_t{
	MEM_PRINT_SUB *mp;
	int num;
}MEM_PRINT;

typedef struct break_point_t{
	int *bp;
	int num;
}BREAKPOINT;

typedef struct option_t {
	FILE *cmd_in;
	int prompt;
	FTYPE ftype_instr;
	FTYPE ftype_data;
	FTYPE ftype_output;
	char *fname_instr;
	char *fname_data;
	char *fname_output;
	char *fname_tags;
	MODE mode;
	uint32_t reg;
	uint32_t reg16;
	uint32_t freg;
	BREAKPOINT breakpoint;
	int to_the_end;
	MEM_PRINT mem_print;
	int step_n;
}OPTION;

//IO

typedef struct io_t {
	FTYPE ftype_in;
	FILE *input;
	FTYPE ftype_out;
	FILE *output;
}IO;

//disassemble

typedef struct assem_t {
	INSTR_TYPE itype;
	char reg[4];
	int32_t rd_or_imm;
	int32_t rs1;
	int32_t rs2_or_imm;
	char mnemonic[10];
	char rm[3];
}ASSEM;

//status

typedef struct tag_t{
	char label[101];
	unsigned int pc;
	long long unsigned int count;
	struct tag_t *next;
}TAG;

typedef struct state_t{
	long long unsigned int instr_num;
	TAG *tags;
	unsigned int heap_top;
	//unsigned int heap_bottom;
	//unsigned int stack_top;
	unsigned int stack_bottom;
	long long unsigned int *op;
}STATE;

#endif
