#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    if (ALUControl == 0b000) // Z = A + B
        *ALUresult = A + B;
    else if(ALUControl == 0b001) // Z = A - B
        *ALUresult = A - B;
    else if(ALUControl == 0b010) // if A < B, Z = 1; otherwise, Z = 0
    {
        // Using two's compliment value
        int a = A;
        int b = B;
        *ALUresult = (a < b) ?  1 : 0;
    }
    else if(ALUControl == 0b011) // if A < B, Z = 1; otherwise, Z = 0 (unsigned)
        *ALUresult = (A < B) ?  1 : 0;
    else if(ALUControl == 0b100) // Z = A AND B
        *ALUresult = A & B;
    else if(ALUControl == 0b101) // Z = A OR B
        *ALUresult = A | B;
    else if(ALUControl == 0b110) // Z = B << 16
        *ALUresult = B << 16;
    else if(ALUControl == 0b111) // Z = NOT A
        *ALUresult = ~A;
    else // Return if ALUControl is invalid, so Zero is not changed
        return; 

    *Zero = (*ALUresult == 0) ? 1 : 0; 
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // Check for halt condition
    if (PC % 4 != 0)
        return 1;

    // Fetch the current instruction
    *instruction = Mem[(PC) >> 2];

    return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    unsigned temp;

    // funct
    temp = 0b111111;
    *funct = (instruction & temp);

    // offset
    temp = 0b1111111111111111;
    *offset = (instruction & temp);
    
    // jsec
    temp = 0b11111111111111111111111111;
    *jsec = (instruction & temp);

    // r3
    temp = 0b1111100000000000;
    *r3 = (instruction & temp) >> 11;

    // r2
    temp = 0b111110000000000000000;
    *r2 = (instruction & temp) >> 16;

    // r1
    temp = 0b11111000000000000000000000;
    *r1 = (instruction & temp) >> 21;

    // op
    temp = 0b11111100000000000000000000000000;
    *op = (instruction & temp) >> 26;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{   
    // R-Type
    // ADD, SUB, AND, OR, SET ON LESS THAN, SET ON LESS THAN UNSIGNED
    if (op == 0b000000)
    {
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0b111;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
    }
    // ADDI
    else if (op == 0b001000)
    {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0b000;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    // LOAD WORD
    else if (op == 0b100011)
    {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUOp = 0b000;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    // STORE WORD
    else if (op == 0b101011)
    {
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0b000;
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        controls->RegWrite = 0;
    }
    // LOAD UPPER IMMEDIATE
    else if (op == 0b001111)
    {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0b110;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    // BRANCH ON EQUAL
    else if (op == 0b000100)
    {
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0b001;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
    }
    // SET LESS THAN IMMEDIATE
    else if (op == 0b001010)
    {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0b010;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    // SET LESS THAN IMMEDIATE UNSIGNED
    else if (op == 0b001011)
    {
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0b011;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    }
    // JUMP
    else if (op == 0b000010)
    {
        controls->RegDst = 2;
        controls->Jump = 1;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0b000;
        controls->MemWrite = 0;
        controls->ALUSrc = 2;
        controls->RegWrite = 0;
    }
    else
        return 1;

    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Just read registers
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value) {
    // Set extended_value to all 0s
    *extended_value = 0;
    // Check if the offset is negative
    if ((offset >> 15) == 1) // It is negative
        // Extension by setting the upper 16 bits to 1.
        *extended_value = offset | 0xFFFF0000;
    else
        // If the sign bit is not set, the number is positive.
        // We extend the sign by setting the upper 16 bits to 0.
        *extended_value = offset;

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    // Use data1 and data2
    if (ALUSrc == 0)
    {
        // R-TYPE
        if (ALUOp == 0b111)
        {
            // ADD
            if (funct == 0b100000)
                ALU(data1, data2, 0b000, ALUresult, Zero);
            // SUB
            else if (funct == 0b100010)
                ALU(data1, data2, 0b001, ALUresult, Zero);
            // AN
            else if (funct == 0b100100)
                ALU(data1, data2, 0b100, ALUresult, Zero);
            // OR
            else if (funct == 0b100101)
                ALU(data1, data2, 0b101, ALUresult, Zero);
            // SLT
            else if (funct == 0b101010)
                ALU(data1, data2, 0b010, ALUresult, Zero);
            // SLTU
            else if (funct == 0b101011)
                ALU(data1, data2, 0b011, ALUresult, Zero);
        }
        // BEQ
        else if (ALUOp == 0b001)
            ALU(data1, data2, 0b001, ALUresult, Zero);
        else
            return 1;
    }
    // Use data1 and extended_value
    else if (ALUSrc == 1)
    {
        if (ALUOp == 0b000)
            ALU(data1, extended_value, 0b000, ALUresult, Zero);
        else if (ALUOp == 0b001)
            ALU(data1, extended_value, 0b001, ALUresult, Zero);
        else if (ALUOp == 0b010)
            ALU(data1, extended_value, 0b010, ALUresult, Zero);
        else if (ALUOp == 0b011)
            ALU(data1, extended_value, 0b011, ALUresult, Zero);
        else if (ALUOp == 0b100)
            ALU(data1, extended_value, 0b100, ALUresult, Zero);
        else if (ALUOp == 0b101)
            ALU(data1, extended_value, 0b101, ALUresult, Zero);
        else if (ALUOp == 0b110)
            ALU(data1, extended_value, 0b110, ALUresult, Zero);
        else if (ALUOp == 0b111)
            ALU(data1, extended_value, 0b111, ALUresult, Zero);
        else
            return 1;
    }
    // Don't care
    else if (ALUSrc == 2)
        return 0;
    else
        return 1;

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // If nothing happens
    if (MemRead == 0 && MemWrite == 0)
        return 0;
    
    // If halt condit
    if (ALUresult % 4 != 0)
        return 1;
    
    // If we should read from memory
    if (MemRead == 1)
    {
        *memdata = Mem[ALUresult >> 2];
        return 0;
    }
    // If we should read into memory
    else if (MemWrite == 1)
    {
        Mem[ALUresult >> 2] = data2;
        return 0;
    }

    return 1;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite == 1)
        // Indicates load instruction, no need to check what RegDst is, it's always r2 (rt)
        if(MemtoReg == 1)
            Reg[r2] = memdata;
        else
            // Depending on regDest, write into that reg
            if(RegDst == 1)
                Reg[r3] = ALUresult;
            else
                Reg[r2] = ALUresult;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // Jump to destination and update PC
    if (Jump == 1)
        *PC = (jsec << 2) | (*PC & 0xf0000000);
    
    // If the branch is taken
    else if (Branch == 1)
        // Branch is true.
        if (Zero == 1)
            // Need to change word offset to byte offset
            *PC = *PC + 4 + (extended_value << 2);
        else
            *PC = *PC + 4;
    else
        *PC = *PC + 4; // Update PC if nothing else occurs
}
