#pragma once

#include <string>
#include <vector>
#include <array>
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

    int pc = 0;

    std::array<uint32_t, REG_MAX> registers;

    std::vector<Instruction> instructions;

    int maxCycle = 0;
};