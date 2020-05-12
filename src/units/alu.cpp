#include "alu.h"
#include "../machine/instruction.h"

void InitALUControl(EALU ALUControls[], EALU RFormats[])
{
    //ALUControls[2] = EALU::ADD;   // J
    //ALUControls[3] = EALU::ADD;   // JAL

    ALUControls[4]  = EALU::SUB;  // BEQ
    ALUControls[5]  = EALU::SUB;  // BNE

    ALUControls[8]  = EALU::ADD;  // ADDI
    ALUControls[9]  = EALU::ADD;  // ADDIU

    ALUControls[10] = EALU::SLT;  // SLTI
    ALUControls[11] = EALU::SLTU; // SLTIU

    ALUControls[12] = EALU::AND;  // ANDI
    ALUControls[13] = EALU::OR;   // ORI

    ALUControls[15] = EALU::SLL;   // LUI

    ALUControls[32] = EALU::ADD;  // LB
    ALUControls[33] = EALU::ADD;  // LH
    ALUControls[35] = EALU::ADD;  // LW
    ALUControls[36] = EALU::ADD;  // LBU
    ALUControls[37] = EALU::ADD;  // LHU

    ALUControls[40] = EALU::ADD;  // SB
    ALUControls[41] = EALU::ADD;  // SH
    ALUControls[43] = EALU::ADD;  // SW

    RFormats[0]  = EALU::SLL;  // SLL
    RFormats[2]  = EALU::SRL;  // SRL
    RFormats[3]  = EALU::SRA;  // SRA

    RFormats[8]  = EALU::ADD;  // JR
    
    RFormats[32] = EALU::ADD;  // ADD
    RFormats[33] = EALU::ADD;  // ADDU
    RFormats[34] = EALU::SUB;  // SUB
    RFormats[35] = EALU::SUB;  // SUBU

    RFormats[36] = EALU::AND;  // AND
    RFormats[37] = EALU::OR;   // OR
    RFormats[39] = EALU::NOR;  // NOR
    
    RFormats[42] = EALU::SLT;  // SLT
    RFormats[43] = EALU::SLTU; // SLTU
}

EALU GetALUControl(uint8_t op, uint8_t funct)
{
    static EALU ALUControls[256]{};
    static EALU RFormats[256]{};
    static bool init = false;

    if (!init)
    {
        InitALUControl(ALUControls, RFormats);
        init = true;
    }

    if (op == 0)
    {
        return RFormats[funct];
    }
    return ALUControls[op];
}
