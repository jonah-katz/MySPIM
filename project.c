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
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

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

