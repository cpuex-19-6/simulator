#ifndef INSTRUCTION_INCLUDE
#define INSTRUCTION_INCLUDE

//opecode, func3, func7
#define OP_LUI	0b0110111 //lui

#define OP_LA	0b0110011 //logic and arithmethic
#define F3_A	0b000 //arithmetic
#define F7_ADD	0b0000000 //add
#define F7_SUB	0b0100000 //sub

#define F3_XOR	0b100 //xor, div
#define F7_XOR	0b0000000 //xor
#define F7_DIV	0b0000001 //div

#define F3_OR	0b110 //or, rem
#define F7_OR	0b0000000 //or
#define F7_REM	0b0000001 //rem

#define F3_AND	0b111 //and, remu
#define F7_AND	0b0000000 //and
#define F7_REMU	0b0000001 //remu

#define F3_SL	0b001 //shift left
#define F7_SLL	0b0000000 //sll

#define F3_SR	0b101 //shift right, divu
#define F7_DIVU	0b0000001 //divu
#define F7_SRL	0b0000000 //srl
#define F7_SRA	0b0100000 //sra

#define OP_LAI	0b0010011 //logic and arithmetic immediate
#define F3_ADDI	0b000 //addi
#define F3_XORI	0b100 //xori
#define F3_ORI	0b110 //ori
#define F3_ANDI	0b111 //andi
#define F3_SLI	0b001 //shift left immediate
#define F7_SLLI	0b0000000 //slli
#define F3_SRI	0b101 //shift right immediate
#define F7_SRLI	0b0000000 //srli
#define F7_SRAI	0b0100000 //srai

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

#define OP_FLW 0b0000111 //flw
#define F3_FLW 0b010 //flw

#define OP_FSW 0b0100111 //fsw
#define F3_FSW 0b010 //fsw

#define F3_RM 0b000 //f3 as rm

#define OP_FLA	0b1010011 //floating point arithmetic logic and others
#define F7_FADD	0b0000000 //fadd
#define F7_FSUB	0b0000100 //fsub
#define F7_FMUL	0b0001000 //fnul
#define F7_FDIV	0b0001100 //fdiv
#define F7_FSQRT 0b0101100 //fsqrt
#define RS2_FSQRT 0b00000 //fsqrt

#define F7_FSGNJ 0b0010000 //fsgnj
#define F3_FSGNJ 0b000 //fsgnj
#define F3_FSGNJX 0b010 //fsgnjx
#define F3_FSGNJN 0b001 //fsgnjn

#define F7_FROUND 0b1000000 //froundrm
#define RM_RNE 0b000 //RNE
#define RM_RTZ 0b001 //RTZ
#define RM_RDN 0b010 //RDN
#define RM_RUP 0b011 //RUP
#define RM_RMM 0b100 //RMM
#define RS2_FROUND 0b00000 //fround

#define F7_TOF 0b1100000 //to f
#define RS2_ITOF 0b00000 //itof
#define RS2_UTOF 0b00001 //utof

#define F7_TOI 0b1101000 //to i
#define RS2_FTOI 0b00000 //ftoi
#define RS2_FTOUI 0b00001 //ftoui

#define F7_FMVI 0b1110000 //fmvi
#define F3_FMVI 0b000 //fmvi
#define RS2_FMVI 0b00000 //fmvi

#define F7_IMVF 0b1111000 //imvf
#define F3_IMVF 0b000 //imvf
#define RS2_IMVF 0b00000 //imvf

#define F7_FCOMPARE 0b1010000 //floating point compare
#define F3_FEQ 0b010 //feq
#define F3_FLT 0b001 //flt
#define F3_FLE 0b000 //fle

#define OP_IN 0b0001011 //input
#define INW 0b00000000000000000010 //inw
#define INF 0b01000000000000000010 //inw

#define OP_OUT 0b0101011 //input
#define OUTW12 0b000000000000 //outw
#define OUTW8 0b00000000 //outw
#define OUTB12 0b000000000000 //outw
#define OUTB8 0b01000000 //outw

#endif
