#pragma once

#include <string>
#include "register.h"

class Instruction
{
public:
    virtual void Execute() = 0;

    static Instruction* MakeInstance(uint32_t rawInst);

protected:
    Instruction(uint32_t raw) {}

    // Raw instruction.
    uint32_t raw = 0;

    ERegister op;
    int8_t rs;
    int8_t rt;
};
