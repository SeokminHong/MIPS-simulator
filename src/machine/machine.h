#pragma once

#include <string>
#include <vector>
#include <array>
#include <stack>
#include "register.h"
#include "instruction.h"

#define MAX_INSTRUCTION 4096

class Machine
{
public:
    Machine();

    void AddInstruction(uint32_t instruction);

    inline void SetMaxCycle(int newMaxCycle)
    {
        maxCycle = newMaxCycle;
    }

    void SetRegister(ERegister reg, uint32_t regVal);

    void Run();

private:
    void Cycle();

    uint32_t pc = 0;

    std::array<uint32_t, REG_MAX> registers;

    std::vector<Instruction> instructions;

    // Only used for the extra challenge of Phase1.
    // jal: push PC+4 into $ra.
    // jr: pop PC from $ra.
    std::stack<uint32_t> ra;

    int maxCycle = 0;
};