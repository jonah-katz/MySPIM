#include "spimcore.h"


/* ALU */
/* 10 Points */
/*  written by Jonah Katz   */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

    switch ((int)ALUControl) { //Cast char to int for switch statement
        case 000:
            *ALUresult = A + B;
            break;
        case 001:
            *ALUresult = A - B;
            break;
        case 010:
            if((signed)A < (signed)B) { //if we're doing a comparison, must be signed. aka int
                *ALUresult = 1;
            } else {
                *ALUresult = 0;
            }
            break;
        case 011:
            if(A < B) {
                *ALUresult = 1;
            } else {
                *ALUresult = 0;
            }
            break;
        case 100:
            *ALUresult = A&B;
            break;
        case 101:
            *ALUresult = A|B;
            break;
        case 110:
            B<<16;
            break;
        case 111:
            *ALUresult = ~A;
            break;
    }

    if(*ALUresult == 0 ){
        *Zero = 1;
    } else {
        *Zero = 0;
    }

}

/* instruction fetch */
/* 10 Points */
/*  written by Zach Chenet  */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //  check for word alignment is a multiple of 4
    if(PC % 4 == 0){

        //  use correct part of PC
        unsigned MemoryLocation = PC >> 2;

        //  set insruction to the disired instruction
        *instruction = mem[MemoryLocation];
        return 0;
    }
    //  if no word alignment
    else{

        return 1;
    }
}


/* instruction partition */
/* 10 Points */
/*  written by Jonah Katz   */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

    /* partition offsets to get desired instruction part */
    unsigned rPart	= 0x1f;
	unsigned operationsPart		= 0x0000003f;
    unsigned offsetPart	= 0x0000ffff;
	unsigned jsecPart	= 0x03ffffff;

    *op = (instruction >> 26 ) & operationsPart; // op = instruction[31 - 26]
    *r1 = (instruction >> 21) & rPart; // r1 = instruction[25 - 21]
    *r2 = (instruction >> 16) & rPart; // r2 = instruction[20 - 16]
    *r3= (instruction >> 11) & rPart; // r3 = instruction[15 - 11]
    *funct = instruction & operationsPart; //funct = instruction[5-0]
    *offset = instruction & offsetPart; //offset = instruction [15-0]
    *jsec = instruction & jsecPart;//jsec = instruction [25-0]

}



/* instruction decode */
/* 15 Points */
/*  written by Zach Chenet  */
int instruction_decode(unsigned op,struct_controls *controls)
{

    //  operation to see how Op Codes decodes
    switch(op){

        //  r-type opCode
        case 00000:
                    control->RegDst = 1;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 0;
                    control->ALUOp = 7;
                    control->MemWrite = 0;
                    control->AluSrc = 0;
                    control->RegWrite = 1;
                break;

        //  add imidiate
        case 001000:
                    control->RegDst = 0;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 0;
                    control->ALUOp = 0;
                    control->MemWrite = 0;
                    control->AluSrc = 1;
                    control->RegWrite = 1;
                break;

        //  load unsigned immidiate
        case 001111:
                    control->RegDst = 0;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 0;
                    control->ALUOp = 6;
                    control->MemWrite = 0;
                    control->AluSrc = 1;
                    control->RegWrite = 1;
                break;

        //  load word
        case 110001:
                    control->RegDst = 0;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 1;
                    control->MemtoReg = 1;
                    control->ALUOp = 0;
                    control->MemWrite = 0;
                    control->AluSrc = 1;
                    control->RegWrite = 1;
                break;

        //  store word
        case 111001:
                    control->RegDst = 3;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 3;
                    control->ALUOp = 0;
                    control->MemWrite = 1;
                    control->AluSrc = 1;
                    control->RegWrite = 0;
                break;

        //  branch equal
        case 000100:
                    control->RegDst = 3;
                    control->Jump = 0;
                    control->Branch = 1;
                    control->MemRead = 0;
                    control->MemtoReg = 3;
                    control->ALUOp = 1;
                    control->MemWrite = 0;
                    control->AluSrc = 0;
                    control->RegWrite = 0;
                 break;
        //  set on less than immidiate
        case 001010
                    control->RegDst = 0;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 0;
                    control->ALUOp = 2;
                    control->MemWrite = 0;
                    control->AluSrc = 1;
                    control->RegWrite = 1;
                break;

        //  set on less than unsigned
        case 001011
                    control->RegDst = 0;
                    control->Jump = 0;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 0;
                    control->ALUOp = 3;
                    control->MemWrite = 0;
                    control->AluSrc = 1;
                    control->RegWrite = 1;
                break;

        //  jump
        case 000010
                    control->RegDst = 0;
                    control->Jump = 1;
                    control->Branch = 0;
                    control->MemRead = 0;
                    control->MemtoReg = 0;
                    control->ALUOp = 0;
                    control->MemWrite = 0;
                    control->AluSrc = 0;
                    control->RegWrite =  0;
                break;

        //  return 1 if halt
        default:
                    return 1;
    }
    return 0;

}

/* Read Register */
/* 5 Points */
/*  written by Jonah Katz   */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    /* write r1 and r2 (which are addressess in Reg, into data1 and data2 */
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

