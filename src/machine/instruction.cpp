#include "instruction.h"
#include <cstdio>

Instruction::Instruction(uint32_t inst)
{
    instruction.raw = inst;
}

bool Instruction::IsBranchOrJump() const
{
    // J, JAL, BEQ, BNE
    if (instruction.reg.op >= 2 && instruction.reg.op <= 5)
    {
        return true;
    }

    if (instruction.reg.op == 0)
    {
        // JR, JALR
        return instruction.reg.funct == 8 ||
               instruction.reg.funct == 9;
    }
    return false;
}
