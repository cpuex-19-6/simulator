#ifndef INSTRUCTION_INCLUDE
#define INSTRUCTION_INCLUDE

//opecode, func3, func7
#define OP_LUI	0b0110111 //lui

#define OP_LA	0b0110011 //logic and arithmethic
#define F3_A	0b000 //arithmetic
#define F7_ADD	0b0000000 //add
#define F7_SUB	0b0100000 //sub
#define F3_OR	0b110 //or
#define F3_AND	0b111 //and

#define OP_LAI	0b0010011 //logic and arithmetic immediate
#define F3_ADDI	0b000 //addi
#define F3_ORI	0b110 //ori
#define F3_ANDI	0b111 //andi

#define OP_LD	0b0000011 //load
#define F3_LB	0b000 //lb
#define F3_LH	0b001 //lh
#define F3_LW	0b010 //lw
#define F3_LBU	0b100 //lbu
#define F3_LHU	0b101 //lhu

#define OP_ST	0b0100011 //store
#define F3_SB	0b000 //sb
#define F3_SH	0b001 //sh
#define F3_SW	0b010 //sw

#define OP_AUIPC 0b0010111 //auipc
#define OP_JAL	0b1101111 //jal
#define OP_JALR	0b1100111 //jalr

#define OP_CB	0b1100011 //conditional branch
#define F3_BEQ	0b000 //beq
#define F3_BNE	0b001 //bne
#define F3_BLT	0b100 //blt
#define F3_BGE	0b101 //bge
#define F3_BLTU	0b110 //bltu
#define F3_BGEU	0b111 //bgeu

#endif
