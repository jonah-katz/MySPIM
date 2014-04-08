#include "spimcore.h"

unsigned getProperRegisterToWriteTo(char RegDst,unsigned r2,unsigned r3);

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
        *instruction = Mem[MemoryLocation];
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
                    controls->RegDst = 1;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 0;
                    controls->ALUOp = 7;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 0;
                    controls->RegWrite = 1;
                break;

        //  add imidiate
        case 001000:
                    controls->RegDst = 0;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 0;
                    controls->ALUOp = 0;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 1;
                    controls->RegWrite = 1;
                break;

        //  load unsigned immidiate
        case 001111:
                    controls->RegDst = 0;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 0;
                    controls->ALUOp = 6;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 1;
                    controls->RegWrite = 1;
                break;

        //  load word
        case 110001:
                    controls->RegDst = 0;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 1;
                    controls->MemtoReg = 1;
                    controls->ALUOp = 0;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 1;
                    controls->RegWrite = 1;
                break;

        //  store word
        case 111001:
                    controls->RegDst = 3;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 3;
                    controls->ALUOp = 0;
                    controls->MemWrite = 1;
                    controls->ALUSrc = 1;
                    controls->RegWrite = 0;
                break;

        //  branch equal
        case 000100:
                    controls->RegDst = 3;
                    controls->Jump = 0;
                    controls->Branch = 1;
                    controls->MemRead = 0;
                    controls->MemtoReg = 3;
                    controls->ALUOp = 1;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 0;
                    controls->RegWrite = 0;
                 break;
        //  set on less than immidiate
        case 001010:
                    controls->RegDst = 0;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 0;
                    controls->ALUOp = 2;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 1;
                    controls->RegWrite = 1;
                break;

        //  set on less than unsigned
        case 001011:
                    controls->RegDst = 0;
                    controls->Jump = 0;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 0;
                    controls->ALUOp = 3;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 1;
                    controls->RegWrite = 1;
                break;

        //  jump
        case 000010:
                    controls->RegDst = 0;
                    controls->Jump = 1;
                    controls->Branch = 0;
                    controls->MemRead = 0;
                    controls->MemtoReg = 0;
                    controls->ALUOp = 0;
                    controls->MemWrite = 0;
                    controls->ALUSrc = 0;
                    controls->RegWrite =  0;
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
/*  written by Jonah Katz   */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    unsigned signBit = offset >> 15; /* the sign but is the left most bit of the 16 bit string */
    if(signBit == 1) {
        /* negative, expand to 32 bits */
        *extended_value = offset | 0xFFFF0000; /* left half are all 1's
    } else {
        /* not negative, just keep at 16 bits */
        *extended_value = offset & 0x0000ffff;
    }
}

/* ALU operations */
/* 10 Points */
/*  written by Zach Chenet  */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

    //  check to see which data is being changed based on the ALUsrc
    if(ALUSrc == 1){
        data2 = extended_value;
    }

    //  check to see if ALUOp is 7 meaning R type instruction
    //  creates switch function to change ALUOp code for desired ALU operation
    if(ALUOp == 7){
        switch(funct){

            //  add function field
            case 100000:
                        ALUOp = 0;
                        break;

            //  subtract function field
            case 100010:
                        ALUOp = 1;
                        break;

            //  less than signed function field
            case 101010:
                        ALUOp = 2;
                        break;

            //  less than unsiged field
            case 101011:
                        ALUOp = 3;
                        break;

            //  And function field
            case 100100:
                        ALUOp = 4;
                        break;

            //  Or function field
            case 100101:
                        ALUOp = 5;
                        break;

            //  shift left field
            case 000100:
                        ALUOp = 6;
                        break;

            //  Not function field
            case 100111:
                        ALUOp = 7;
                        break;

            //  halt occurs due to not proper function field
            default:
                        return 1;
            }

            //  sending the information to the ALU depending on the function field
            ALU(data1,data2, ALUOp,ALUresult,Zero);

        }

        //  send the information to ALU for non function field
        else{
            ALU(data1,data2,ALUOp,ALUresult,Zero);
        }

        return 0;
    }
}

/* Read / Write Memory */
/* 10 Points */
/*  written by Jonah Katz  */

int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    if(MemRead) {
        /* reading from memory */
        /* check to make sure ALUresult is properly aligned word. Although, it surely will be because this check has been done in instruction_fetch */
        if(ALUresult %4 == 0) {
            memdata = Mem[ALUresult >> 2]; /* shift right by 2 to get desired contents */
        } else {
            /* not proper; halt */
            return 1;
        }
    }

    if(MemWrite) {
        /* writing to memory */
        if(ALUresult %4 == 0) {
            Mem[ALUresult >> 2] = data2;
        } else {
            /* not proper; halt */
            return 1;
        }

    }

    return 0;

}


/* Write Register */
/* 10 Points */
/*  written by Jonah Katz  */

void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite) {
        /* ok, should proceed */
        switch (MemtoReg) {
            case 0:
                /* value is coming from ALUResult */
                Reg[getProperRegisterToWriteTo(RegDst,r2,r3)] = ALUresult;
                break;
            case 1:
                /* value is coming from memory */
                Reg[getProperRegisterToWriteTo(RegDst,r2,r3)] = memdata;
                break;
        }

    }

}
/*  written by Jonah Katz  */
unsigned getProperRegisterToWriteTo(char RegDst,unsigned r2,unsigned r3) {
    switch (RegDst) {
        case 0:
            return r2;
            break;
        default:
            return r3;
            break;
    }
}

/* PC update */
/* 10 Points */
/*  written by Zach Chenet  */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //  increases the program counter to the next word
    *PC += 4;

    //  checks to see if branching occured and if we got a zero
    //  if this is the case add the extended value to the PC
    if(Zero == 1 && Branch == 1){
        *PC += extended_value << 2;
    }

    //  check to see if need to jump
    //  combines the jump which is shifted to left by two to match up with updated PC
    if(Jump == 1){
        *PC = (jsec << 2) | (*PC & 0xf0000000);
    }
}

